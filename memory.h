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
    void set_memory(const Memory_loc memory_loc, Register_value value);
};

#endif // MEMORY_H
