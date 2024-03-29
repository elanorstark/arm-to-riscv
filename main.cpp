#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <chrono>

#include <cstdio>
#include <cinttypes>
#include <cmath>
#include "Value.h"
#include "Instruction.h"
#include "Elf_read.h"
#include "config.h"

extern "C" {
#include "capstone/include/capstone/capstone.h"
}

// extern "C" size_t CAPSTONE_API cs_disasm(csh ud, const uint8_t *buffer, size_t size, uint64_t offset, size_t count, cs_insn **insn);

//#define CODE "\x20\x00\x80\x52\x41\x00\x80\x52\x20\x00\x00\x0b\xc0\x03\x5f\xd6"
//#define CODE "\x20\x00\x80\x52\x41\x00\x80\x52\x21\x00\x00\x0b\x00\x20\xc1\x1a\xc0\x03\x5f\xd6"
//#define CODE "\xe0\xff\x9f\x52\xe1\xff\x9f\x52\x00\x00\x01\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x04\x00\x31"
//#define CODE "\x00\x00\x80\x52\x41\x01\x80\x52\x00\x00\x01\x0b\x21\x04\x00\x51\x3f\x00\x00\x71\x40\x00\x00\x54\xfc\xff\xff\x17\xc0\x03\x5f\xd6"


int disassemble_cs(csh *handle, cs_insn **insn,
                   size_t *count,
                   Elf_read &reader) {

    size_t j;

    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, handle) != CS_ERR_OK)
        return -1;

    cs_option(*handle, CS_OPT_DETAIL, CS_OPT_ON); // slower but more detail

    *count = cs_disasm(*handle, reinterpret_cast<const uint8_t *>(reader.data), reader.length, 0x000, 0,
                       insn); // might need to be reader.length - 1

    // testing - print assembly instructions
    if (*count > 0) {
        for (j = 0; j < *count; j++) {
            // Print assembly
            printf("%X:\t%s\t%s\n", j * 4, (*insn)[j].mnemonic, (*insn)[j].op_str);
        }

    } else
        printf("ERROR: Failed to disassemble given code!\n");

    return 0;
}

Register *create_register(cs_arm64_op &operand, const csh *handle) {
    if (operand.type == ARM64_OP_REG) {
        std::string reg_name = cs_reg_name(*handle, operand.reg);
        if(reg_name=="wzr") {
            return &EmptyRegister::emptyRegister;
        }
        int reg_num = std::stoi(reg_name.substr(1, -1));
        return &Register::registers[reg_num];
    } else {
        std::cout << "Operand isn't register: " << operand.type << std::endl;
        throw std::runtime_error("Operand isn't register");
    }

}

Value *create_operand(cs_arm64_op &operand, csh *handle) {
    if (operand.type == ARM64_OP_REG) {
        return create_register(operand, handle);
    } else if (operand.type == ARM64_OP_IMM) {
        return new Const(operand.imm);
    }
    throw std::runtime_error("Operand isn't supported");
}

