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

Process_State Process_State::z = Process_State();
Process_State Process_State::n = Process_State();
Process_State Process_State::c = Process_State();
Process_State Process_State::v = Process_State();
Process_State Process_State::ss = Process_State();
Process_State Process_State::il = Process_State();
Process_State Process_State::d = Process_State();
Process_State Process_State::a = Process_State();
Process_State Process_State::i = Process_State();
Process_State Process_State::f = Process_State();
Process_State Process_State::m = Process_State();

std::map<std::string, Process_State*> Process_State::names { {"n", &Process_State::n},
                                                             {"c", &Process_State::c},
                                                             {"z", &Process_State::z},
                                                             {"v", &Process_State::v},
                                                             {"ss", &Process_State::ss},
                                                             {"il", &Process_State::il},
                                                            {"d", &Process_State::d},
                                                             {"a", &Process_State::a},
                                                             {"i", &Process_State::i},
                                                             {"f", &Process_State::f},
                                                             {"m", &Process_State::m}};

uint64_t Process_State::get() {
    return this->value;
}


void Process_State::set_by_name(std::string name, uint64_t value) {
    uint8_t one_bit = 1 & value;
    Process_State::names[name]->set(one_bit);
}

uint64_t Process_State::get_by_name(std::string name){
    return Process_State::names[name]->get();
}

Process_State::Process_State() {
    this->value = 0;
}

void Process_State::set(uint64_t value) {
    this->value = value;
}
