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

#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "gemini_types.h"

class Memory
{
private:
    std::vector <Register_value> main_memory;

public:
    Memory();
    Register_value get_memory (Memory_loc memory_loc);
    void set_memory(Memory_loc memory_loc, Register_value value);
    void tick();
};

#endif // MEMORY_H
