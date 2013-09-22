#ifndef GEMINI_PARSER_H
#define GEMINI_PARSER_H

#include "gemini_types.h"

#include <string>
namespace Gemini_parser {


void get_opcode();
void parse_opcode();
bool empty_line();
void strip_comment();
bool memory_access();
bool value_access();

Gemini_operand * parse_instruction ( const std::string l);

}


#endif // GEMINI_PARSER_H
