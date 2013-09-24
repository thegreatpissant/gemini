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

#ifndef GEMINI_PARSER_H
#define GEMINI_PARSER_H

#include "gemini_types.h"

#include <string>
namespace Gemini_parser {


void get_opcode();
void parse_opcode();
bool empty_line();
void strip_comment();
bool memory_access();
bool value_access();

Gemini_operand * parse_instruction ( const std::string l);

}


#endif // GEMINI_PARSER_H
