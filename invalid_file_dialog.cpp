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
 */
#include "invalid_file_dialog.h"
#include "ui_invalid_file_dialog.h"

Invalid_file_dialog::Invalid_file_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Invalid_file_dialog)
{
    ui->setupUi(this);
}

Invalid_file_dialog::~Invalid_file_dialog()
{
    delete ui;
}

void Invalid_file_dialog::set_source_code(Source_code sc)
{
    source_code = sc;
}


void Invalid_file_dialog::set_error_list(Error_lines el)
{
    error_lines = el;
    ui->textEdit->toHtml();
    ui->textEdit->insertHtml("<b> Errors Found in file!! See <font color=red>RED</font> lines</b><br>");

    auto eline = el.begin();
    for (std::size_t line = 0; line < source_code.size(); line++ )
    {
        QString formated_line =
                QString::number(line+1) + ":  " +
                QString::fromStdString(source_code[line]) +
                "<br>";
        if ( *eline == line )
        {
            formated_line = "<font color=red><b>" + formated_line + "</b></font>";
            eline++;
        }
        ui->textEdit->insertHtml(formated_line);
    }
}

void Invalid_file_dialog::on_pushButton_clicked()
{
    this->close();
}
