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

/*
 * Invalid_file_dialog: show the parsing errors of the files ways.
 */

#include "invalid_file_dialog.h"
#include "ui_invalid_file_dialog.h"

//  Constructor
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

//  Setup the source code to display
void Invalid_file_dialog::set_source_code(Source_code sc)
{
    source_code = sc;
}

void Invalid_file_dialog::Generate_error_list_text ()
{
    //  QT supports html markup so lets introduce some red lines for
    //  showing off those wonderfull errors.
    ui->textEdit->toHtml();
    ui->textEdit->insertHtml("<b> Errors Found in file!! See <font color=red>RED</font> lines</b><br>");

    auto eline = error_lines.begin();
    for (std::size_t line = 0; line < source_code.size(); line++ )
    {
        QString formated_line =
                QString::number(line+1) + ":  " +
                QString::fromStdString(source_code[line]).toHtmlEscaped();

        if ( *eline == line )
        {
            formated_line = "<font color=red><b>" + formated_line + "</b></font>";
            eline++;
        }
        formated_line += " <br>";
        ui->textEdit->insertHtml(formated_line);
    }
}

void Invalid_file_dialog::set_error_list(Error_lines el)
{
    error_lines = el;
    Generate_error_list_text();
}

void Invalid_file_dialog::on_pushButton_clicked()
{
    this->close();
}
