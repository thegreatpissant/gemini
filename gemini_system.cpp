#include "gemini_system.h"

Gemini_system::Gemini_system()
{
}

void Gemini_system::power_on()
{
    memory = std::shared_ptr<Memory> (new Memory);
    cpu.set_memory( memory );
    cpu.load_byte_code ( byte_code );
    cpu.initialize();
    cpu.tick();
}


void Gemini_system::load_byte_code(std::shared_ptr<Byte_code> b_c)
{
    byte_code = b_c;
}

void Gemini_system::cycle_clock()
{
    clock.tick ();
    cpu.tick();
}

Gemini_system_info Gemini_system::get_system_info()
{
    Gemini_system_info tmp;
    tmp.A = cpu.A;
    tmp.B = cpu.B;
    tmp.Acc = cpu.Acc;
    tmp.Zero = cpu.Zero;
    tmp.One = cpu.One;
    tmp.PC = cpu.PC;
    tmp.MAR = cpu.MAR;
    tmp.MDR = cpu.MDR;
    tmp.TEMP = cpu.TEMP;
    tmp.IR = cpu.IR;
    tmp.CC = cpu.CC;
    tmp.instruction_index = cpu.instruction_index;
    tmp.instruction = cpu.instruction;

    return tmp;
}
