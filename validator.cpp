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

#include "validator.h"
#include "gemini_parser.h"
#include "gemini_types.h"

/*
 * The VALIDATOR!! - The Compiler, comming soon.
 */

//  User our parser and types to validate and translate the source
//  file from what they think it is into a pseudo byte_code form, and
//  return error if it is not.  Linking is done later in gemini.cpp.
Error_lines validate_source (Source_code source_code, std::shared_ptr<Byte_code> byte_code)
{
    Error_lines error_lines;

    for ( std::size_t i = 0; i < source_code.size(); i++ )
    {
        Gemini_operand *operand = Gemini_parser::parse_instruction(source_code[i]);
        if ( operand->op== Gemini_op::INVALID )
            error_lines.push_back (i);
        if ( operand->op != Gemini_op::EMPTY )
            byte_code->push_back (*operand);
    }
    return error_lines;  // HA, HA.
}
