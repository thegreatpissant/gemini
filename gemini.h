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

private:
    Ui::gemini *ui;
};

#endif // GEMINI_H
