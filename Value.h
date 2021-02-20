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

class Process_State: public Value {
public:
    static std::map<std::string, Process_State*> names;

    uint64_t value;

    uint64_t get();

    void set(uint64_t);

    Process_State();

    static uint64_t get_by_name(std::string name);

    static void set_by_name(std::string name, uint64_t value);

    static Process_State n;
    static Process_State z;
    static Process_State c;
    static Process_State v;
    static Process_State ss;
    static Process_State il;
    static Process_State d;
    static Process_State a;
    static Process_State i;
    static Process_State f;
    static Process_State m;

};

class Const: public Value {
public:
    uint64_t value{0};
    Const(uint64_t value);

    void set(uint64_t value);

    uint64_t get();
};

#endif //CODE_VALUE_H
