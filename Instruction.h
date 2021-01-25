#ifndef CODE_INSTRUCTION_H
#define CODE_INSTRUCTION_H


#include "Value.h"

class Instruction {
public:
    virtual void run() = 0;
};

class LikeAdd: public Instruction {
public:
    Register* destination;
    Value* op1;
    Value* op2;

    LikeAdd(Register* d, Value* op1, Value* op2);
};

class Add: public LikeAdd {
    using LikeAdd::LikeAdd;
public:
    void run();

};


#endif //CODE_INSTRUCTION_H
