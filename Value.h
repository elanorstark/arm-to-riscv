#ifndef CODE_VALUE_H
#define CODE_VALUE_H

#include <cstdint>

class Value {
public:
    virtual void set(uint64_t) = 0;
    virtual uint64_t get() = 0;
};

class Register: public Value {
public:
    Register();

    uint64_t contents{0};

    void set(uint64_t value) override;

    uint64_t get();

//    static void set_registers();

    static Register registers[31];

    static Register pc;

    static Register sp;
};

class Const: public Value {
public:
    uint64_t value{0};
    Const(uint64_t value);

    void set(uint64_t value);

    uint64_t get();
};

#endif //CODE_VALUE_H
