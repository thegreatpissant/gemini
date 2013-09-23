#include "memory.h"

Memory::Memory()
{
    main_memory.resize(255);
}

Register_value Memory::get_memory(Memory_loc memory_loc)
{
    if ( memory_loc < main_memory.size() && memory_loc >= 0)
        return main_memory[memory_loc];
    return Register_value {};
}

void Memory::set_memory(const Memory_loc memory_loc, Register_value value)
{
    if ( memory_loc < main_memory.size() && memory_loc >= 0)
        main_memory[memory_loc] = value;
}
