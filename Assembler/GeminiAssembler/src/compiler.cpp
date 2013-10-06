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

#include "compiler.h"
#include "gemini_parser.h"

/*
 * The Compiler
 */

static void validate (Source_code &source_code, Operand_code &operand_code, Error_lines &error_lines)
{
    for ( std::size_t i = 0; i < source_code.size(); i++ )
    {
        Gemini_operand *operand = Gemini_parser::parse_instruction(source_code[i]);
        if ( operand->op== Gemini_op::INVALID )
            error_lines.push_back (i);
        if ( operand->op != Gemini_op::EMPTY )
            operand_code.push_back (*operand);
    }
}

//  User our parser and types to validate and translate the source
//  file from what they think it is into a pseudo byte_code form, and
//  return error if it is not.  Linking is done later in gemini.cpp.
Error_lines validate_source (Source_code source_code)
{
    Error_lines error_lines;
    Operand_code operand_code;
    validate (source_code, operand_code, error_lines);
    return error_lines;
}

Operand_code source_to_operands (std::shared_ptr<Source_code> source_code)
{
    Error_lines error_lines;
    Operand_code operand_code;
    validate (*source_code, operand_code, error_lines);
    return operand_code;
}

static Byte_code_segment to_byte_code_segment (Gemini_operand gop)
{
    Byte_code_segment byte_code_segment;
    byte_code_segment ^= byte_code_segment;
    byte_code_segment |= static_cast<u_int8_t>(gop.op);
    byte_code_segment <<= 8;
    byte_code_segment |= static_cast<u_int8_t>(gop.access_type);
    byte_code_segment <<= 16;
    byte_code_segment |= gop.value;
    return byte_code_segment;
}

Byte_code operands_to_bytecode(std::shared_ptr<Operand_code> operand_code)
{
    Byte_code byte_code;
    for (auto &oc: *operand_code)
    {
        byte_code.push_back( to_byte_code_segment(oc) );
    }
    return byte_code;
}

