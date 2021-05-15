#include <iostream>
#include "Instruction.h"

// LikeAdd
LikeAdd::LikeAdd(Register *d, Register *op1, Value *op2, bool flags) {
    this->destination = d;
    this->op1 = op1;
    this->op2 = op2;
    this->update_flags = flags;
}

void Add::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: add " << this->destination->type << this->destination->get() << " " << this->op1->type << this->op1->get()
                  << " " << this->op2->type << this->op2->get()
                  << "\n";
    }
    uint64_t result = this->op1->get() + this->op2->get();

    if (update_flags) {
        ProcessState::reset();
        // flags:
        // zero: set z
        if (result == 0) {
            ProcessState::z.set(1);
            std::cout << " z set ";
        }
        // negative:
        // carry: set c
        unsigned int unsigned_sum = unsigned(this->op1->get()) + unsigned(this->op2->get());
        std::cout << "!!!!" << (unsigned_sum != unsigned(result)) << "\n";
        if (result < this->op1->get()) {
            ProcessState::c.set(1);
            std::cout << " c set ";
        }
        // overflow: set v
        signed int signed_sum = signed(this->op1->get()) + signed(this->op2->get());
        if (signed(result) != signed_sum) {
            ProcessState::z.set(1);
            std::cout << " z set ";
        }
    }
    this->destination->set(result);

}

void Sub::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: sub " << this->destination->type << this->destination->get() << " " << this->op1->type << this->op1->get()
                  << " " << this->op2->type << this->op2->get()
                  << "\n";
    }
    uint64_t result = this->op1->get() - this->op2->get();
    if (update_flags) {
        ProcessState::reset();
        // flags:
        // zero: set z
        if (result == 0) {
            ProcessState::z.set(1);
        }
            // negative: set n
        else if (this->op2->get() > this->op1->get()) {
            ProcessState::n.set(1);
        }
            // TODO: conditions to set carry
        else if (this->op1->get() >= this->op2->get()) {
            ProcessState::c.set(1);
        }
        // can overflow be set?
    }
    this->destination->set(result);
}

void Mul::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: mul " << this->destination->type << this->destination->get() << " " << this->op1->type
                  << this->op1->get() << " " << this->op2->type << this->op2->get()
                  << "\n";
    }

    this->destination->set(this->op1->get() * this->op2->get()); // TODO: check
}

// LikeMov
LikeMov::LikeMov(Register *d, Value *op1) {
    this->destination = d;
    this->op1 = op1;
}

void Mov::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: mov " << this->destination->type << this->destination->get() << " " << this->op1->type
                  << this->op1->get() << "\n";
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
        std::cout << "Running: lsl " << this->destination->type << this->destination->get() << " " << this->op1->type
                  << this->op1->get() << " " << this->op2->type << this->op2->get()
                  << " divide by " << (typeid(*op2) == typeid(*op1) ? 64 : 1) << " "
                  << (this->op2->get()) / (typeid(*op2) == typeid(*op1) ? 64 : 1) << "\n";
    }
    this->destination->set((this->op1->get()) << ((this->op2->get()) % (typeid(*op2) == typeid(*op1) ? 64 : 1)));
    // TODO: check behaviour for registers
}

void Lsr::run() {
    if (Instruction::debug_mode) {
        std::cout << "Running: lsr " << this->destination->type << this->destination->get() << " " << this->op1->type
                  << this->op1->get() << " " << this->op2->type << this->op2->get()
                  << " divide by " << (typeid(*op2) == typeid(*op1) ? 64 : 1) << " "
                  << (this->op2->get()) / (typeid(*op2) == typeid(*op1) ? 64 : 1) << "\n";
    }
    this->destination->set((this->op1->get()) >> ((this->op2->get()) % (typeid(op2) == typeid(op1) ? 64 : 1)));
    // TODO: check behaviour for registers
}

void Branch::run() {
    if (condition()) {
        if (Instruction::debug_mode) {
            std::cout << "Branch: changing pc value from " << Register::pc.get() << " to " << destination->get() << "\n";
        }
        Register::pc.set(destination->get());
    }
}

Branch::Branch(Value *destination) {
    this->destination = destination;
}

bool Branch::condition() {
    return true;
}

bool Beq::condition() {
    return ProcessState::z.get() == 1;
}

bool Bne::condition() {
    return ProcessState::z.get() == 0;
}

bool Bhs::condition() {
    return ProcessState::c.get() == 1;
}

bool Blo::condition() {
    return ProcessState::c.get() == 0;
}

bool Bhi::condition() {
    return ProcessState::c.get() == 1 && ProcessState::z.get() == 0;
}

bool Bls::condition() {
    return !(ProcessState::c.get() == 1 && ProcessState::z.get() == 0);
}

bool Bge::condition() {
    return ProcessState::n.get() == ProcessState::v.get();
}

bool Blt::condition() {
    return ProcessState::n.get() != ProcessState::v.get();
}

bool Bgt::condition() {
    return ProcessState::z.get() == 0 && ProcessState::v.get() == ProcessState::n.get();
}

bool Ble::condition() {
    return !(ProcessState::z.get() == 0 && ProcessState::n.get() == ProcessState::v.get());
}

bool Bmi::condition() {
    return ProcessState::n.get() == 1;
}

bool Bpl::condition() {
    return ProcessState::n.get() == 0;
}

bool Bvs::condition() {
    return ProcessState::v.get() == 1;
}

bool Bvc::condition() {
    return ProcessState::v.get() == 0;
}

bool Bal::condition() {
    return true;
}

bool Bnv::condition() {
    return true;
}

bool B::condition() {
    return true;
}
