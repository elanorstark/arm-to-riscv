//
// Created by elano on 16/03/2021.
//

#ifndef CODE_ELF_READ_H
#define CODE_ELF_READ_H

#include <string>

class Elf_read {
public:
    Elf_read(std::string);
    char* data;
    uint32_t length;
};


#endif //CODE_ELF_READ_H
