#include "geminiassemblercontrol.h"

GeminiAssemblerControl::GeminiAssemblerControl()
{
}

void GeminiAssemblerControl::setModel(std::shared_ptr <GeminiAssemblerModel> m)
{
    this->model = m;
}


void GeminiAssemblerControl::setView(std::shared_ptr <GeminiAssemblerWindow> v)
{
    this->view = v;
}

bool GeminiAssemblerControl::test_assembly_file()
{

}


bool GeminiAssemblerControl::test_bytecode_file()
{

}

bool GeminiAssemblerControl::convert_assembly_to_bytecode()
{
}
