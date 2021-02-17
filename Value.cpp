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

Register pc = Register();

Register sp = Register();

//void Register::set_registers() {
//    for (int i = 0; i < 31; i++) {
//        Register::registers[i] = Register();
//        // std::cout << Register::registers[i].get();
//    }
//    // std::cout << "\n";
//}

Const::Const(uint64_t value) {
    this->value = value;
}

void Const::set(uint64_t _) {
    throw std::exception("Can't set value of constant");
}

uint64_t Const::get() {
    return this->value;
}

std::map<std::string, SPSR_Register*> SPSR_Register::names { {"n", &SPSR_Register::n},
                                                             {"c", &SPSR_Register::c},
                                                             {"z", &SPSR_Register::z},
                                                             {"v", &SPSR_Register::v},
                                                             {"ss", &SPSR_Register::ss},
                                                             {"il", &SPSR_Register::il},
                                                             {"d", &SPSR_Register::d},
                                                             {"a", &SPSR_Register::a},
                                                             {"i", &SPSR_Register::i},
                                                             {"f", &SPSR_Register::f},
                                                             {"m", &SPSR_Register::m}};

uint64_t SPSR_Register::get() {
    return this->value;
}


void SPSR_Register::set_by_name(std::string name, uint64_t value) {
    uint8_t one_bit = 1 & value;
    SPSR_Register::names[name]->set(one_bit);
}

uint64_t SPSR_Register::get_by_name(std::string name){
    return SPSR_Register::names[name]->get();
}

SPSR_Register::SPSR_Register() {
    this->value = 0;
}

void SPSR_Register::set(uint64_t value) {
    this->value = value;
}
