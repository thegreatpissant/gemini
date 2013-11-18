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
 * *
 * Project 2: Gemini Enhancements, Implement the following
 * - Gemini Assembler: Create bytecode files from our Gemini assembly files
 * - Support Loading of binary files into the Gemini Simulator
 * - Add >=, <= Condition Jump checks
 * - Add JMP and RET commands for functions
 * - Add Cache implementation for One block Direct and 2 Way Set
 * - Extra credit: SETHI and SETLO instruction to handle 32 bit numbers
 * - Extra credit: Overflow on Multiplication and Divide
 * - Extra credit: JMP and RET instructions to support 25 levels of recursion
 * - Extra credit: Allow cache to support 4 memory block grabing at a time
 */

/*
 *  Gemini system : Handles all items in a gemini system: CPU, MEMORY, BUS, IO
 */

#include "gemini_system.h"
#include "QtCore"

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
    cpu.halt ();
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

bool Gemini_system::done()
{
    return cpu.done();
}

void Gemini_system::setView(gemini *view)
{
    this->gemini_view = view;
    this->cpu.setView( view );
}
