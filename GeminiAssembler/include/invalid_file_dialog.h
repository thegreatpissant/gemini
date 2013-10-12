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

#ifndef INVALID_FILE_DIALOG_H
#define INVALID_FILE_DIALOG_H

#include <QDialog>

#include "gemini_types.h"
#include <memory>

/*
 * Invalid_file_dialog: show the parsing errors of the files ways.
 */

namespace Ui {
class Invalid_file_dialog;
}

class Invalid_file_dialog : public QDialog
{
    Q_OBJECT
private:
    Source_code source_code;
    Error_lines error_lines;

    void Generate_error_list_text();
public:
    explicit Invalid_file_dialog(QWidget *parent = 0);
    ~Invalid_file_dialog();

    //  Initialize the source code to display against
    void set_source_code ( Source_code sc );
    void set_error_list  ( Error_lines el);

private slots:
    void on_pushButton_clicked(); // Close dialog box button

private:
    Ui::Invalid_file_dialog *ui;  //  What we are displaying in
};

#endif // INVALID_FILE_DIALOG_H
