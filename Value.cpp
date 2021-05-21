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
    this->type = "Reg";
}

Register Register::registers[31];

Register Register::pc = Register();

Register Register::sp = Register();

//void Register::set_registers() {
//    for (int i = 0; i < 31; i++) {
//        Register::registers[i] = Register();
//        // std::cout << Register::registers[i].get();
//    }
//    // std::cout << "\n";
//}

Const::Const(uint64_t value) {
    this->value = value;
    this->type = "Imm";
}

void Const::set(uint64_t _) {
    throw std::exception("Can't set value of constant");
}

uint64_t Const::get() {
    return this->value;
}

ProcessState ProcessState::z = ProcessState();
ProcessState ProcessState::n = ProcessState();
ProcessState ProcessState::c = ProcessState();
ProcessState ProcessState::v = ProcessState();
//ProcessState ProcessState::ss = ProcessState();
//ProcessState ProcessState::il = ProcessState();
//ProcessState ProcessState::d = ProcessState();
//ProcessState ProcessState::a = ProcessState();
//ProcessState ProcessState::i = ProcessState();
//ProcessState ProcessState::f = ProcessState();
//ProcessState ProcessState::m = ProcessState();

std::map<std::string, ProcessState *> ProcessState::names{{"n", &ProcessState::n},
                                                          {"c", &ProcessState::c},
                                                          {"z", &ProcessState::z},
                                                          {"v", &ProcessState::v}};
//                                                             {"ss", &ProcessState::ss},
//                                                             {"il", &ProcessState::il},
//                                                            {"d", &ProcessState::d},
//                                                             {"a", &ProcessState::a},
//                                                             {"i", &ProcessState::i},
//                                                             {"f", &ProcessState::f},
//                                                             {"m", &ProcessState::m}};

bool ProcessState::get() {
    return this->value;
}


void ProcessState::set_by_name(std::string name, bool value) {
    uint8_t one_bit = 1 & value;
    ProcessState::names[name]->set(one_bit);
}

bool ProcessState::get_by_name(std::string name) {
    return ProcessState::names[name]->get();
}

ProcessState::ProcessState() {
    this->value = 0;
}

void ProcessState::set(bool value) {
    this->value = value;
}

EmptyRegister EmptyRegister::emptyRegister = EmptyRegister();

void EmptyRegister::set(uint64_t value) {
}
