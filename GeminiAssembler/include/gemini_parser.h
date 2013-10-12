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
 *  Gemini parser: Parses a line of gemini asm checking for validity
 *  and returning its operand equivilant.
 */
#ifndef GEMINI_PARSER_H
#define GEMINI_PARSER_H

#include "gemini_types.h"

#include <string>
namespace Gemini_parser {

  // get opcode from the instruction string
void get_opcode();
  //  Interpret what the instruction requires of the opcode
void parse_opcode();
  //  Is the line empty
bool empty_line();
  //  remove comment from line
void strip_comment();
  //  is this a memory access?
bool memory_access();
  //  is this a value access?
bool value_access();
  //  Parse the full instruction from opcode to value and return the operand.
Gemini_operand * parse_instruction ( const std::string l);

}


#endif // GEMINI_PARSER_H
