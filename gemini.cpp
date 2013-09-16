/*
 *  James Feister
 *  cisc360 Gemini project
 */

#include "gemini.h"
#include "ui_gemini.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

gemini::gemini(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gemini)
{
    ui->setupUi(this);
}

gemini::~gemini()
{
    delete ui;
}

void gemini::on_actionQuit_triggered()
{
    qApp->quit();
}

void gemini::on_actionLoad_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(
                this,
                tr("Open File"), QString (), tr ("Gemini Files (*.s)"));

    if ( !file_name.isEmpty() ) {
        QFile file( file_name);
        if ( !file.open( QIODevice::ReadOnly) ) {
            QMessageBox::critical (this, tr("Error"), tr("Could not open file") );
            return;
        }
        QTextStream in (&file);
//        ui->textEdit->setText(in.readAll());
        file.close();
    }
}
