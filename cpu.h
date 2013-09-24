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


class CPU
{
private:
    std::shared_ptr<Memory> memory;

    void execute_instruction();

public:
    std::shared_ptr<Byte_code> byte_code;

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
//     Gemini_byte_code IR;
    Register_value CC;
    //  instruction
    Register_value instruction_index;
    Gemini_operand instruction;

    CPU();
    void tick();
    void load_byte_code ( std::shared_ptr<Byte_code> bc );
    void set_memory (std::shared_ptr<Memory> m);
    void initialize ();
    void stop();
};

#endif // CPU_H
