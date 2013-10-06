#include "geminiassemblermodel.h"

void GeminiAssemblerModel::set_bytecode_file_name(std::string s)
{
    this->bytecode_file_name = s;
}

void GeminiAssemblerModel::set_bytecode(std::shared_ptr<Byte_code> bc)
{
    this->byte_code = bc;
}

void GeminiAssemblerModel::set_assembly(std::shared_ptr<Source_code> ac)
{
    this->assembly_code = ac;
}

void GeminiAssemblerModel::set_error_lines(Error_lines el)
{
    this->error_lines = std::make_shared<Error_lines> (el);
}

std::shared_ptr<Error_lines> GeminiAssemblerModel::get_error_lines()
{
    return this->error_lines;
}

std::shared_ptr<Source_code> GeminiAssemblerModel::get_source_code()
{
    return this->assembly_code;
}
std::shared_ptr<Byte_code> GeminiAssemblerModel::get_byte_code ()
{
    return this->byte_code;
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

std::string GeminiAssemblerModel::get_assembly_file_name()
{
    return this->assembly_file_name;
}

std::string GeminiAssemblerModel::get_bytecode_file_name()
{
    return this->bytecode_file_name;
}
