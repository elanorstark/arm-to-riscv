#include "Instruction.h"
#include "Value.h"

LikeAdd::LikeAdd(Register *d, Value *op1, Value *op2) {
    this->destination = d;
    this->op1 = op1;
    this->op2 = op2;
}

void Add::run() {
    this->destination->set(this->op1->get() + this->op2->get());
}

void Sub::run() {
    this->destination->set(this->op1->get() - this->op2->get());
}