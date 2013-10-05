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
 * The VALIDATOR!! - The Compiler, comming soon.
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <memory>
#include "../../gemini_types.h"

//  Validate and translate the source file from what they think it is
//  into a pseudo byte_code form, and return error lines if it is not.
Error_lines validate_source (Source_code source_code);
Operand_code source_to_operands (std::shared_ptr<Source_code> source_code);

#endif // VALIDATOR_H
