#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include <cstdio>
#include <cinttypes>
#include <cmath>

#include "Value.h"
#include "Instruction.h"

extern "C" {
#include "capstone/include/capstone/capstone.h"
}

// extern "C" size_t CAPSTONE_API cs_disasm(csh ud, const uint8_t *buffer, size_t size, uint64_t offset, size_t count, cs_insn **insn);

//#define CODE "\x20\x00\x80\x52\x41\x00\x80\x52\x20\x00\x00\x0b\xc0\x03\x5f\xd6"
//#define CODE "\x20\x00\x80\x52\x41\x00\x80\x52\x21\x00\x00\x0b\x00\x20\xc1\x1a\xc0\x03\x5f\xd6"
#define CODE "\xe0\xff\x9f\x52\xe1\xff\x9f\x52\x00\x00\x01\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x00\x00\x2b\x00\x04\x00\x31"

class Line {
public:
    std::string mnc;  // Mnemonic
    std::string lineText; // Text read from file

    Line(std::string lineText, std::string mnc) {
        this->lineText = lineText;
        this->mnc = mnc;
    }
};

int disassemble_cs(csh *handle, cs_insn **insn,
                   size_t *count) { //, cs_regs *regs_red, cs_regs *regs_write, uint8_t *red_count, uint8_t *write_count) {

    size_t j;

    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, handle) != CS_ERR_OK)
        return -1;

    cs_option(*handle, CS_OPT_DETAIL, CS_OPT_ON); // slower but more detail

    *count = cs_disasm(*handle, reinterpret_cast<const uint8_t *>(CODE), sizeof(CODE) - 1, 0x1000, 0, insn);


    // testing - print assembly instructions
    if (*count > 0) {
        for (j = 0; j < *count; j++) {
            // Print assembly
//            printf("%X:\t%s\t%s\n", 0x1000+j, (*insn)[j].mnemonic, (*insn)[j].op_str);
        }

    } else
        printf("ERROR: Failed to disassemble given code!\n");

    return 0;
}

Register *create_register(cs_arm64_op &operand, csh *handle) {
    if (operand.type == ARM64_OP_REG) {
        std::string reg_name = cs_reg_name(*handle, operand.reg);
        int reg_num = std::stoi(reg_name.substr(1, -1));
        return &Register::registers[reg_num];
    } else {
        throw std::exception("Operand isn't register");
    }

}

Value *create_operand(cs_arm64_op &operand, csh *handle) {
    if (operand.type == ARM64_OP_REG) {
        return create_register(operand, handle);
    } else if (operand.type == ARM64_OP_IMM) {
        return new Const(operand.imm);
    }
}

Instruction *create_instruction(cs_insn &this_insn, int line, cs_regs &regs_read, csh *handle) {
    cs_detail *detail = this_insn.detail;
    int op_count = detail->arm64.op_count;
    cs_arm64_op *operands = detail->arm64.operands;

    std::cout << this_insn.mnemonic << "\n";
    if (strcmp(this_insn.mnemonic, "add") == 0 || strcmp(this_insn.mnemonic, "adds") == 0) {
        return new Add(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), strcmp(this_insn.mnemonic, "adds") == 0);

    } else if (strcmp(this_insn.mnemonic, "sub") == 0 || strcmp(this_insn.mnemonic, "subs") == 0) {
        return new Sub(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle), strcmp(this_insn.mnemonic, "subs") == 0);

    } else if (strcmp(this_insn.mnemonic, "cmp") == 0) {
        return new Sub(&EmptyRegister::emptyRegister, create_register(operands[0], handle),
                       create_operand(operands[1], handle), true);

    } else if (strcmp(this_insn.mnemonic, "lsl") == 0) {
        return new Lsl(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "lsr") == 0) {
        return new Lsr(create_register(operands[0], handle),
                       create_register(operands[1], handle),
                       create_operand(operands[2], handle));

    } else if (strcmp(this_insn.mnemonic, "movz") == 0) {
        return new Mov(create_register(operands[0], handle),
                       create_operand(operands[1], handle));

    } else {
        return new Ret(); // currently using for any instructions that haven't been added
    }
}

int main() {

    csh handle;
    cs_insn *insn;
    size_t count;
    cs_regs regs_read, regs_write;
    uint8_t read_count, write_count;

    disassemble_cs(&handle, &insn, &count); // disassembler
    uint8_t d = 8;
    uint8_t g = pow(2, 8) - 1;
    uint8_t f = d + g;
    std::cout << ": " << unsigned(f) << "\n";

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

    Instruction::debug_mode_set(true);

    Register::pc.set(0);
    std::cout << "PC" << Register::pc.get() << "\n";


    for (int i = 0; i < count; i++) {
        cs_regs_access(handle, &insn[i], regs_read, &read_count, regs_write, &write_count);
        std::string mnc = insn[i].mnemonic;
        instructions[i] = create_instruction(insn[i], i, regs_read, &handle);
//        instructions[i]->run();
    }

    int oldPc;

    while (Register::pc.get() / 4 < count - 1) {
        oldPc = Register::pc.get();
        std::cout << oldPc;
        Register::pc.set(oldPc + 4);
        instructions[oldPc / 4]->run();
    }

    std::cout << "\nresult (w0, w1, w2 value): " << Register::registers[0].get() << " " << Register::registers[1].get()
              << " " << Register::registers[2].get();

/*    // close the file
    CodeFile.close();*/

    cs_close(&handle);

    return 0;
}