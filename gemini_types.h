#ifndef GEMINI_TYPES_H
#define GEMINI_TYPES_H

#include <string>
#include <vector>

enum class Gemini_op {
    LABEL,
    LDA,
    STA,
    ADD,
    SUB,
    AND,
    OR,
    NOTA,
    BA,BE,BL,BG,
    NOP,
    EMPTY,
    INVALID
};
//  TODO: Replace these with overloaded versions
//  An include default translation of a Gemini_op to a std::string
std::string gemini_op_to_std_string ( Gemini_op gemini_op );


enum class Gemini_access_type {
    NONE,
    MEMORY,
    VALUE
};
//  An include default translation of a Gemini_access_type to a std::string
std::string gemini_access_type_to_std_string ( Gemini_access_type gemini_access_type );

//using Memory_loc = int;
//using Value = int;
using Label = std::string;
using Register_value = int;
using Memory_loc = std::size_t;
using Value = Register_value;
std::string gemini_register_value_to_std_string (Register_value memory);

struct Gemini_operand {
    Gemini_op op;
    Gemini_access_type access_type;
    Register_value memory;
    Register_value value;
    Label label;
//    Gemini_operand operator= (Gemini_operand &rhs)
//    {
//        Gemini_operand tmp;
//        tmp.op = rhs.op;
//        tmp.access_type = rhs.access_type;
//        tmp.memory = rhs.memory;
//        tmp.value = rhs.value;
//        tmp.label = rhs.label;

//        return tmp;
//    }
};

//  An include default translation of a Gemini_operand to a std::string
std::string gemini_operand_to_std_string ( Gemini_operand gemini_operand );

using Byte_code   = std::vector<Gemini_operand>;
using Source_code = std::vector<std::string>;
using Error_lines = std::vector<std::size_t>;

struct Gemini_system_info {
    Memory_loc A;
    Memory_loc B;
    Memory_loc Acc;
    Memory_loc Zero;
    Memory_loc One;
    Memory_loc PC;
    Memory_loc MAR;
    Memory_loc MDR;
    Memory_loc TEMP;
    Memory_loc IR;
    Memory_loc CC;

    Gemini_operand instruction;
    Memory_loc instruction_index;
};

#endif // GEMINI_TYPES_H
