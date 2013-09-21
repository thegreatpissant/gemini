#ifndef GEMINI_OPS_H
#define GEMINI_OPS_H

#include <string>

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

using Memory = int;
using Label = std::string;
using Value = int;

struct Gemini_operand {
    Gemini_op operand;
    Memory memory;
    Value value;
    Label label;
};

#endif // GEMINI_OPS_H
