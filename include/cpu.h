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
#include <stack>


/*
 *  Gemini system CPU: Performs operations and generates clock signals.
 *
 */
class CPU
{
private:
    void execute_instruction();  //  Should end up as a function pointer that we use if powered on
    Register_value get_value(Instruction_register ir);
    Gemini_op_type get_op(Instruction_register ir);
    Gemini_access_type get_access_type(Instruction_register ir);

public:
    CPU();

    //  Memory from the gemini_system
    std::shared_ptr<Memory> memory;

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
    Register_value PC;
    Register_value MAR;
    Register_value MDR;
    Register_value TEMP;

    Instruction_register IR;

    Register_value CC;
    Register_value CE;

    std::stack <Register_value> jmp_stack;
    const int JMP_STACK_MAX_DEPTH = 25;
    int jmp_stack_depth;

    Register_value instruction_index;  //  not sure what the instruction_index was for


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
