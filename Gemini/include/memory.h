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

#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "gemini_types.h"

/*
 * gemini memory: will handle the memory request of the CPU, in cache and system forms
 */

struct Cache_line {
    bool valid { false };
    bool dirty { false };
    Memory_loc address { 0 };
    std::vector<int16_t> data;
};

using Cache = std::vector <Cache_line>;

class Memory
{
private:
    //  Gemini system main memory
    std::vector <Register_value> main_memory;

    //  Gemini system cache
    Cache_type cache_type;
    int cache_size;
    int cache_set_size;
    int block_size;
    Cache cache;
    std::size_t hit_line;
    std::size_t hit_offset;

    Register_value get_from_cache ( );
    Register_value get_from_memory ( Memory_loc memory_loc );
    void write_to_cache ( Register_value value );
public:
    Memory();
    // Post and Request functions for main memory.
    Register_value get_memory (Memory_loc memory_loc);
    void set_memory(Memory_loc memory_loc, Register_value value);
    void set_cache_type(Cache_type ct);
    void tick();  // Not used yet
    void flush_cache ();
    bool search_cache (Memory_loc ml);
    int hits;
    int misses;
};

#endif // MEMORY_H
