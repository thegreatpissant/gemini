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

#ifndef INVALID_FILE_DIALOG_H
#define INVALID_FILE_DIALOG_H

#include <QDialog>

#include "gemini_types.h"


namespace Ui {
class Invalid_file_dialog;
}

class Invalid_file_dialog : public QDialog
{
    Q_OBJECT
private:
    Source_code source_code;
    Error_lines error_lines;

public:
    explicit Invalid_file_dialog(QWidget *parent = 0);
    ~Invalid_file_dialog();

    void set_source_code ( Source_code sc );

    void set_error_list  ( Error_lines el);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Invalid_file_dialog *ui;
};

#endif // INVALID_FILE_DIALOG_H
