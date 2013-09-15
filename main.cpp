/*
 *  James Feister
 *  cisc360 Gemini project
 */

#include "gemini.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gemini w;
    w.show();

    return a.exec();
}
