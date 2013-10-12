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

//  new type of typedef!!
//using Register_value = u_int8_t;
//using Memory_loc = u_int8_t;
//using Value = u_int8_t;
//using Instruction_register = u_int32_t;
//using Gemini_op_type = u_int8_t;

typedef u_int8_t Memory_loc;
typedef u_int8_t Gemini_op_type;
typedef int16_t Value;
typedef int16_t Register_value;
typedef u_int32_t Instruction_register;
using Byte_code_segment = u_int32_t;
using Byte_code = std::vector<Byte_code_segment>;

//  new type of enums
enum class Gemini_op : Gemini_op_type
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
enum class Gemini_access_type : Gemini_op_type
{
    NONE = 0xFF,
    MEMORY = 0x00,
    VALUE  = 0x01
};

enum class Cache_type {
    DIRECT_ONEBLOCK,
    DIRECT_FOURBLOCK,
    TWOWAYSET_ONEBLOCK,
    TWOWAYSET_FOURBLOCK
};

//  Some system information
struct Gemini_system_info
{
    Register_value A;
    Register_value B;
    Register_value Acc;
    Register_value Zero;
    Register_value One;
    Register_value PC;
    Register_value MAR;
    Register_value MDR;
    Register_value TEMP;
    Instruction_register  IR;
    Register_value CC;
    Register_value CE;
    Register_value OVF;
    int32_t SL0;
    int32_t SL1;
    Register_value instruction_index;
    int jmp_stack_depth;
    int cache_hits;
    int cache_misses;
    Cache_type cache_type;
};

std::string gemini_op_to_std_string( Gemini_op gemini_op );
std::string gemini_access_type_to_std_string( Gemini_access_type gemini_access_type );
std::string gemini_register_value_to_std_string( Register_value rv );
std::string gemini_instruction_register_to_std_string ( Instruction_register ir );
std::string gemini_instruction_register_value_to_std_string ( Instruction_register ir );
std::string gemini_value_to_std_string ( Value value );
std::string gemini_cache_type_to_std_string ( Cache_type cache_type );


#endif // GEMINI_TYPES_H
