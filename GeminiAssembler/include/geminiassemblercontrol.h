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

#ifndef GEMINIASSEMBLERCONTROL_H
#define GEMINIASSEMBLERCONTROL_H


#include "geminiassemblermodel.h"

#include <memory>
#include "geminiassemblerwindow.h"
class GeminiAssemblerWindow;

class GeminiAssemblerControl {
private:
  GeminiAssemblerModel *model;
  GeminiAssemblerWindow *view;

public:
  GeminiAssemblerControl();

  void setModel (GeminiAssemblerModel *m );
  void setView  (GeminiAssemblerWindow *v );

  bool convert_assembly_to_bytecode ();

  bool test_and_read_assembly_file();
  bool test_bytecode_file();
  bool validate_assembly_source();
  bool write_byte_code_to_file();
};

#endif // GEMINIASSEMBLERCONTROL_H
