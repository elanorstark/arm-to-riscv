// adapted from http://elfio.sourceforge.net/elfio.pdf

#include "Elf_read.h"
#include <elfio/elfio.hpp>
#include <exception>

Elf_read::Elf_read(std::string filename) {

    // Create elfio reader
    ELFIO::elfio reader;
    // Load ELF data
    if (!reader.load(filename)) {
        std::cout << "Can't find or process ELF file " << filename << std::endl;
        throw std::runtime_error("Can't find or process ELF file");
    }

    ELFIO::Elf_Half sec_num = reader.sections.size();
    for (int i = 0; i < sec_num; ++i) {
        const ELFIO::section *psec = reader.sections[i];
        if (psec->get_name() == ".data") {
            std::cout << "Found .data"
                      << "\t Size: " << psec->get_size()
                      << std::endl;

        }
        if (psec->get_name() == ".text") {
            // Access section's data if .text
            std::cout << "Found .text"
                      << "\t Size: " << psec->get_size() << std::endl;
            this->data = new char[psec->get_size()];
            memcpy(this->data, psec->get_data(), psec->get_size());
//            std::cout << "copied section" << std::endl;
            this->length = psec->get_size();
//            break; // only want first .text
        }
    }

}
