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

/*
 *  Gemini system CPU: Performs operations and generates clock signals.
 *
 */

#include "cpu.h"
#include <stdexcept>

CPU::CPU( )
{
    jmp_stack_depth = 0;
}

void CPU::tick( )
{
    //  cpu clock
    execute_instruction( );
}

void CPU::execute_instruction( )
{
    if ( PC >= ( *byte_code ).size( ) )
    {
        return;
    }
    this->IR = ( *byte_code )[PC];

    Value value;

    switch ( static_cast<Gemini_op>( get_op( IR ) ) )
    {
    case Gemini_op::LDA:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc = value;
        PC++;
        break;
    case Gemini_op::STA:
        memory->set_memory( get_value( IR ), Acc );
        PC++;
        break;
    case Gemini_op::ADD:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc += value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::SUB:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc -= value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::MUL:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc *= value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::DIV:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc /= value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::AND:
        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc &= value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::OR:

        if ( get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if ( get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        Acc |= value;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::NOTA:
        Acc = ~Acc;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::JMP:
        if (++jmp_stack_depth > JMP_STACK_MAX_DEPTH)
            throw (std::out_of_range("Stack overflow"));
        jmp_stack.push(PC+1);
        PC = get_value( IR );
        break;
    case Gemini_op::RET:
        if (--jmp_stack_depth < 0)
            throw (std::out_of_range("Stack underflow"));
        PC = jmp_stack.top();
        jmp_stack.pop();
        break;
    case Gemini_op::BA:
        PC = get_value( IR );
        break;
    case Gemini_op::BE:
        if ( CE == 0 )
            PC = get_value( IR );
        else
            PC++;
        break;
    case Gemini_op::BL:
        if ( CC == 1 )
            PC = get_value( IR );
        else
            PC++;
        break;
    case Gemini_op::BG:
        if ( CC == 2 )
            PC = get_value( IR );
        else
            PC++;
        break;
    case Gemini_op::BGE:
        if ( CC == 2 && CE == 1 )
            PC = get_value ( IR );
        else
            PC ++;
        break;
    case Gemini_op::BLE:
        if ( CC == 1 && CE == 1 )
            PC = get_value ( IR );
        else
            PC++;
        break;
    case Gemini_op::BNE:
        if ( CE == 0 )
            PC = get_value ( IR );
        else
            PC++;
        break;

    case Gemini_op::NOP:
        Acc += 0;
        PC++;
        break;
    case Gemini_op::HLT:
        PC = (*byte_code).size ();
        break;
    case Gemini_op::LABEL:
        break;
    case Gemini_op::EMPTY:
        break;
    case Gemini_op::INVALID:
        break;
    }
}

Register_value CPU::get_value( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x0000FFFF;
    return ( (Register_value)tmp );
}

Gemini_op_type CPU::get_op( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0xFF000000;
    tmp >>= 24;
    return ( (Gemini_op_type)tmp );
}

Gemini_access_type CPU::get_access_type( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x00FF0000;
    tmp >>= 16;
    return ( (Gemini_access_type) tmp );
}

void CPU::initialize( )
{
    Zero = 0;
    One = 1;
    PC = instruction_index = 0;
    IR = ( *byte_code )[PC];
}

void CPU::stop( )
{
    PC = byte_code->size( );
}

void CPU::load_byte_code( std::shared_ptr<Byte_code> bc )
{
    byte_code = bc;
}

void CPU::set_memory( std::shared_ptr<Memory> mem )
{
    memory = mem;
}
