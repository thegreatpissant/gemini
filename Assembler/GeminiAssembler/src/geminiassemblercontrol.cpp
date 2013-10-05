#include "geminiassemblercontrol.h"
#include "validator.h"

#include <fstream>
#include <map>

#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QTextStream>


GeminiAssemblerControl::GeminiAssemblerControl()
{
}

void GeminiAssemblerControl::setModel(GeminiAssemblerModel *m)
{
    this->model = m;
}


void GeminiAssemblerControl::setView(GeminiAssemblerWindow *v)
{
    this->view = v;
}

bool GeminiAssemblerControl::test_and_read_assembly_file()
{
   QFile assembly_file ( QString::fromStdString(model->get_assembly_file_name()));
   if (!assembly_file.open(QIODevice::ReadOnly))
        return false;

   Source_code source_code;
   QTextStream in_stream (&assembly_file);
   while (!in_stream.atEnd())
   {
       source_code.push_back (in_stream.readLine().toStdString());
   }
   assembly_file.close();
   model->set_assembly(std::make_shared<Source_code> (source_code));
   return true;
}

bool GeminiAssemblerControl::validate_assembly_source ()
{

   Error_lines error_lines = validate_source (*this->model->get_source_code());
   if (! error_lines.empty() )
   {
       model->set_error_lines(error_lines);
       return false;
   }
    return true;
}


bool GeminiAssemblerControl::test_bytecode_file()
{
    std::ofstream bytecode_file (this->model->get_bytecode_file_name());
    if (! bytecode_file )
        return false;
    return true;
}

bool GeminiAssemblerControl::convert_assembly_to_bytecode()
{
    std::shared_ptr<Operand_code> operand_code = std::make_shared<Operand_code>(source_to_operands (this->model->get_source_code()));

    //  Insert a branch always to the location of main as the first instruction
    Gemini_operand operand;
    operand.op = Gemini_op::BA;
    operand.label = "main";
    operand.access_type = Gemini_access_type::VALUE;
    operand_code->insert(operand_code->begin(), operand);

    //  Generate the label table to perform look ups against.
    std::map<Label, int> label_table;
    for ( std::size_t i = 0; i < operand_code->size(); i++)
    {
        if ((*operand_code)[i].op == Gemini_op::LABEL)
        {
            label_table[(*operand_code)[i].label] = i;
            operand_code->erase(operand_code->begin() + i);
        }
    }

    //  Check for the main label.
    if ( label_table.find(Label{"main"}) == label_table.end())
    {
        QMessageBox *mb = new QMessageBox(0);
        mb->setText("linking failed. main label not found.");
        mb->show();
        return false;
    }

    //  Link in the labels
    for (auto &oc : *operand_code)
    {
        if (oc.op == Gemini_op::BA || oc.op == Gemini_op::BE || oc.op == Gemini_op::BG || oc.op == Gemini_op::BL)
        {
            if (label_table.find(oc.label) == label_table.end())
            {
                QMessageBox *mb = new QMessageBox(0);
                mb->setText("Linking failed. label " + QString::fromStdString(oc.label) + " not found.");
                mb->show();
            }
            else {
                oc.value = label_table[oc.label];
                oc.label.clear();
            }
        }
    }

    //  Convert operands to gemini bytecode

    //  Write out the bytecode file
    std::ofstream bytecode_file (this->model->get_bytecode_file_name());
    if (! bytecode_file )
        return false;

    return true;
}
