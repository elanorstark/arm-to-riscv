#ifndef CODE_MEMORY_H
#define CODE_MEMORY_H

#define CHUNKLOG 12
#define CHUNKSIZE (1 << CHUNKLOG)

#include <unordered_map>
#include <array>

class Memory {
public:
    static std::unordered_map<uint64_t, std::array<uint64_t, CHUNKSIZE>> memory;
    static uint64_t get_64(uint64_t address);
    static void set_64(uint64_t address, uint64_t value);
};

#endif //CODE_MEMORY_H
