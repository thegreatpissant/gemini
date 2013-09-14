#include "gemini.h"
#include "ui_gemini.h"

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
