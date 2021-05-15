#include "Memory.h"


std::unordered_map<uint64_t, std::array<uint64_t, CHUNKSIZE>> Memory::memory;

uint64_t Memory::get_64(uint64_t address) {
    uint64_t chunk_address = address & ~(CHUNKSIZE-1);
    if (Memory::memory.count(chunk_address)) {
        uint64_t chunk_offset = address - chunk_address;
        uint64_t value = Memory::memory[chunk_address][chunk_offset];
        return value;
    }
    return 0; // TODO: choose what to do if don't have this memory yet
}

void Memory::set_64(uint64_t address, uint64_t value) {
    uint64_t chunk_address = address & ~(CHUNKSIZE-1);
    uint64_t chunk_offset = address - chunk_address;
    Memory::memory[chunk_address][chunk_offset] = value;
}

