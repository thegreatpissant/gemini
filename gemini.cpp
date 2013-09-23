/*
 *  James Feister
 *  cisc360 Gemini project
 */
#include "gemini.h"
#include "ui_gemini.h"
#include "gemini_types.h"
#include "gemini_system.h"
#include "validator.h"
#include "invalid_file_dialog.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>

extern Gemini_system gemini_system;

gemini::gemini(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gemini)
{
    ui->setupUi(this);
}

gemini::~gemini()
{
    delete ui;
}

void gemini::gemini_display_callback()
{
    Gemini_system_info gemini_system_info = gemini_system.get_system_info();
    //  Set all the registers
   ui->reg_A    ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.A)));
   ui->reg_B    ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.B)));
   ui->reg_Acc  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.Acc )));
   ui->reg_Zero ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.Zero )));
   ui->reg_One  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.One )));
   ui->reg_PC   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.PC )));
   ui->reg_MAR  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.MAR )));
   ui->reg_MDR  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.MDR )));
   ui->reg_TEMP ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.TEMP )));
   ui->reg_IR   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.IR )));
   ui->reg_CC   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.CC )));

    //  Set the Instruction
    ui->inst_label_value ->setText(QString::fromStdString(gemini_operand_to_std_string(gemini_system_info.instruction)));

    //  Set the Instruction index
   ui->inst_label_index ->setText( QString::fromStdString(gemini_register_value_to_std_string(gemini_system_info.instruction_index)));
}

void gemini::on_actionQuit_triggered()
{
    qApp->quit();
}

void gemini::on_actionLoad_triggered()
{
    ui->pushButton->setEnabled(false);
    QString file_name = QFileDialog::getOpenFileName(
                this,
                tr("Open File"), QString (), tr ("Gemini Files (*.s)"));

    if ( file_name.isEmpty() )
        return;

    QFile file( file_name );
    if ( !file.open( QIODevice::ReadOnly) ) {
        QMessageBox::critical (this, tr("Error"), tr("Could not open file") );
        return;
    }

    Source_code source_code;
    QTextStream in (&file);
    QTextStream in_stream (&file);
    while (!in_stream.atEnd())
    {
        source_code.push_back ( in_stream.readLine().toStdString() );
    }
    file.close();

    //  TODO: Put into a compilation class and remove from this function
    //  Validate source code file and retrive its byte_code
    std::shared_ptr<Byte_code> byte_code = std::shared_ptr<Byte_code> { new Byte_code };
    Error_lines error_lines = validate_source (source_code, byte_code);
    if (! error_lines.empty() )
    {
        Invalid_file_dialog * ifd  = new Invalid_file_dialog(this);
        ifd->set_source_code( source_code );
        ifd->set_error_list( error_lines );
        ifd->show();
        return;
    }

    //  TODO::Spawn this into its own thread
    //  Linker

    //  insert a branch always to the location of main as the first instruction
    Gemini_operand operand;
    operand.op = Gemini_op::BA;
    operand.label = "main";
    operand.access_type = Gemini_access_type::VALUE;
    byte_code->insert(byte_code->begin(), operand);

    std::map<Label, int> label_table;
    for ( std::size_t i = 0; i < byte_code->size(); i++)
    {
        if ((*byte_code)[i].op == Gemini_op::LABEL)
        {
            label_table[(*byte_code)[i].label] = i;
            byte_code->erase(byte_code->begin() + i);
        }
    }

    if ( label_table.find(Label{"main"}) == label_table.end())
    {
        QMessageBox *mb = new QMessageBox(this);
        mb->setText("linking failed. main label not found.");
        mb->show();
        return;
    }

    //  Link the labels to the bytecode
    for (auto &bc : *byte_code)
    {
        if (bc.op == Gemini_op::BA || bc.op == Gemini_op::BE || bc.op == Gemini_op::BG || bc.op == Gemini_op::BL)
        {
            //  TODO: Generate a warning dialog for unknown label lookup.
            if (label_table.find(bc.label) == label_table.end())
            {
                QMessageBox *mb = new QMessageBox(this);
                mb->setText("linking failed. label " + QString::fromStdString(bc.label) + " not found.");
                mb->show();
            }
            else {
                bc.value = label_table[bc.label];
                bc.label.clear();
            }
        }
    }

    //  Send Byte_code to gemini system
    gemini_system.load_byte_code(byte_code);

    //  Set CPU to ready
    gemini_system.power_on();

    //  Enable the 'next' button
    ui->pushButton->setEnabled(true);

    this->gemini_display_callback();
}

void gemini::on_pushButton_clicked()
{
    gemini_system.cycle_clock();
    this->gemini_display_callback();
}
