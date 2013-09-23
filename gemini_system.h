#ifndef GEMINI_SYSTEM_H
#define GEMINI_SYSTEM_H

#include <memory>

#include "gemini_types.h"
#include "cpu.h"
#include "memory.h"
#include "clock.h"


class Gemini_system
{
private:
    CPU cpu;
    std::shared_ptr<Memory> memory;
    Clock clock;
    std::shared_ptr<Byte_code> byte_code;

public:
    Gemini_system();
    void power_on ();
    void load_byte_code (std::shared_ptr<Byte_code> b_c );
    void cycle_clock ();
    Gemini_system_info get_system_info ();
};

#endif // GEMINI_SYSTEM_H
