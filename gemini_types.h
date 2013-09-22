#ifndef GEMINI_TYPES_H
#define GEMINI_TYPES_H

#include <string>
#include <vector>

enum class Gemini_op {
    LABLE,
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

enum class Gemini_access_type {
    MEMORY,
    VALUE
};

using Memory_loc = int;
using Label = std::string;
using Value = int;

struct Gemini_operand {
    Gemini_op operand;
    Gemini_access_type access_type;
    Memory_loc memory;
    Value value;
    Label label;
};

using Byte_code   = std::vector<Gemini_operand>;
using Source_code = std::vector<std::string>;
using Error_lines = std::vector<std::size_t>;

#endif // GEMINI_TYPES_H