Instruction *create_instruction(cs_insn &this_insn, int line, cs_regs &regs_read, csh *handle) {
    cs_detail *detail = this_insn.detail;
    int op_count = detail->arm64.op_count;
    cs_arm64_op *operands = detail->arm64.operands;

//    std::cout << (this_insn.mnemonic) << "\n";

    // ARITHMETIC
    if (strcmp(this_insn.mnemonic, "add") == 0 || strcmp(this_insn.mnemonic, "adds") == 0) {
        Add* instruction = new Add(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), strcmp(this_insn.mnemonic, "adds") == 0);
        if (operands[2].shift.type != ARM64_SFT_INVALID) {
            instruction->setShift(operands[2].shift.value, operands[2].shift.type);
        }
        return instruction;

    } else if (strcmp(this_insn.mnemonic, "cmn") == 0) {
        Add* instruction = new Add(&EmptyRegister::emptyRegister, create_register(operands[0], handle),
                       create_operand(operands[1], handle), true);

        if (operands[1].shift.type != ARM64_SFT_INVALID) {
            instruction->setShift(operands[1].shift.value, operands[1].shift.type);
        }
        return instruction;
    } else if (strcmp(this_insn.mnemonic, "sub") == 0 || strcmp(this_insn.mnemonic, "subs") == 0) {
        return new Sub(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), strcmp(this_insn.mnemonic, "subs") == 0);

    } else if (strcmp(this_insn.mnemonic, "mul") == 0) {
        return new Mul(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "cmp") == 0) {
        return new Sub(&EmptyRegister::emptyRegister, create_register(operands[0], handle),
                       create_operand(operands[1], handle), true);


        // LOGICAL
    } else if (strcmp(this_insn.mnemonic, "lsl") == 0) {
        return new Lsl(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "lsr") == 0) {
        return new Lsr(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if ((strcmp(this_insn.mnemonic, "and") == 0) || strcmp(this_insn.mnemonic, "ands") == 0) {
        return new And(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), strcmp(this_insn.mnemonic, "ands") == 0);

    } else if (strcmp(this_insn.mnemonic, "orr") == 0) {
        return new Orr(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "eor") == 0) {
        return new Eor(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "tst") == 0) {
        return new And(&EmptyRegister::emptyRegister,
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), true);

        // MOV
    } else if (strcmp(this_insn.mnemonic, "mov") == 0 || strcmp(this_insn.mnemonic, "movz") == 0) {
        return new Mov(create_register(operands[0], handle),
                       create_operand(operands[1], handle));

        // BRANCHES
    } else if (strcmp(this_insn.mnemonic, "movk") == 0) {
        Movk* instruction = new Movk(create_register(operands[0], handle),
                       create_operand(operands[1], handle));
        instruction->setShift(operands[1].shift.value, operands[1].shift.type);
        return instruction;

        // BRANCHES
    } else if (strcmp(this_insn.mnemonic, "b") == 0 | strcmp(this_insn.mnemonic, "br") == 0) {
        return new B(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.eq") == 0) {
        return new Beq(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.ne") == 0) {
        return new Bne(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.hs") == 0) {
        return new Bhs(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.lo") == 0) {
        return new Blo(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.hi") == 0) {
        return new Bhi(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.ls") == 0) {
        return new Bls(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.ge") == 0) {
        return new Bge(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.lt") == 0) {
        return new Blt(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.gt") == 0) {
        return new Bgt(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.le") == 0) {
        return new Ble(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.mi") == 0) {
        return new Bmi(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.pl") == 0) {
        return new Bpl(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.vs") == 0) {
        return new Bvs(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.vc") == 0) {
        return new Bvc(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.al") == 0) {
        return new Bal(create_operand(operands[0], handle));

    } else if (strcmp(this_insn.mnemonic, "b.nv") == 0) {
        return new Bnv(create_operand(operands[0], handle));

        // OTHER
    } else if (strcmp(this_insn.mnemonic, "ret") == 0) {
        return new Ret();

    } else {
        throw std::runtime_error("Instruction isn't supported");
    }
}

int main(int argc, char **argv) {

    csh handle;
    cs_insn *insn;
    size_t count;
    cs_regs regs_read, regs_write;
    uint8_t read_count, write_count;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <elf_file>" << std::endl;
        return 1;
    }

    // input includes elf file to use
    Elf_read reader(argv[1]);

//    for (int i = 0; i < reader.length; i++) {
//        std::cout << (int) reader.data[i] << " ";
//    }
    std::cout << "\n------------\n";
    std::cout << "Instructions read from input ELF file:\n";

    disassemble_cs(&handle, &insn, &count, reader); // disassembler
    uint8_t d = 8;
    uint8_t g = pow(2, 8) - 1;
    uint8_t f = d + g;
//    std::cout << ": " << unsigned(f) << "\n";

/*    std::cout << insn[0].mnemonic << "\n";
    int details_result = cs_regs_access(handle, &insn[0], regs_read, &read_count, regs_write, &write_count);
    if (details_result == 0) {
        if (read_count > 0) {
            printf("\n\tRegisters read:");
            for (i = 0; i < read_count; i++) {
                printf(" %s", cs_reg_name(handle, regs_read[i]));
            }
            printf("\n");
        }

        if (write_count > 0) {
            printf("\n\tRegisters modified:");
            for (i = 0; i < write_count; i++) {
                printf(" %s", cs_reg_name(handle, regs_write[i]));
            }
            printf("\n");
        }
    }
    */


/*    std::string myText;
    std::ifstream CodeFile("arm_examples/factorial.txt");
    if (!CodeFile.is_open()) {
        return 1;
    }*/


    Instruction **instructions = new Instruction *[count];

    Register::pc.set(0);
    std::cout << "\n---------------\nRunning instructions\n";
    if(DEBUG) std::cout << "Starting PC: " << Register::pc.get() << "\n";


    for (int i = 0; i < count; i++) {
        cs_regs_access(handle, &insn[i], regs_read, &read_count, regs_write, &write_count);
        std::string mnc = insn[i].mnemonic;
        instructions[i] = create_instruction(insn[i], i, regs_read, &handle);
//        instructions[i]->run();
    }

    int oldPc, totalInstructions = 0;
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();

    while (Register::pc.get() / 4 < count) { // divide by 4 may need to change depending on offset
        totalInstructions++;
        oldPc = Register::pc.get();
        if (DEBUG) std::cout << "PC: " << oldPc << "\t";
        Register::pc.set(oldPc + 4);
        instructions[oldPc / 4]->run();
    }

    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::high_resolution_clock::now();

    if (DEBUG) {
        std::cout << "PC: " << oldPc << " " << Register::pc.get() << "\n";
    }

    long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "\nResult (x0, x1, x2 values): "
              << Register::registers[0].get() << " "
              << Register::registers[1].get() << " "
              << Register::registers[2].get() << std::endl
              << "Time taken " << (double)ns / 1000000.0 << "ms" << std::endl
              << "Total instructions " << totalInstructions << std::endl
              << "Instructions/second " << (totalInstructions / ((double) ns / 1000000000.0)) << std::endl;

    cs_close(&handle);

    return 0;
}