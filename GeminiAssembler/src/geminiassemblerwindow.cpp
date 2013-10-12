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
 * *
 * Project 2: Gemini Enhancements, Implement the following
 * - Gemini Assembler: Create bytecode files from our Gemini assembly files
 * - Support Loading of binary files into the Gemini Simulator
 * - Add >=, <= Condition Jump checks
 * - Add JMP and RET commands for functions
 * - Add Cache implementation for One block Direct and 2 Way Set
 * - Extra credit: SETHI and SETLO instruction to handle 32 bit numbers
 * - Extra credit: Overflow on Multiplication and Divide
 * - Extra credit: JMP and RET instructions to support 25 levels of recursion
 * - Extra credit: Allow cache to support 4 memory block grabing at a time
 */

#include "geminiassemblerwindow.h"
#include "ui_geminiassemblerwindow.h"

#include "invalid_file_dialog.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include "gemini_types.h"

GeminiAssemblerWindow::GeminiAssemblerWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::GeminiAssemblerWindow )
{
    ui->setupUi( this );

    ui->pushButton_asmfile->setText(default_assembly_button_text);
    ui->pushButton_bytecode_file->setText(default_bytecode_button_text);
    ui->pushButton_convert->setEnabled(false);
}

GeminiAssemblerWindow::~GeminiAssemblerWindow( )
{
    delete ui;
}

void GeminiAssemblerWindow::setModel( GeminiAssemblerModel *g )
{
    this->model = g ;
}

void GeminiAssemblerWindow::setControl(GeminiAssemblerControl *c)
{
    this->control = c;
}

void GeminiAssemblerWindow::alert_assembly_file_open_failed()
{
    this->ui->pushButton_convert->setEnabled(false);
    this->ui->pushButton_asmfile->setText (default_assembly_button_text);

    QMessageBox::critical (this, tr("Error"), tr("Could not open Assembly file for reading") );
}

void GeminiAssemblerWindow::alert_bytecode_file_open_failed()
{
    this->ui->pushButton_convert->setEnabled(false);
    this->ui->pushButton_bytecode_file->setText (default_assembly_button_text);

    QMessageBox::critical (this, tr("Error"), tr("Could not open Bytecode file for writing") );
}

void GeminiAssemblerWindow::alert_assembly_conversion_failed()
{
   QMessageBox::critical (this, tr("Error"), tr("Assembly Conversion Failed") );
}

void GeminiAssemblerWindow::alert_assembly_source_invalid ()
{
    Invalid_file_dialog *ifd  = new Invalid_file_dialog(this);
    ifd->set_source_code( *model->get_source_code() );
    ifd->set_error_list(  *model->get_error_lines() );
    ifd->show();
}

void GeminiAssemblerWindow::alert_write_byte_code_to_file_failed()
{
    QMessageBox::critical (this, tr("Error"), tr("Writting Bytecode to file failed") );
}

void GeminiAssemblerWindow::on_pushButton_asmfile_clicked( )
{
    QString file_name = QFileDialog::getOpenFileName( this, tr( "Open File" ), QString( ),
                                                      tr( "Gemini Files (*.*)" ) );

    model->set_assembly_file_name( file_name.toStdString( ) );

    if (!this->control->test_and_read_assembly_file())
    {
        this->alert_assembly_file_open_failed();
        return;
    }
    if (!this->control->validate_assembly_source())
    {
        this->alert_assembly_source_invalid();
        return;
    }

    this->ui->pushButton_asmfile->setText(QString::fromStdString(model->get_assembly_file_name()));

    if (model->bytecode_file_name_is_set())
        this->ui->pushButton_convert->setEnabled(true);

}

void GeminiAssemblerWindow::on_pushButton_bytecode_file_clicked( )
{
    QString file_name = QFileDialog::getSaveFileName( this, tr( "Save File" ), QString( ),
                                                      tr( "Gemini Files (*.*)" ) );

    model->set_bytecode_file_name( file_name.toStdString( ) );


    if (this->control->test_bytecode_file())
    {
        this->ui->pushButton_bytecode_file->setText(file_name);

        if (model->assembly_file_name_is_set())
            this->ui->pushButton_convert->setEnabled(true);
    }
    else
    {
        this->alert_bytecode_file_open_failed();
    }

}

void GeminiAssemblerWindow::on_pushButton_convert_clicked( )
{
    if (!this->control->convert_assembly_to_bytecode())
    {
        this->alert_assembly_conversion_failed();
        return;
    }
    if (!this->control->write_byte_code_to_file())
    {
        this->alert_write_byte_code_to_file_failed();
        return;
    }

    //  Tell the world we are done.
    QMessageBox *mb = new QMessageBox(this);
    mb->setText("Conversion finished!");
    mb->show();


}
