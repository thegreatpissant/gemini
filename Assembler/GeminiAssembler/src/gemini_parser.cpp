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

//  Global to this namespace
std::string line;
Gemini_operand *operand;  //  TODO: change to shared_ptr <Gemini_operand> and test

//  Remove the coment from the line
void strip_comment ()
{
    std::string t = line.substr(0, line.find_first_of ("!"));
    line = t;
}

//  is the line empty
bool is_empty_line ()
{
    if ( line.empty() )
        return true;

    if ( std::string::npos == line.find_first_not_of(" \t\r\n") )
        return true;

    return false;
}

//  Remove whitespace from begining of a line
void strip_whitespace ( )
{
    auto beg = line.find_first_not_of(" \t\r\n");
    if ( beg == std::string::npos )
    {
        line.clear();
        return;
    }
    line = line.substr(beg);
}

//  get the value see reference for trailing whitespace test
bool get_value ( Register_value &value )
{
    //  Need some sort of default 
    value = -1;

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

//  Get the jump label
bool get_jump_label ( )
{
    operand->access_type = Gemini_access_type::VALUE;
    strip_whitespace ();
    if ( line.empty())
        return false;
    auto e = line.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
    if ( e == std::string::npos )
        operand->label = line;
    else
        operand->label = line.substr(0, e);
    if ( e == std::string::npos )
    {
        line.clear();
        return true;
    }
    else
        line = line.substr(e);

//        if (line.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos)
//            return false;

    return true;
}

//  Is this a memory access?
bool memory_access ( )
{
    strip_whitespace ();
    if ( line[0] == '$' )
    {
        operand->access_type = Gemini_access_type::MEMORY;
        line = line.substr(1);
        return get_value ( operand->value );
    }
    return false;
}

//  Is this a value access?
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

// Get opcode from the instruction string
void get_opcode ( )
{
    std::string::size_type idx_inst_beg, idx_op_end;
    std::string opcode;

    //  identify the opcode
    idx_inst_beg = line.find_first_not_of(" \t");
    idx_op_end = line.substr(idx_inst_beg).find_first_of(" :\n");

    //  parse out the opcode
    opcode = line.substr(idx_inst_beg, idx_op_end);

    //  Translate into our enum
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

    //  Clean up the rest of the line for residual junk check.
    if ( idx_op_end == std::string::npos )
        line.clear();
    else //  What else is in the line?
        line = line.substr(idx_inst_beg+idx_op_end);
    strip_whitespace();
}

//  Interpret what the instruction requires of the opcode
void parse_opcode ()
{
    strip_comment () ;
    strip_whitespace ();

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

//  Parse the full instruction from opcode to value and return the operand.
Gemini_operand * parse_instruction ( const std::string l)
{
    line = l;
    operand = new Gemini_operand ();
    std::replace (line.begin(), line.end(), '\t', ' ');
    parse_opcode ();

    return operand;
}


}
