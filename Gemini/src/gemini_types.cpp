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
 * Gemini types: Various types used in the gemini application
 */
#include "gemini_types.h"

#include <iomanip>
#include <sstream>

//  An include default translation of a Gemini_operand to a std::string
std::string gemini_instruction_register_to_std_string ( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0xFF000000;
    tmp >>= 24;
    Gemini_op gop = (Gemini_op)tmp;
    tmp = ir;
    tmp &= 0x00FF0000;
    tmp >>=16;
    Gemini_access_type gopt = (Gemini_access_type) tmp;
    tmp = ir;
    tmp &= 0x0000FFFF;
    Value v = (Value)tmp;

    std::string operand_string =
            gemini_op_to_std_string ( gop ) + " " +
            gemini_access_type_to_std_string( gopt ) + " " +
            gemini_value_to_std_string( v );

    return operand_string;
}

std::string gemini_instruction_register_value_to_std_string(Instruction_register ir)
{
    std::stringstream s;
    s <<
         "0x" <<
         std::setfill ('0') << std::setw(sizeof(Instruction_register) * 2) <<
         std::hex <<
         static_cast <Instruction_register> (ir);
    return s.str();
}

std::string gemini_register_value_to_std_string (Register_value rv)
{
    std::stringstream s;
    s <<
         "0x" <<
         std::setfill ('0') << std::setw(sizeof (Register_value)*2) <<
         std::hex <<
         static_cast<Register_value>(rv);
    return s.str();
}

std::string gemini_value_to_std_string (Value value)
{
    std::stringstream s;
    s <<
         "0x" <<
         std::setfill ('0') << std::setw(sizeof (Value)*2) <<
         std::hex <<
         static_cast<Value>(value);
    return s.str();
}

//  An include default translation of a Gemini_access_type to a std::string
std::string gemini_access_type_to_std_string ( Gemini_access_type gemini_access_type )
{
    switch (static_cast<Gemini_access_type>(gemini_access_type))
    {
    case Gemini_access_type::MEMORY :
        return std::string {"M"};
    case Gemini_access_type::VALUE :
        return std::string {"I"};
    case Gemini_access_type::NONE :
        return std::string {"_"};
    }
    return std::string {"_"};
}


//  An include default translation of a Gemini_op to a std::string
std::string gemini_op_to_std_string ( Gemini_op gemini_op )
{
    switch (static_cast<Gemini_op>(gemini_op))
    {
    case Gemini_op::LABEL :
        return std::string {"LABEL"};
    case Gemini_op::LDA :
        return std::string {"LDA"};
    case Gemini_op::STA :
        return std::string {"STA"};
    case Gemini_op::ADD :
        return std::string {"ADD"};
    case Gemini_op::SUB:
        return std::string {"SUB"};
    case Gemini_op::MUL:
       return std::string {"MUL"};
    case Gemini_op::DIV:
       return std::string {"DIV"};
    case Gemini_op::AND:
        return std::string {"AND"};
    case Gemini_op::OR:
        return std::string {"OR"};
    case Gemini_op::NOTA:
        return std::string {"NOTA"};
    case Gemini_op::BA:
        return std::string {"BA"};
    case Gemini_op::BE:
        return std::string {"BE"};
    case Gemini_op::BL:
        return std::string {"BL"};
    case Gemini_op::BG:
        return std::string {"BG"};
    case Gemini_op::BGE:
        return std::string {"BGE"};
    case Gemini_op::BLE:
      return std::string {"BLE"};
    case Gemini_op::BNE:
        return std::string {"BNE"};
    case Gemini_op::JMP:
       return std::string {"JMP"};
    case Gemini_op::RET:
        return std::string {"RET"};
    case Gemini_op::HLT:
        return std::string {"HLT"};
    case Gemini_op::NOP:
        return std::string {"NOP"};
    case Gemini_op::INVALID:
        return std::string {"INVALID"};
    case Gemini_op::EMPTY:
        return std::string {"EMPTY"};
    case Gemini_op::SETHI:
        return std::string {"SETHI"};
    case Gemini_op::SETLO:
        return std::string {"SETLO"};
    case Gemini_op::LDHI:
        return std::string {"LDHI"};
    case Gemini_op::LDLO:
        return std::string {"LDLO"};
    case Gemini_op::ADDSL:
        return std::string {"ADDSL"};
    case Gemini_op::SUBSL:
        return std::string {"SUBSL"};
    case Gemini_op::MULSL:
        return std::string {"MULSL"};
    case Gemini_op::DIVSL:
        return std::string {"DIVSL"};
}
    return std::string {"INVALID"};
}



std::string gemini_cache_type_to_std_string(Cache_type cache_type)
{
    switch (cache_type) {
    case Cache_type::DIRECT_FOURBLOCK:
        return std::string { "Direct Four Block" };
    case Cache_type::DIRECT_ONEBLOCK:
        return std::string { "Direct One Block" };
    case Cache_type::TWOWAYSET_FOURBLOCK:
        return std::string { "Two Way Set Four Block" };
    case Cache_type::TWOWAYSET_ONEBLOCK:
        return std::string { "Two Way Set One Block" };
    }
    return std::string {"INVALID"};
}

