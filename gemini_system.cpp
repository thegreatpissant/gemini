/*
 * James A. Feister jfeister@udel.edu, openjaf@gmail.com
 * Project located at www.github.com/thegreatpissant/gemini.git
 * Class: CISC 360 - Computer Architecture
 * Instructor: Seth Morecraft
 * Web: http://www.cis.udel.edu/~morecraf/cisc360/
 *
 * Project 1: Gemini architecture, Implement the following
 * - GUI: Showing registers and instruction
 * - Parsing of program for syntax errors: Alert user of failure
 * - Running of program instructions, non bytecode translation
 * - Detection of memory access errors, Alert user of failure
 * - This is the base of the project.
 */

#include "gemini_system.h"

Gemini_system::Gemini_system()
{
}

void Gemini_system::power_on()
{
    power = true;
    memory = std::shared_ptr<Memory> (new Memory);
    cpu.set_memory( memory );
    cpu.load_byte_code ( byte_code );
    cpu.initialize();
    cpu.tick();
}

void Gemini_system::power_off ()
{
    cpu.stop ();
    power = false;
}

void Gemini_system::load_byte_code(std::shared_ptr<Byte_code> b_c)
{
    byte_code = b_c;
}

void Gemini_system::cycle_clock()
{
    if (power)
        tick ();
}

void Gemini_system::tick ()
{
    cpu.tick ();
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
