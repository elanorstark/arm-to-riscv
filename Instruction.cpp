#include <iostream>
#include "Instruction.h"
#include "config.h"

// LikeAdd
LikeAdd::LikeAdd(Register *d, Register *op1, Value *op2, bool flags) {
    this->destination = d;
    this->op1 = op1;
    this->op2 = op2;
    this->update_flags = flags;
}

void Add::run() {
    if (DEBUG) {
        std::cout << "Running: add " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }

    uint64_t rhs = this->op2->get();
    if (this->shiftType != 0) {
        if (this->shiftType == ARM64_SFT_LSL) {
            rhs = rhs << this->shiftVal;
        } else if (this->shiftType == ARM64_SFT_LSR) {
            rhs = rhs >> this->shiftVal;
        } else if (this->shiftType == ARM64_SFT_ASR) {
            rhs = ((int64_t) rhs) >> this->shiftVal;
        }
    }

    uint64_t result = this->op1->get() + rhs;

    if (update_flags) {
        ProcessState::reset();
        // flags:
        // zero: set z
        if (result == 0) {
            ProcessState::z.set(1);
            if (DEBUG) std::cout << " z set ";
        }
        // negative:
        // carry: set c
        unsigned int unsigned_sum = unsigned(this->op1->get()) + unsigned(this->op2->get());
        if (DEBUG) std::cout << "!!!!" << (unsigned_sum != unsigned(result)) << "\n";
        if (result < this->op1->get()) {
            ProcessState::c.set(1);
            if (DEBUG) std::cout << " c set ";
        }
        // overflow: set v
        signed int signed_sum = signed(this->op1->get()) + signed(this->op2->get());
        if (signed(result) != signed_sum) {
            ProcessState::z.set(1);
            if (DEBUG) std::cout << " z set ";
        }
    }
    this->destination->set(result);

}

void Add::setShift(unsigned int shiftVal, arm64_shifter shiftType) {
    this->shiftVal = shiftVal;
    this->shiftType = shiftType;
}

void Sub::run() {
    if (DEBUG) {
        std::cout << "Running: sub " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
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
    if (DEBUG) {
        std::cout << "Running: mul " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }

    this->destination->set(this->op1->get() * this->op2->get());
}

// LikeMov
LikeMov::LikeMov(Register *d, Value *op1) {
    this->destination = d;
    this->op1 = op1;
}

void Mov::run() {
    if (DEBUG) {
        std::cout << "Running: mov " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "\n";
    }

    this->destination->set(this->op1->get());
}

void Movk::setShift(unsigned int shiftVal, arm64_shifter shiftType) {
    this->shiftVal = shiftVal;
    this->shiftType = shiftType;
}

void Movk::run() {
    if (DEBUG) {
        std::cout << "Running: movk " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "\n";
    }

    uint64_t value = this->op1->get();
    if (this->shiftType != 0) {
        if (this->shiftType == ARM64_SFT_LSL) {
            value = value << this->shiftVal;
        } else if (this->shiftType == ARM64_SFT_LSR) {
            value = value >> this->shiftVal;
        } else if (this->shiftType == ARM64_SFT_ASR) {
            value = ((int64_t) value) >> this->shiftVal;
        }
    }
    value |= this->destination->get();
    this->destination->set(value);
}

void Ret::run() {
    if (DEBUG) {
        std::cout << "Running: ret" << "\n";
    }
}

void Lsl::run() {
    if (DEBUG) {
        std::cout << "Running: lsl " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }
    this->destination->set((this->op1->get()) << (this->op2->get() % 64));
    // TODO: check behaviour for registers
}

void Lsr::run() {
    if (DEBUG) {
        std::cout << "Running: lsr " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }
    this->destination->set((this->op1->get()) >> (this->op2->get() % 64));
    // TODO: check behaviour for registers
}

void And::run() {
    if (DEBUG) {
        std::cout << "Running: and " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }
    this->destination->set(this->op1->get() & this->op2->get());
}

void Orr::run() {
    if (DEBUG) {
        std::cout << "Running: orr " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }
    this->destination->set(this->op1->get() | this->op2->get());
}

void Eor::run() {
    if (DEBUG) {
        std::cout << "Running: eor " << "dest={" << this->destination->type << ": "
                  << this->destination->get() << "} "
                  << "op1={" << this->op1->type << ": " << this->op1->get() << "} "
                  << "op2={" << this->op2->type << ": " << this->op2->get() << "} "
                  << "\n";
    }
    this->destination->set(this->op1->get() ^ this->op2->get());
}

void Branch::run() {
    if (condition()) {
        if (DEBUG) {
            std::cout << "Branch: changing pc value from " << Register::pc.get() << " to " << destination->get()
                      << "\n";
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
