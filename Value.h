#ifndef CODE_VALUE_H
#define CODE_VALUE_H

#include <cstdint>
#include <map>

class Value {
public:
    virtual void set(uint64_t) = 0;

    virtual uint64_t get() = 0;
};

class Register : public Value {
public:
    Register();

    uint64_t contents{0};

    void set(uint64_t value) override;

    uint64_t get() override;

//    static void set_registers();

    static Register registers[31];

    static Register pc;

    static Register sp;
};

class EmptyRegister : public Register {
public:
    static EmptyRegister emptyRegister;

    void set(uint64_t value) override;
};

class ProcessState : public Value {
private:
    uint64_t value;
public:
    static std::map<std::string, ProcessState *> names;


    uint64_t get() override;

    void set(uint64_t) override;

    ProcessState();

    static uint64_t get_by_name(std::string name);

    static void set_by_name(std::string name, uint64_t value);

    static ProcessState n; // negative
    static ProcessState z; // zero
    static ProcessState c; // carry/unsigned overflow
    static ProcessState v; // signed overflow

//    // remove below:
//    static ProcessState ss;
//    static ProcessState il;
//    static ProcessState d;
//    static ProcessState a;
//    static ProcessState i;
//    static ProcessState f;
//    static ProcessState m;

    static void reset() {
        n.set(0);
        z.set(0);
        c.set(0);
        v.set(0);
    }

};

class Const : public Value {
private:
    uint64_t value{0};
public:

    explicit Const(uint64_t value);

    void set(uint64_t value) override;

    uint64_t get() override;
};

#endif //CODE_VALUE_H
