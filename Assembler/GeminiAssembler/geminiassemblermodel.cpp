#include "geminiassemblermodel.h"

void GeminiAssemblerModel::set_bytecode_file_name(std::string s)
{
    this->bytecode_file_name = s;
}

void GeminiAssemblerModel::set_bytecode(Byte_code bc)
{
    this->byte_code = bc;
}

void GeminiAssemblerModel::set_assembly(Source_code ac)
{
    this->assembly_code = ac;
}

void GeminiAssemblerModel::set_error_lines(Error_lines el)
{
    this->error_lines = el;
}

Error_lines GeminiAssemblerModel::get_error_lines()
{
    return this->error_lines;
}

Source_code GeminiAssemblerModel::get_source_code()
{
    return this->assembly_code;
}

bool GeminiAssemblerModel::assembly_file_name_is_set()
{
    return (!this->assembly_file_name.empty());
}

bool GeminiAssemblerModel::bytecode_file_name_is_set()
{
    return (!this->bytecode_file_name.empty());
}


GeminiAssemblerModel::GeminiAssemblerModel()
{
}

void GeminiAssemblerModel::set_assembly_file_name(std::string s)
{
    this->assembly_file_name = s;
}
