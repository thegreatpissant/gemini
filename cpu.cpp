#include "cpu.h"

CPU::CPU()
{
}

void CPU::tick()
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
            value = memory->get_memory(instruction.value);
        }
        else if (instruction.access_type == Gemini_access_type::VALUE)
        {
            value = instruction.value;
        }
        Acc = value;
        break;
    case Gemini_op::STA :
        memory->set_memory(instruction.value, Acc);
        break;
    case Gemini_op::ADD :
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.value);
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
        break;
    case Gemini_op::SUB:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.value);
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
        break;
    case Gemini_op::AND:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.value);
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
        break;
    case Gemini_op::OR:
        if (instruction.access_type == Gemini_access_type::MEMORY)
        {
            value = memory->get_memory(instruction.value);
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
        break;
    case Gemini_op::NOTA:
        Acc = ~Acc;
        if (Acc > 0)
            CC = 1;
        if (Acc == 0)
            CC = 0;
        if (Acc < 0)
            CC = -1;
        break;
    case Gemini_op::BA:
        PC = instruction.value;
        break;
    case Gemini_op::BE:
        if ( CC == 0 )
            PC = instruction.value;
        break;
    case Gemini_op::BL:
        if ( CC < 0 )
            PC = instruction.value;
        break;
    case Gemini_op::BG:
        if ( CC > 0 )
            PC = instruction.value;
        break;
    case Gemini_op::NOP:
        Acc += 0;
        break;
    case Gemini_op::LABEL:
    case Gemini_op::EMPTY:
    case Gemini_op::INVALID:
        break;
    }
    if ( instruction.op != Gemini_op::BA && instruction.op != Gemini_op::BE && instruction.op != Gemini_op::BG && instruction.op != Gemini_op::BL )
        PC++;
}

void CPU::initialize()
{
    PC = instruction_index = 0;
    instruction = (*byte_code)[PC];
}

void CPU::load_byte_code(std::shared_ptr<Byte_code> bc)
{
    byte_code = bc;
}

void CPU::set_memory(std::shared_ptr<Memory> mem)
{
    memory = mem;
}


