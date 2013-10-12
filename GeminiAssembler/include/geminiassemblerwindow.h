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

#ifndef GEMINIASSEMBLERWINDOW_H
#define GEMINIASSEMBLERWINDOW_H

#include <QMainWindow>

#include <memory>

#include "geminiassemblermodel.h"
#include "geminiassemblercontrol.h"
class GeminiAssemblerControl;



namespace Ui {
class GeminiAssemblerWindow;
}

class GeminiAssemblerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GeminiAssemblerWindow(QWidget *parent = 0);
    ~GeminiAssemblerWindow();
    
private slots:
    void on_pushButton_asmfile_clicked();

    void on_pushButton_convert_clicked();

    void on_pushButton_bytecode_file_clicked();

private:
    Ui::GeminiAssemblerWindow *ui;

    GeminiAssemblerModel *model;
    GeminiAssemblerControl *control;

    const QString default_assembly_button_text = "Click to load assembly file";
    const QString default_bytecode_button_text = "Click to load bytecode file";

public:
    void setModel (GeminiAssemblerModel *g);
    void setControl (GeminiAssemblerControl *c);

    void alert_assembly_file_open_failed ();
    void alert_bytecode_file_open_failed ();
    void alert_assembly_conversion_failed ();
    void alert_assembly_source_invalid();
    void alert_write_byte_code_to_file_failed();
};

#endif // GEMINIASSEMBLERWINDOW_H
