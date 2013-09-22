#ifndef GEMINI_SYSTEM_H
#define GEMINI_SYSTEM_H

#include "gemini_types.h"
#include "cpu.h"
#include "memory.h"
#include "clock.h"

class Gemini_system
{
private:
    CPU cpu;
    Memory memory;
    Clock clock;
public:
    Gemini_system();
    void power_on ();
    void load_byte_code ( Byte_code byte_code );
    void cycle_clock ();
};

#endif // GEMINI_SYSTEM_H
