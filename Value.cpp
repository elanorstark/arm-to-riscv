#include <exception>
#include "Value.h"

void Register::set(uint64_t value) {
    this->contents = value;
}

uint64_t Register::get() {
    return this->contents;
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