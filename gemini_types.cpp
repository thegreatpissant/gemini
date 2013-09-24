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

#include "gemini_types.h"
#include <iomanip>
#include <sstream>

//  An include default translation of a Gemini_operand to a std::string
std::string gemini_operand_to_std_string ( Gemini_operand gemini_operand )
{
    std::string operand_string =
        gemini_op_to_std_string (gemini_operand.op) + " " +
        gemini_access_type_to_std_string(gemini_operand.access_type) + " ";
    if (gemini_operand.access_type == Gemini_access_type::MEMORY)
        operand_string += gemini_register_value_to_std_string(gemini_operand.memory);
    else
        operand_string += gemini_register_value_to_std_string(gemini_operand.value);

    return operand_string;
}

std::string gemini_register_value_to_std_string (Register_value memory)
{
    std::stringstream s;
    s << "0x" <<  std::setfill ('0') << std::setw(sizeof(Register_value)*2) <<
         memory;
//    return std::to_string(memory);
    return s.str();
}

//  An include default translation of a Gemini_access_type to a std::string
std::string gemini_access_type_to_std_string ( Gemini_access_type gemini_access_type )
{
    switch (gemini_access_type)
    {
    case Gemini_access_type::MEMORY :
        return std::string {"M"};
    case Gemini_access_type::VALUE :
        return std::string {"V"};
    case Gemini_access_type::NONE :
        return std::string {"I"};
    }
    return std::string {"I"};
}

//  An include default translation of a Gemini_op to a std::string
std::string gemini_op_to_std_string ( Gemini_op gemini_op )
{
    switch (gemini_op)
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
    case Gemini_op::NOP:
        return std::string {"NOP"};
    case Gemini_op::INVALID:
        return std::string {"INVALID"};
    case Gemini_op::EMPTY:
        return std::string {"EMPTY"};
}
    return std::string {"INVALID"};
}

