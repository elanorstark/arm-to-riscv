#include <iostream>
#include "Instruction.h"

// LikeAdd
LikeAdd::LikeAdd(Register *d, Register *op1, Value *op2) {
    this->destination = d;
    this->op1 = op1;
    this->op2 = op2;
}

void Add::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: add " << this->destination->get() << " " << this->op1->get() << " " << this->op2->get()
                  << "\n";
    }
    this->destination->set(this->op1->get() + this->op2->get());
}

void Sub::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: sub " << this->destination->get() << " " << this->op1->get() << " " << this->op2->get()
                  << "\n";
    }
    this->destination->set(this->op1->get() - this->op2->get());
}

// LikeMov
LikeMov::LikeMov(Register *d, Value *op1) {
    this->destination = d;
    this->op1 = op1;
}

void Mov::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: mov " << this->destination->get() << " " << this->op1->get() << "\n";
    }
    this->destination->set(this->op1->get());
}

void Ret::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: ret" << "\n";
    }
}

bool Instruction::debug_mode = false;

void Instruction::debug_mode_set(bool mode) {
    Instruction::debug_mode = mode;
}

void Lsl::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: lsl " << this->destination->get() << " " << this->op1->get() << " " << this->op2->get()
                  << " divide by " << (typeid(*op2) == typeid(*op1) ? 64 : 1) << " "
                  << (this->op2->get()) / (typeid(*op2) == typeid(*op1) ? 64 : 1) << "\n";
    }
    this->destination->set((this->op1->get()) << ((this->op2->get()) % (typeid(*op2) == typeid(*op1) ? 64 : 1)));
    // TODO: check behaviour for registers
}

void Lsr::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: lsr " << this->destination->get() << " " << this->op1->get() << " " << this->op2->get()
                  << " divide by " << (typeid(*op2) == typeid(*op1) ? 64 : 1) << " "
                  << (this->op2->get()) / (typeid(*op2) == typeid(*op1) ? 64 : 1) << "\n";
    }
    this->destination->set((this->op1->get()) >> ((this->op2->get()) % (typeid(op2) == typeid(op1) ? 64 : 1)));
    // TODO: check behaviour for registers
}