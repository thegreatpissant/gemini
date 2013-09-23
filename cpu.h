#ifndef CPU_H
#define CPU_H

#include "gemini_types.h"
#include "memory.h"
#include <memory>


class CPU
{
private:

    std::shared_ptr<Memory> memory;

public:    //  Code
    std::shared_ptr<Byte_code> byte_code;

    //  registers
    Register_value OP_RESULT;
    Register_value A;
    Register_value B;
    Register_value Acc;
    Register_value Zero;
    Register_value One;
    Register_value PC;
    Register_value MAR;
    Register_value MDR;
    Register_value TEMP;
    std::size_t IR;
    Register_value CC;
    //  instruction
    Register_value instruction_index;
    Gemini_operand instruction;

    CPU();
    void tick();
    void load_byte_code ( std::shared_ptr<Byte_code> bc );
    void set_memory (std::shared_ptr<Memory> m);
    void initialize ();
};

#endif // CPU_H
