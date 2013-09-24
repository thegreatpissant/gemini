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

#include "gemini_parser.h"

#include <algorithm>
#include <string>
#include <stdexcept>

//  TODO: detect incorrect memory/value usage per operation

namespace Gemini_parser {

std::string line;
Gemini_operand *operand;

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

bool get_value ( Register_value &value )
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
    operand->access_type = Gemini_access_type::VALUE;
    strip_whitespace ();
    if (line.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos)
        return false;
    auto e = line.find_first_of(" ");
    operand->label = line.substr(0, e);
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
        operand->access_type = Gemini_access_type::MEMORY;
        line = line.substr(1);
        return get_value ( operand->memory );
    }
    return false;
}
bool value_access ( )
{
    strip_whitespace ();
    if ( line[0] == '#' && line[1] == '$')
    {
        operand->access_type = Gemini_access_type::VALUE;
        line = line.substr(2);
        return get_value ( operand->value );
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
        operand->op = Gemini_op::LDA;
    }
    else if (opcode == "sta")
    {
        operand->op = Gemini_op::STA;
    }
    else if (opcode == "add")
    {
        operand->op = Gemini_op::ADD;
    }
    else if (opcode == "sub")
    {
        operand->op = Gemini_op::SUB;
    }
    else if (opcode == "and")
    {
        operand->op = Gemini_op::AND;
    }
    else if (opcode == "or")
    {
        operand->op = Gemini_op::OR;
    }
    else if (opcode == "nota")
    {
        operand->op = Gemini_op::NOTA;
    }
    else if (opcode == "ba")
    {
        operand->op = Gemini_op::BA;
    }
    else if (opcode == "be")
    {
        operand->op = Gemini_op::BE;
    }
    else if (opcode == "bl")
    {
        operand->op = Gemini_op::BL;
    }
    else if (opcode == "bg")
    {
        operand->op = Gemini_op::BG;
    }
    else if (opcode == "nop")
    {
        operand->op = Gemini_op::NOP;
    }
    else if (line[idx_inst_beg + idx_op_end] == ':')
    {
        operand->op = Gemini_op::LABEL;
        operand->label = opcode;
    }
    else if (line[idx_inst_beg + idx_op_end] != ':')
    {
        operand->op = Gemini_op::INVALID;
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
        operand->op = Gemini_op::EMPTY;
        return;
    }

    get_opcode ();

    if (operand->op == Gemini_op::INVALID)
        return;

    if (operand->op == Gemini_op::LABEL)
        return;

    if ( (operand->op == Gemini_op::NOP || operand->op == Gemini_op::NOTA) && is_empty_line () )
        return;

    if ( operand->op == Gemini_op::BA || operand->op == Gemini_op::BE ||
         operand->op == Gemini_op::BG || operand->op == Gemini_op::BL)
    {
        if ( get_jump_label () && is_empty_line () )
            return;
    }
    else if ( memory_access () && is_empty_line () )
        return;
    else if ( value_access () && is_empty_line () )
        return;

    operand->op = Gemini_op::INVALID;
    return;
}

Gemini_operand * parse_instruction ( const std::string l)
{
    line = l;
    operand = new Gemini_operand ();

    strip_comment () ;
    strip_whitespace ();

    parse_opcode ();

    return operand;
}


}
