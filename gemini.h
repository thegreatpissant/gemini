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
    void gemini_display_callback ();

    void set_cpu_error();

private slots:
    //  Terminate the application
    void on_actionQuit_triggered();

  //  Loading a gemini asm file
    void on_actionLoad_triggered();

  //  Handles the user initializing a clock cycle
    void on_pushButton_clicked();

private:
    Ui::gemini *ui;
};

#endif // GEMINI_H
