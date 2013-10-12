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

#ifndef GEMINIASSEMBLERMODEL_H
#define GEMINIASSEMBLERMODEL_H

#include <string>
#include <memory>

#include "gemini_types.h"

class GeminiAssemblerModel {
private:
  std::string assembly_file_name;
  std::string bytecode_file_name;

  std::shared_ptr<Source_code> assembly_code;
  std::shared_ptr<Byte_code> byte_code;

  std::shared_ptr<Error_lines> error_lines;

public:
  GeminiAssemblerModel();
  void set_assembly_file_name(std::string s);
  std::string get_assembly_file_name ();
  std::string get_bytecode_file_name ();
  void set_bytecode_file_name(std::string s);
  void set_bytecode (std::shared_ptr<Byte_code> bc);
  void set_assembly (std::shared_ptr<Source_code> ac);
  void set_error_lines (Error_lines el);
  std::shared_ptr<Error_lines> get_error_lines ();
  std::shared_ptr<Source_code> get_source_code ();
  bool assembly_file_name_is_set ();
  bool bytecode_file_name_is_set ();
  std::shared_ptr<Byte_code> get_byte_code();
};

#endif // GEMINIASSEMBLERMODEL_H
