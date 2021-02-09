/* test1.c */
//http://www.capstone-engine.org/lang_c.html

#include <stdio.h>
#include <inttypes.h>

#include "capstone/include/capstone/capstone.h"

#define CODE "\x40\x00\x01\x4b"
//"\x20\x00\x80\x52\x41\x00\x80\x52\x20\x00\x00\x0B\xC0\x03\x5F\xD6"



//int main(void)
//{
//    csh handle;
//    cs_insn *insn;
//    size_t count, j;
//    cs_regs regs_read, regs_write;
//    uint8_t read_count, write_count, i;
//
//    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK)
//        return -1;
//    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
//    count = cs_disasm(handle, CODE, sizeof(CODE)-1, 0x1000, 0, &insn);
//    if (count > 0) {
//        size_t j;
//        for (j = 0; j < count; j++) {
//            //printf("%s\n",insn[j].op_str);
//        }
//
//        if (cs_regs_access(handle, &insn[j],
//                           regs_read, &read_count,
//                           regs_write, &write_count) == 0) {
//            if (read_count > 0) {
//                printf("\n\tRegisters read:");
//                for (i = 0; i < read_count; i++) {
//                    printf(" %s", cs_reg_name(handle, regs_read[i]));
//                }
//                printf("\n");
//            }
//
//            if (write_count > 0) {
//                printf("\n\tRegisters modified:");
//                for (i = 0; i < write_count; i++) {
//                    printf(" %s", cs_reg_name(handle, regs_write[i]));
//                }
//                printf("\n");
//            }
//        }
//
//
//
//    cs_free(insn, count);
//    } else {
//        printf("ERROR: Failed to disassemble given code!\n");
//    }
//
//    cs_close(&handle);
//
//    return 0;
//}

int main(void)
{
    csh handle;
    cs_insn *insn;
    size_t count, j;
    cs_regs regs_read, regs_write;
    uint8_t read_count, write_count, i;

    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK)
        return -1;

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    count = cs_disasm(handle, CODE, sizeof(CODE)-1, 0x1000, 0, &insn);
    if (count > 0) {
        for (j = 0; j < count; j++) {
            // Print assembly
            printf("%s\t%s\n", insn[j].mnemonic, insn[j].op_str);

            // Print all registers accessed by this instruction.
            if (cs_regs_access(handle, &insn[j], regs_read, &read_count, regs_write, &write_count) == 0) {
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
        }

        cs_free(insn, count);
    } else
        printf("ERROR: Failed to disassemble given code!\n");

    cs_close(&handle);

    return 0;
}