#include "gemini_system.h"

Gemini_system::Gemini_system()
{
}

void Gemini_system::power_on()
{

}


void Gemini_system::load_byte_code(Byte_code byte_code)
{
    cpu.load_byte_code ( byte_code );
}

void Gemini_system::cycle_clock()
{
    clock.tick ();
}
