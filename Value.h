#ifndef CODE_VALUE_H
#define CODE_VALUE_H

#include <cstdint>
#include <map>

class Value {
public:
    virtual void set(uint64_t) = 0;
    virtual uint64_t get() = 0;
};

class Register: public Value {
public:
    Register();

    uint64_t contents{0};

    void set(uint64_t value);

    uint64_t get();

//    static void set_registers();

    static Register registers[31];

    static Register pc;

    static Register sp;
};

class SPSR_Register: public Value {
public:
    static std::map<std::string, SPSR_Register*> names;

    uint64_t value;

    uint64_t get();

    void set(uint64_t);

    SPSR_Register();

    static uint64_t get_by_name(std::string name);

    static void set_by_name(std::string name, uint64_t value);

    static SPSR_Register n;
    static SPSR_Register z;
    static SPSR_Register c;
    static SPSR_Register v;
    static SPSR_Register ss;
    static SPSR_Register il;
    static SPSR_Register d;
    static SPSR_Register a;
    static SPSR_Register i;
    static SPSR_Register f;
    static SPSR_Register m;

};

class Const: public Value {
public:
    uint64_t value{0};
    Const(uint64_t value);

    void set(uint64_t value);

    uint64_t get();
};

#endif //CODE_VALUE_H
