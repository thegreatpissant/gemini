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

#ifndef GEMINI_SYSTEM_H
#define GEMINI_SYSTEM_H

#include <memory>

#include "gemini_types.h"
#include "cpu.h"
#include "memory.h"

class Gemini_system
{
private:
    CPU cpu;
    std::shared_ptr<Memory> memory;
    std::shared_ptr<Byte_code> byte_code;
    bool power;

public:
    Gemini_system();
    void power_on ();
    void power_off();
    void load_byte_code (std::shared_ptr<Byte_code> b_c );
    void cycle_clock ();
    void tick ();
    Gemini_system_info get_system_info ();
};

#endif // GEMINI_SYSTEM_H
