#ifndef CPU_H
#define CPU_H

#include "gemini_types.h"

class CPU
{
private:
    Byte_code byte_code;
    //  registers

public:
    CPU();
    void load_byte_code ( Byte_code bc );
};

#endif // CPU_H
