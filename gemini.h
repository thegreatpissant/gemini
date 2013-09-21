/*
 *  James Feister
 *  cisc360 Gemini project
 */

#ifndef GEMINI_H
#define GEMINI_H

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

private slots:
    void on_actionQuit_triggered();

    void on_actionLoad_triggered();

    void on_pushButton_clicked();

private:
    Ui::gemini *ui;
};

#endif // GEMINI_H
