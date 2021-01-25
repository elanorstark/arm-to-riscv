#include <exception>
#include <iostream>
#include "Value.h"

void Register::set(uint64_t value) {
    this->contents = value;
}

uint64_t Register::get() {
    return this->contents;
}

Register::Register() {
    this->contents = 0;
}

Register Register::registers[31];

void Register::set_registers() {
    for (int i = 0; i < 31; i++) {
        Register::registers[i] = Register();
        // std::cout << Register::registers[i].get();
    }
    // std::cout << "\n";
}

Const::Const(uint64_t value) {
    this->value = value;
}

void Const::set(uint64_t _) {
    throw std::exception("Can't set value of constant");
}

uint64_t Const::get() {
    return this->value;
}