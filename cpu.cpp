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

#include "cpu.h"

CPU::CPU()
{
}

void CPU::tick()
{
    memory->tick ();
    //  cpu clock
    execute_instruction ();
}

void CPU::execute_instruction ()
{
    if (PC >= (*byte_code).size())
    {
        return;
    }
    instruction = (*byte_code)[PC];

    Value value;

    switch (instruction.op) {
    case Gemini_op::LDA :
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.memory);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc = value;
        PC++;
        break;
    case Gemini_op::STA :
        memory->set_memory(instruction.memory, Acc);
        PC++;
        break;
    case Gemini_op::ADD :
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.memory);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc += value;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        PC++;
        break;
    case Gemini_op::SUB:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.memory);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc -= value;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        PC++;
        break;
    case Gemini_op::AND:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.memory);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc &= value;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        PC++;
        break;
    case Gemini_op::OR:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.memory);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc |= value;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        PC++;
        break;
    case Gemini_op::NOTA:
        Acc = ~Acc;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        PC++;
        break;
    case Gemini_op::BA:
        PC = instruction.value;
        break;
    case Gemini_op::BE:
        if ( CC == 0 )
            PC = instruction.value;
        else
            PC++;
        break;
    case Gemini_op::BL:
        if ( CC < 0 )
            PC = instruction.value;
        else
            PC ++;
        break;
    case Gemini_op::BG:
        if ( CC > 0 )
            PC = instruction.value;
        else
            PC ++;
        break;
    case Gemini_op::NOP:
        Acc += 0;
        PC++;
        break;
    case Gemini_op::LABEL:
    case Gemini_op::EMPTY:
    case Gemini_op::INVALID:
        break;
    }
}

void CPU::initialize()
{
    PC = instruction_index = 0;
    instruction = (*byte_code)[PC];
}

void CPU::stop ()
{
    PC = byte_code->size ();
}

void CPU::load_byte_code(std::shared_ptr<Byte_code> bc)
{
    byte_code = bc;
}

void CPU::set_memory(std::shared_ptr<Memory> mem)
{
    memory = mem;
}


