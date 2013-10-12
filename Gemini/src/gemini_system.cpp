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
/*
 *  Gemini system : Handles all items in a gemini system: CPU, MEMORY, BUS, IO
 */

#include "gemini_system.h"

Gemini_system::Gemini_system()
{
}

//  Turn it on 
//  TODO: integrate this and its complement into a switch on the front?
void Gemini_system::power_on()
{
    power = true;
    memory = std::shared_ptr<Memory> (new Memory);
    memory->set_cache_type( this->cache_type);
    cpu.set_memory( memory );
    cpu.load_byte_code ( byte_code );
    cpu.initialize();
    cpu.tick();
}

//  Turn it off
void Gemini_system::power_off ()
{
    cpu.stop ();
    power = false;
}

//  Send bytecode into the system
void Gemini_system::load_byte_code(std::shared_ptr<Byte_code> b_c)
{
    byte_code = b_c;
}

void Gemini_system::set_cache_type(Cache_type ct)
{
    this->cache_type= ct;
}

//  External notification to cycle the clock
void Gemini_system::cycle_clock()
{
    if (power)
        tick ();
}

//  A would be function pointer to the current ticking component
void Gemini_system::tick ()
{
    cpu.tick ();
}

//  Retrive some information about the system, for now it is the CPU registers
Gemini_system_info Gemini_system::get_system_info()
{
    //  Populate the gemini system information struct
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
    tmp.CE = cpu.CE;
    tmp.instruction_index = cpu.instruction_index;
    tmp.jmp_stack_depth = cpu.jmp_stack_depth;
    tmp.cache_hits = memory->hits;
    tmp.cache_misses = memory->misses;
    tmp.cache_type = this->cache_type;
    return tmp;
}
