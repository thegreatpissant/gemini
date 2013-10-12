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

/*
 *  Gemini system : Handles all items in a gemini system: CPU, MEMORY, BUS, IO
 */
class Gemini_system
{
private:
    CPU cpu;
    std::shared_ptr<Memory> memory;
    std::shared_ptr<Byte_code> byte_code;
    bool power;
    Cache_type cache_type;

public:
    Gemini_system();
    void power_on ();
    void power_off();
    //  send bytecode into the system
    void load_byte_code (std::shared_ptr<Byte_code> b_c );
    //  Set the cache mode
    void set_cache_type( Cache_type ct);
    //  external notification to cycle the clock
    void cycle_clock ();
    //  A would be function pointer to the current ticking component
    void tick ();
    //  Retrive some information about the system, for now it is the CPU registers
    Gemini_system_info get_system_info ();
};

#endif // GEMINI_SYSTEM_H
