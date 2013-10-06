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
 * Gemini types: Various types used in the gemini application
 */

#ifndef GEMINI_TYPES_H
#define GEMINI_TYPES_H

#include <string>
#include <vector>

//  new type of typedef!!
using Label = std::string;
using Register_value = u_int8_t;
using Memory_loc = u_int8_t;
using Value = u_int8_t;
using Instruction_register = u_int32_t;

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
//    BGE   = 0x0D,
//    BLE   = 0x0E,
//    BNE   = 0x0F,
//    MUL   = 0x10,
//    DIV   = 0x11,
//    JMP   = 0x12,
//    RET   = 0x13,
//    HLT   = 0x14
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

//  Some system information
struct Gemini_system_info
{
    Memory_loc A;
    Memory_loc B;
    Memory_loc Acc;
    Memory_loc Zero;
    Memory_loc One;
    Memory_loc PC;
    Memory_loc MAR;
    Memory_loc MDR;
    Memory_loc TEMP;
    Instruction_register  IR;
    Memory_loc CC;
    Gemini_operand instruction;
    Memory_loc instruction_index;
};

//  Abstract, Abstract, more custom types
using Operand_code = std::vector<Gemini_operand>;
using Byte_code_segment = u_int32_t;
using Byte_code = std::vector<Byte_code_segment>;
using Source_code = std::vector<std::string>;
using Error_lines = std::vector<std::size_t>;

//  Give me some info!!
//  Functions to retrive human readable information on the defined types above
//  TODO: Replace these with overloaded versions of the same function name
// 'gemini_type_to_std_string'
std::string gemini_op_to_std_string( Gemini_op gemini_op );
std::string gemini_access_type_to_std_string( Gemini_access_type gemini_access_type );
std::string gemini_register_value_to_std_string( Register_value memory );
std::string gemini_operand_to_std_string( Gemini_operand gemini_operand );

#endif // GEMINI_TYPES_H
