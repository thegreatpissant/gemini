#ifndef INVALID_FILE_DIALOG_H
#define INVALID_FILE_DIALOG_H

#include <QDialog>

#include "gemini_types.h"


namespace Ui {
class Invalid_file_dialog;
}

class Invalid_file_dialog : public QDialog
{
    Q_OBJECT
private:
    Source_code source_code;
    Error_lines error_lines;

public:
    explicit Invalid_file_dialog(QWidget *parent = 0);
    ~Invalid_file_dialog();

    void set_source_code ( Source_code sc );

    void set_error_list  ( Error_lines el);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Invalid_file_dialog *ui;
};

#endif // INVALID_FILE_DIALOG_H
