#include "validator.h"
#include "gemini_parser.h"
#include "gemini_types.h"

Error_lines validate_source (Source_code source_code, std::shared_ptr<Byte_code> byte_code)
{
    Error_lines error_lines;

    for ( std::size_t i = 0; i < source_code.size(); i++ )
    {
        Gemini_operand *operand = Gemini_parser::parse_instruction(source_code[i]);
        if ( operand->op== Gemini_op::INVALID )
            error_lines.push_back (i);
        if ( operand->op != Gemini_op::EMPTY )
            byte_code->push_back (*operand);
    }

    return error_lines;
}
