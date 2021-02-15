#ifndef CODE_INSTRUCTION_H
#define CODE_INSTRUCTION_H


#include "Value.h"

class Instruction {
public:
    virtual void run() = 0;
    static bool debug_mode;
    static void debug_mode_set(bool mode);
};

class LikeAdd: public Instruction {
public:
    Register* destination;
    Register* op1;
    Value* op2;

    LikeAdd(Register* d, Register* op1, Value* op2);
};

class Add: public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run();

};

class Sub: public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run();

};

class Lsl: public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run();
};

class Lsr: public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run();
};

class LikeMov: public Instruction {
public:
    Register* destination;
    Value* op1;

    LikeMov(Register* d, Value* op1);
};

class Mov: public LikeMov {
    using LikeMov::LikeMov;
public:
    void run();
};

class Ret: public Instruction {
public:
    void run();
};

#endif //CODE_INSTRUCTION_H
