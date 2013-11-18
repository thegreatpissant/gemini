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
 *
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
 *  Gemini application gui
 *
 *  Handles all events related to the gui and code.
 *  Some parts may require a better seperation in the MVC sense
 */

#ifndef GEMINI_H
#define GEMINI_H

#include "gemini_types.h"
#include "gemini_system.h"

#include <QMainWindow>

namespace Ui {
class gemini;
}

class gemini : public QMainWindow
{
    Q_OBJECT

public:
    explicit gemini(QWidget *parent = 0);
    ~gemini();

    void set_cpu_error();

private slots:
    //  Terminate the application
    void on_actionQuit_triggered();

  //  Loading a gemini asm file
    void on_actionLoad_triggered();

  //  Handles the user initializing a clock cycle
    void on_pushButton_clicked();

    void on_action_sel_triggered();

    void on_pushButton_runall_clicked();

    void enable_user_interaction (bool e);

public slots:
    void show_fetch_done (fetch_signal_ptr fsi);
    void show_decode_done (decode_signal_ptr dsi);
    void show_execute_done (execute_signal_ptr esi);
    void show_store_done (store_signal_ptr ssi);
    void show_pipeline_stats (pipeline_stats_ptr psp);

private:
    Ui::gemini *ui;
};

#endif // GEMINI_H
