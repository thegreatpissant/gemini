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
    QString file_name = QFileDialog::getOpenFileName(
                this,
                tr("Open File"), QString (), tr ("Gemini Files (*.s)"));

    if ( !file_name.isEmpty() )
    {
        QFile file( file_name );
        if ( !file.open( QIODevice::ReadOnly) ) {
            QMessageBox::critical (this, tr("Error"), tr("Could not open file") );
            return;
        }
        QTextStream in (&file);
        Source_code source_code;
        QTextStream in_stream (&file);
        while (!in_stream.atEnd())
        {
            source_code.push_back ( in_stream.readLine().toStdString() );
        }
        file.close();

        //  Validate file
        Error_lines error_lines = validate_source (source_code);
        if (! error_lines.empty() )
        {
            Invalid_file_dialog * ifd  = new Invalid_file_dialog(this);
            ifd->set_source_code( source_code );
            ifd->set_error_list( error_lines );
            ifd->show();
        }
        else
            return;

    }
}

void gemini::on_pushButton_clicked()
{


}
