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

#ifndef CPU_H
#define CPU_H

#include "gemini_types.h"
#include "memory.h"

#include <memory>


/*
 *  Gemini system CPU: Performs operations and generates clock signals.
 *
 */
class CPU
{
private:
    void execute_instruction();  //  Should end up as a function pointer that we use if powered on

public:
    CPU();

    //  All gemini application bytecode stored here.
    std::shared_ptr<Byte_code> byte_code;

    //  onboard memory, "Registers" Did not see any problem with them
    //  being public.
    Register_value OP_RESULT;
    Register_value A;
    Register_value B;
    Register_value Acc;
    Register_value Zero;
    Register_value One;
    std::size_t PC;
    Register_value MAR;
    Register_value MDR;
    Register_value TEMP;
    Gemini_operand IR;
/*  Change to in later version
 *     Gemini_byte_code IR; */
    Register_value CC;

    Register_value instruction_index;  //  not sure what the instruction_index was for
    Gemini_operand instruction;

    //  External action to initiate a clock tick
    void tick();

    //  Bring in bytecode to the CPU
    void load_byte_code ( std::shared_ptr<Byte_code> bc );
    
    //  Set internal handle to main memory section
    void set_memory (std::shared_ptr<Memory> m);

    //  bring CPU to starting state
    void initialize ();

    //  Stop the cpu from running
    void stop();
};

#endif // CPU_H
