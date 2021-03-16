#ifndef CODE_INSTRUCTION_H
#define CODE_INSTRUCTION_H


#include "Value.h"

class Instruction {
public:
    virtual void run() = 0;

    static bool debug_mode;

    static void debug_mode_set(bool mode);

    bool update_flags;
};

class LikeAdd : public Instruction {
public:
    Register *destination;
    Register *op1;
    Value *op2;

    LikeAdd(Register *d, Register *op1, Value *op2, bool flags = false);
};

class Add : public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run() override;
};

class Sub : public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run() override;

};

class Mul :  public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run() override;
};

class Lsl : public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run() override;
};

class Lsr : public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run() override;
};

class LikeMov : public Instruction {
public:
    Register *destination;
    Value *op1;

    LikeMov(Register *d, Value *op1);
};

class Mov : public LikeMov {
    using LikeMov::LikeMov;
public:
    void run() override;
};

class Ret : public Instruction {
public:
    void run() override;
};

class Branch : public Instruction {
public:
    explicit Branch(Value *destination);

    Value *destination;

    void run() override;
    virtual bool condition();
};

class Beq : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bne : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bhs : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Blo : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bhi : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bls : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bge : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Blt : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bgt : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Ble : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bmi : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bpl : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bvs : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bvc : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bal : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class Bnv : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

class B : public Branch {
    using Branch::Branch;
public:
    bool condition() override;
};

#endif //CODE_INSTRUCTION_H
