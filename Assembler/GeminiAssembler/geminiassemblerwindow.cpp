#include "geminiassemblerwindow.h"
#include "ui_geminiassemblerwindow.h"

#include "../../invalid_file_dialog.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include "../../gemini_types.h"

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

void GeminiAssemblerWindow::setModel( std::shared_ptr<GeminiAssemblerModel> g )
{
    this->model = g ;
}

void GeminiAssemblerWindow::setControl(std::shared_ptr<GeminiAssemblerControl> c)
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
    Invalid_file_dialog * ifd  = new Invalid_file_dialog(this);
    ifd->set_source_code( model->get_source_code() );
    ifd->set_error_list( model->get_error_lines() );
    ifd->show();
    return;

}


void GeminiAssemblerWindow::on_pushButton_asmfile_clicked( )
{
    QString file_name = QFileDialog::getOpenFileName( this, tr( "Open File" ), QString( ),
                                                      tr( "Gemini Files (*.*)" ) );

    model->set_assembly_file_name( file_name.toStdString( ) );

    this->ui->pushButton_asmfile->setText(file_name);

    if (model->bytecode_file_name_is_set())
    {
        this->ui->pushButton_convert->setEnabled(true);
    }
}

void GeminiAssemblerWindow::on_pushButton_bytecode_file_clicked( )
{
    QString file_name = QFileDialog::getOpenFileName( this, tr( "Open File" ), QString( ),
                                                      tr( "Gemini Files (*.*)" ) );

    model->set_bytecode_file_name( file_name.toStdString( ) );

    this->ui->pushButton_bytecode_file->setText(file_name);

    if (model->assembly_file_name_is_set())
    {
        this->ui->pushButton_convert->setEnabled(true);
    }

}

void GeminiAssemblerWindow::on_pushButton_convert_clicked( )
{
    if (!this->control->test_assembly_file())
    {
        this->alert_assembly_file_open_failed();
    }
    if (!this->control->test_bytecode_file())
    {
        this->alert_bytecode_file_open_failed();
    }
    if (!this->control->convert_assembly_to_bytecode())
    {
        this->alert_assembly_conversion_failed();
    }

}
