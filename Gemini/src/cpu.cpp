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

    //  Temps to hold ...
    Value value;
    Instruction_register i32;

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
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        //  Overflow detection
        mull = Acc;
        mull *= value;
        if ( ((mull & 0x7FFF0000) >> 16) != 0)
            OVF = 1;
        else OVF = 0;
        Acc = (mull & 0xFFFF0000) >> 16;
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        //  Overflow detection
        divl = Acc;
        divl /= value;
        if ( ((divl & 0x7FFF0000) >> 16) != 0)
            OVF = 1;
        else OVF = 0;
        Acc = (mull & 0xFFFF0000) >> 16;
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
        if ( Acc == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::NOTA:
        Acc = ~Acc;
        //  Set less then or equal
        if ( Acc > 0 )
            CC = 2;
        else if ( Acc < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
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
        if ( CE == 1 )
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
    case Gemini_op::SETHI:
        if (get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if (get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        //  0 or 1 specifies what register we are going to be using
        i32 = 0;
        i32 |= this->Acc;
        i32 <<= 16;
        i32 |= 0x0000FFFF;
        if (value == 0)
            this->SL0 &= i32;
        else if ( value == 1)
            this->SL1 &= i32;
        else
            throw (std::out_of_range("CPU SETHI register access violation"));
        PC++;
        break;
    case Gemini_op::SETLO:
        if (get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if (get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        i32 = 0xFFFF0000;
        i32 |= this->Acc;
        if (value == 0)
            this->SL0 &= i32;
        else if ( value == 1)
            this->SL1 &= i32;
        else
            throw (std::out_of_range("CPU SETHLO register access violation"));
        PC++;
        break;
    case Gemini_op::LDHI:
        //  Load High part of either SL0 or SL1 into the accumilator
        if (get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if (get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        i32 = 0;
        if (value == 0)
            i32 = SL0;
        else if ( value == 1)
            i32 = SL1;
        else
            throw (std::out_of_range("CPU SETHLO register access violation"));
        i32 >>= 16;
        i32 &= 0x0000FFFF;
        Acc = 0;
        Acc |= i32;
        PC++;
        break;
    case Gemini_op::LDLO:
        //  Load the low part of either SL0 or SL1 into the accumilator
        //  Load High part of either SL0 or SL1 into the accumilator
        if (get_access_type( IR ) == Gemini_access_type::MEMORY )
        {
            value = memory->get_memory( get_value( IR ) );
        }
        else if (get_access_type( IR ) == Gemini_access_type::VALUE )
        {
            value = get_value( IR );
        }
        i32 = 0;
        if (value == 0)
            i32  = SL0;
        else if ( value == 1)
            i32 = SL1;
        else
            throw (std::out_of_range("CPU SETHLO register access violation"));
        i32 &= 0x0000FFFF;
        Acc = 0;
        Acc |= i32;
        PC++;
        break;
    case Gemini_op::ADDSL:
        //  Add SL0 and SL1 and put the result in SL0
        SL0 += SL1;
        //  Set less then or equal
        if ( SL0 > 0 )
            CC = 2;
        else if ( SL0 < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
        if (  SL0 == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::SUBSL:
        //  Sub SL0 and SL1 and put the result in SL0
        SL0 -= SL1;
        //  Set less then or equal
        if ( SL0 > 0 )
            CC = 2;
        else if ( SL0 < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
        if (  SL0 == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::MULSL:
        //  Multiply SL0 and SL1 and put the result in SL0
        SL0 *= SL1;
        //  Set less then or equal
        if ( SL0 > 0 )
            CC = 2;
        else if ( SL0 < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
        if (  SL0 == 0 )
            CE = 1;
        else
            CE = 0;
        PC++;
        break;
    case Gemini_op::DIVSL:
        //  Divide SL0 and SL1 and put the result in SL0
        SL0 /= SL1;
        //  Set less then or equal
        if ( SL0 > 0 )
            CC = 2;
        else if ( SL0 < 0 )
            CC = 1;
        else
            CC = 0;
        //  Set equal
        if (  SL0 == 0 )
            CE = 1;
        else
            CE = 0;
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

void CPU::set_cache_mode(Cache_type cache_type)
{
    memory->set_cache_type( cache_type );
}
