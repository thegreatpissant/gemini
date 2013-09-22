/*
 *  James Feister
 *  cisc360 Gemini project
 */

#include "gemini.h"
#include "gemini_system.h"
#include <QApplication>

Gemini_system gemini_system;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gemini w;
    w.show();

    return a.exec();
}
