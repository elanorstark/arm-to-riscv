#include <iostream>
#include <elfio/elfio.hpp>

using namespace ELFIO;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: tutorial <elf_file>" << std::endl;
        return 1;
    }
    // Create elfio reader
    elfio reader;
// Load ELF data
    if (!reader.load(argv[1])) {
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return 2;
    }
// Print ELF file properties
    std::cout << "ELF file class : ";
    if (reader.get_class() == ELFCLASS32)
        std::cout << "ELF32" << std::endl;
    else
        std::cout << "ELF64" << std::endl;

    std::cout << "ELF file encoding : ";
    if (reader.get_encoding() == ELFDATA2LSB)
        std::cout << "Little endian" << std::endl;
    else
        std::cout << "Big endian" << std::endl;
    // Print ELF file sections info
    Elf_Half sec_num = reader.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;
    for (int i = 0; i < sec_num; ++i) {
        const section *psec = reader.sections[i];
        std::cout << " [" << i << "/" << sec_num << "] "
                  << "\t" << psec->get_name()
                  << "\t"
                  << psec->get_size()
                  << "\t" << psec->get_address()
                  << std::endl;
        // Access section's data
        const char *p = reader.sections[i]->get_data();
//        if (psec->get_name()==".text") {
//            std::cout << (int)p[0] << " " << (int)p[1] << "\n";
//        }
    }
    // Print ELF file segments info
    Elf_Half seg_num = reader.segments.size();
    std::cout << "Number of segments: " << seg_num << std::endl;
    for (int i = 0; i < seg_num; ++i) {
        const segment *pseg = reader.segments[i];
        std::cout << " [" << i << "] 0x" << std::hex
                  << pseg->get_flags()
                  << "\t0x"
                  << pseg->get_virtual_address()
                  << "\t0x"
                  << pseg->get_file_size()
                  << "\t0x"
                  << pseg->get_memory_size()
                  << std::endl;
        // Access segments's data
        const char *p = reader.segments[i]->get_data();
    }
    return 0;
}