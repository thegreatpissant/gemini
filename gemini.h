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

#ifndef GEMINI_H
#define GEMINI_H

#include <QMainWindow>
#include "gemini_types.h"
#include "gemini_system.h"

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
    void on_actionQuit_triggered();

    void on_actionLoad_triggered();

    void on_pushButton_clicked();

private:
    Ui::gemini *ui;
};

#endif // GEMINI_H
