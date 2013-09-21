#include "validator.h"
#include "gemini_parser.h"
#include "gemini_ops.h"
#include "gemini_types.h"

Error_lines validate_source (Source_code source_code)
{
    Error_lines error_lines;

    for ( std::size_t i = 0; i < source_code.size(); i++ )
    {
        Gemini_operand *op = Gemini_parser::parse_instruction(source_code[i]);
        if ( op->operand == Gemini_op::INVALID )
            error_lines.push_back (i);
    }

    return error_lines;
}
