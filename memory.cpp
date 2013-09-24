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

#include "memory.h"
#include <stdexcept>

Memory::Memory()
{
    main_memory.resize(256);
}

Register_value Memory::get_memory(Memory_loc memory_loc)
{
    if ( memory_loc < main_memory.size() )
        return main_memory[memory_loc];
    throw (std::out_of_range("CPU caused a Main Memory access violation"));
}

void Memory::set_memory(Memory_loc  memory_loc, Register_value value)
{
    if ( memory_loc  < main_memory.size())
        main_memory[memory_loc] = value;
    else
        throw (std::out_of_range("CPU caused a Main Memory access violation"));
}

void Memory::tick()
{

}
