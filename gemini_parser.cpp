#include "gemini_parser.h"

#include <algorithm>
#include <string>
#include <stdexcept>

namespace Gemini_parser {

std::string line;
Gemini_operand *op;

void strip_comment ()
{
    std::string t = line.substr(0, line.find_first_of ("!"));
    line = t;
}

bool is_empty_line ()
{
    if ( line.empty() )
        return true;

    if ( std::string::npos == line.find_first_not_of(" \t\r\n") )
        return true;

    return false;
}

void strip_whitespace ( )
{
    auto beg = line.find_first_not_of(" \t");
    if ( beg == std::string::npos )
        return;
    line = line.substr(beg);
}

bool get_value ( int& value )
{
    value = -1;
//    std::string val = line.substr(0,line.find_first_not_of(" "));
//    if (val.size() > 3)
//        return false;

    //  As per http://www.kumobius.com/2013/08/c-string-to-int/
    //  With a trailing whitespace test.
    try
    {
        std::size_t last_char;
        value = std::stoi(line, &last_char, 10);
        last_char = line.find_first_not_of("1234567890");
        if ( last_char == std::string::npos )
            line = " ";
        else
            line = line.substr(last_char);
        return true;
    }
    catch (std::invalid_argument& )
    {
        return false;
    }
    catch (std::out_of_range& )
    {
        return false;
    }
}

bool get_jump_label ( )
{
    op->access_type = Gemini_access_type::MEMORY;
    strip_whitespace ();
    if (line.find_first_not_of("abcdefghijklmnopqrstuvwxyz ") != std::string::npos)
        return false;
    auto e = line.find_first_of(" ");
    op->label = line.substr(0, e);
    if ( e == std::string::npos )
        line.clear();
    else
        line = line.substr(e);

    return true;
}

bool memory_access ( )
{
    strip_whitespace ();
    if ( line[0] == '$' )
    {
        op->access_type = Gemini_access_type::MEMORY;
        line = line.substr(1);
        return get_value ( op->memory );
    }
    return false;
}
bool value_access ( )
{
    strip_whitespace ();
    if ( line[0] == '#' && line[1] == '$')
    {
        op->access_type = Gemini_access_type::VALUE;
        line = line.substr(2);
        return get_value ( op->value );
    }
    return false;
}

void get_opcode ( )
{
    std::string::size_type idx_inst_beg, idx_op_end;
    std::string opcode;

    idx_inst_beg = line.find_first_not_of(" \t");
    idx_op_end = line.substr(idx_inst_beg).find_first_of(" :\n");

    opcode = line.substr(idx_inst_beg, idx_op_end);

    if (opcode == "lda")
    {
        op->operand = Gemini_op::LDA;
    }
    else if (opcode == "sta")
    {
        op->operand = Gemini_op::STA;
    }
    else if (opcode == "add")
    {
        op->operand = Gemini_op::ADD;
    }
    else if (opcode == "sub")
    {
        op->operand = Gemini_op::SUB;
    }
    else if (opcode == "and")
    {
        op->operand = Gemini_op::AND;
    }
    else if (opcode == "or")
    {
        op->operand = Gemini_op::OR;
    }
    else if (opcode == "nota")
    {
        op->operand = Gemini_op::NOTA;
    }
    else if (opcode == "ba")
    {
        op->operand = Gemini_op::BA;
    }
    else if (opcode == "be")
    {
        op->operand = Gemini_op::BE;
    }
    else if (opcode == "bl")
    {
        op->operand = Gemini_op::BL;
    }
    else if (opcode == "bg")
    {
        op->operand = Gemini_op::BG;
    }
    else if (opcode == "nop")
    {
        op->operand = Gemini_op::NOP;
    }
    else if (line[idx_inst_beg + idx_op_end] == ':')
    {
        op->operand = Gemini_op::LABLE;
        op->label = opcode;
    }
    else if (line[idx_inst_beg + idx_op_end] != ':')
    {
        op->operand = Gemini_op::INVALID;
        return;
    }

    if ( idx_op_end == std::string::npos )
        line.clear();
    else
        line = line.substr(idx_inst_beg+idx_op_end+1);
}

void parse_opcode ()
{
    if ( is_empty_line() )
    {
        op->operand = Gemini_op::EMPTY;
        return;
    }

    get_opcode ();

    if (op->operand == Gemini_op::INVALID)
        return;

    if (op->operand == Gemini_op::LABLE)
        return;

    if ( (op->operand == Gemini_op::NOP || op->operand == Gemini_op::NOTA)
         && is_empty_line () )
        return;

    if (op->operand == Gemini_op::BA || op->operand == Gemini_op::BE
            || op->operand == Gemini_op::BG || op->operand == Gemini_op::BL)
    {
        if ( get_jump_label () && is_empty_line () )
            return;
    }
    else if ( memory_access () && is_empty_line () )
        return;
    else if ( value_access () && is_empty_line () )
        return;

    op->operand = Gemini_op::INVALID;
    return;
}

Gemini_operand * parse_instruction ( const std::string l)
{
    line = l;
    op = new Gemini_operand ();

    strip_comment () ;
    strip_whitespace ();

    parse_opcode ();

    return op;
}


}
