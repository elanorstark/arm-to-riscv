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

#include "Value.h"
#include "Instruction.h"

extern "C" {
    #include "capstone/include/capstone/capstone.h"
}

extern "C" size_t CAPSTONE_API cs_disasm(csh ud, const uint8_t *buffer, size_t size, uint64_t offset, size_t count, cs_insn **insn);

#define CODE "\xfd\x7b\xbe\xa9\xfd\x03\x00\x91\xe0\x1f\x00\xb9\xe0\x1f\x40\xb9\x1f\x04\x00\x71\x6c\x00\x00\x54\x20\x00\x80\x52\x07\x00\x00\x14\xe0\x1f\x40\xb9\x00\x04\x00\x51\xf6\xff\xff\x97\xe1\x03\x00\x2a\xe0\x1f\x40\xb9\x20\x7c\x00\x1b\xfd\x7b\xc2\xa8\xc0\x03\x5f\xd6"

class Line {
public:
    std::string mnc;  // Mnemonic
    std::string lineText; // Text read from file

    Line(std::string lineText, std::string mnc) {
        this->lineText = lineText;
        this->mnc = mnc;
    }
};

// Function from: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}


cs_insn * main2(void)
{
    csh handle;
    cs_insn *insn;
    size_t count, j;
    cs_regs regs_read, regs_write;
    uint8_t read_count, write_count, i;

    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK)
        return NULL;

//    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON); // slower but more detail

    count = cs_disasm(handle, reinterpret_cast<const uint8_t *>(CODE), sizeof(CODE) - 1, 0x1000, 0, &insn);

    // testing - print assembly instructions
    if (count > 0) {
        for (j = 0; j < count; j++) {
            // Print assembly
            printf("%X: %s\t%s\n", 0x1000+j, insn[j].mnemonic, insn[j].op_str);
        }

//        cs_free(insn, count);
    } else
        printf("ERROR: Failed to disassemble given code!\n");

    cs_close(&handle);

    return insn;
}

int main() {

    cs_insn *insn = main2(); // disassembler

    printf("%s\n", insn[0].mnemonic);

    std::string myText;
    std::ifstream CodeFile("arm_examples/factorial.txt");
    if (!CodeFile.is_open()) {
        return 1;
    }

    // create add instruction using objects
    Instruction **instructions = new Instruction *[1];
    instructions[0] = new Add(&Register::registers[0], new Const(2), new Const(3));
    instructions[0]->run();
    std::cout << "result of example add: " << Register::registers[0].get();

    // close the file
    CodeFile.close();
    return 0;
}
