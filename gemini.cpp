/*
 *  James Feister
 *  cisc360 Gemini project
 */
#include "gemini.h"
#include "ui_gemini.h"
#include "gemini_types.h"
#include "validator.h"
#include "invalid_file_dialog.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>

#include <memory>

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
    Gemini_operand op;
    op.operand = Gemini_op::BA;
    op.label = "main";
    byte_code->insert(byte_code->begin(), op);

    std::map<Label, int> label_table;
    for ( std::size_t i = 0; i < byte_code->size(); i++)
    {
        if ((*byte_code)[i].operand == Gemini_op::LABLE)
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
        if (bc.operand == Gemini_op::BA || bc.operand == Gemini_op::BE || bc.operand == Gemini_op::BG || bc.operand == Gemini_op::BL)
        {
            //  TODO: Generate a warning dialog for unknown label lookup.
            bc.memory = label_table[bc.label];
            bc.label.clear();
        }
    }

    //  Send Byte_code to gemini system

    //  Set CPU to ready

    //  Enable the 'next' button
    ui->pushButton->setEnabled(true);

}

void gemini::on_pushButton_clicked()
{
    

}
