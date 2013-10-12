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
 * gemini memory: will handle the memory request of the CPU, in cache and system forms
 */

#include "memory.h"
#include <stdexcept>
#include <random>

Memory::Memory()
{
    main_memory.resize(256);
}

/* Post and Request functions for main memory. When switching to
 * bytecode this unsigned vs signed conversion for comparison will go
 * away.  For now the wraparound should be safe enough to only check
 * if the requested value is greater then the size of memory.
 */
Register_value Memory::get_memory( Memory_loc memory_loc )
{
    if ( memory_loc >= main_memory.size() )
        throw (std::out_of_range("CPU caused a Main Memory access violation"));

    if ( search_cache(memory_loc) )
    {  //  Hit
        this->hits++;
        return get_from_cache ();
    }
    else
    {  // Miss
        this->misses++;
        return get_from_memory (memory_loc);
    }
}

Register_value Memory::get_from_cache (  )
{
    return cache[hit_line].data[hit_offset];
}

Register_value Memory::get_from_memory ( Memory_loc memory_loc )
{
    std::size_t replace_line = 0;
    //  Find cache line to wipe
    if (this->block_size == 1)
    {
        replace_line = 0;
    }
    else
    {
        //  Random replace
        std::mt19937 r_engine;
        r_engine.seed ( memory_loc );
        replace_line = rand() % cache_set_size;
    }

    //  Map the lines chache set offset into the appropriate cache set.
    u_int16_t byte_block = memory_loc / block_size;
    u_int16_t cache_set = byte_block % (cache_size / cache_set_size );
    u_int16_t offset = memory_loc % block_size;
    replace_line = replace_line + cache_set;
    //  Fill cache line with new data
    //  First if its dirty write it to memory
    if ( this->cache[replace_line].dirty )
    {  //  Write modified cache data block to memory
        for (auto i = 0; i < this->block_size; i++)
        {
            this->main_memory[this->cache[replace_line].address + i] = this->cache[replace_line].data[i];
        }
    }
    //  Fill with new data
    this->cache[replace_line].valid = true;
    this->cache[replace_line].dirty = false;
    //  Grab the full block size
    for (auto i = 0; i < block_size; i++)
    {
        this->cache[replace_line].data[i] = this->main_memory[memory_loc - offset + i];
    }
    //  Oh yeah, Return data queried for
    return cache[replace_line].data[offset];
}


bool Memory::search_cache (Memory_loc ml)
{
    //  The byte_block in memory this is in
    u_int16_t byte_block = ml / block_size;
    //  The set in cache this is in
    u_int16_t cache_set  = byte_block % (cache_size/cache_set_size);
    //  Where in cache this set logically begins
    u_int16_t cache_set_begin = cache_set * cache_set_size;
    //  Offset into a block this memory address will reside
    u_int16_t offset = ml % block_size;
    //  Search the cache set
    for ( auto i = 0; i < cache_set_size; i++, cache_set_begin++)
    {
        if ( cache[cache_set_begin].valid && (ml - offset) == cache[cache_set_begin].address )
        {  //  Hit
            hit_line = cache_set_begin;
            hit_offset = offset;
            return true;
        }
    }
    return false;
}

void Memory::set_memory(Memory_loc  memory_loc, Register_value value)
{
    if ( memory_loc  < main_memory.size())
        main_memory[memory_loc] = value;
    else
        throw (std::out_of_range("CPU caused a Main Memory access violation"));

    if ( search_cache(memory_loc) )
    {   //  Hit
        this->hits++;
        write_to_cache ( value );
    }
    else
    {   //  Miss
        this->misses++;
        this->main_memory[memory_loc] = value;
        get_from_memory (memory_loc);
    }
}

void Memory::write_to_cache ( Register_value value )
{
    cache[hit_line].data[hit_offset] = value;
    cache[hit_line].dirty = true;
}
void Memory::set_cache_type(Cache_type ct)
{
    cache_size = 8;
    cache_type = ct;
    switch (ct) {
    case Cache_type::DIRECT_ONEBLOCK:
        this->cache_set_size = 1;
        this->block_size = 1;
        break;
    case Cache_type::DIRECT_FOURBLOCK:
        this->cache_set_size = 1;
        this->block_size = 4;
        break;
    case Cache_type::TWOWAYSET_ONEBLOCK:
        this->cache_set_size = 2;
        this->block_size = 1;
        break;
    case Cache_type::TWOWAYSET_FOURBLOCK:
        this->cache_set_size = 2;
        this->block_size = 4;
        break;
    }
    this->flush_cache();
}

void Memory::tick()
{
    //  Unused for now...
//    assert (false);
}

void Memory::flush_cache()
{
    this->cache.resize( cache_size );
    for (auto &f : cache) {
        f.data.resize(block_size);
    }
    hits = 0;
    misses = 0;
}
