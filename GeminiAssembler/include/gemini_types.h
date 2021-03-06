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

#ifndef GEMINI_TYPES_H
#define GEMINI_TYPES_H

#include <string>
#include <vector>

//  Type definintions
using Label = std::string;
using Register_value = u_int8_t;
using Memory_loc = u_int8_t;
using Value = int16_t;
using Byte_code_segment = u_int32_t;

//  new type of enums
enum class Gemini_op : u_int8_t
{
    LABEL = 0x00,
    LDA   = 0x01,
    STA	  = 0x02,
    ADD   = 0x03,
    SUB   = 0x04,
    AND   = 0x05,
    OR    = 0x06,
    NOTA  = 0x07,
    BA    = 0x08,
    BE    = 0x09,
    BL    = 0x0A,
    BG    = 0x0B,
    NOP   = 0x0C,
    EMPTY = 0xFE,
    INVALID = 0xFF,
    BGE   = 0x0D,
    BLE   = 0x0E,
    BNE   = 0x0F,
    MUL   = 0x10,
    DIV   = 0x11,
    JMP   = 0x12,
    RET   = 0x13,
    HLT   = 0x14,
    SETHI = 0x15,
    SETLO = 0x16,
    ADDSL = 0x17,
    SUBSL = 0X18,
    MULSL = 0X19,
    DIVSL = 0X1A,
    LDHI  = 0x1B,
    LDLO  = 0X1C
};
enum class Gemini_access_type : u_int8_t
{
    NONE = 0xFF,
    MEMORY = 0x00,
    VALUE  = 0x01
};

//  nothing fancy and new here
struct Gemini_operand
{
    Gemini_op op;
    Gemini_access_type access_type;
    Value value;
    Label label;
};


//  Abstract, Abstract, more custom types
using Operand_code = std::vector<Gemini_operand>;
using Byte_code = std::vector<Byte_code_segment>;
using Source_code = std::vector<std::string>;
using Error_lines = std::vector<std::size_t>;


#endif // GEMINI_TYPES_H

