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

#include "geminiassemblermodel.h"

void GeminiAssemblerModel::set_bytecode_file_name(std::string s)
{
    this->bytecode_file_name = s;
}

void GeminiAssemblerModel::set_bytecode(std::shared_ptr<Byte_code> bc)
{
    this->byte_code = bc;
}

void GeminiAssemblerModel::set_assembly(std::shared_ptr<Source_code> ac)
{
    this->assembly_code = ac;
}

void GeminiAssemblerModel::set_error_lines(Error_lines el)
{
    this->error_lines = std::make_shared<Error_lines> (el);
}

std::shared_ptr<Error_lines> GeminiAssemblerModel::get_error_lines()
{
    return this->error_lines;
}

std::shared_ptr<Source_code> GeminiAssemblerModel::get_source_code()
{
    return this->assembly_code;
}
std::shared_ptr<Byte_code> GeminiAssemblerModel::get_byte_code ()
{
    return this->byte_code;
}

bool GeminiAssemblerModel::assembly_file_name_is_set()
{
    return (!this->assembly_file_name.empty());
}

bool GeminiAssemblerModel::bytecode_file_name_is_set()
{
    return (!this->bytecode_file_name.empty());
}


GeminiAssemblerModel::GeminiAssemblerModel()
{
}

void GeminiAssemblerModel::set_assembly_file_name(std::string s)
{
    this->assembly_file_name = s;
}

std::string GeminiAssemblerModel::get_assembly_file_name()
{
    return this->assembly_file_name;
}

std::string GeminiAssemblerModel::get_bytecode_file_name()
{
    return this->bytecode_file_name;
}
