#include "invalid_file_dialog.h"
#include "ui_invalid_file_dialog.h"

Invalid_file_dialog::Invalid_file_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Invalid_file_dialog)
{
    ui->setupUi(this);
}

Invalid_file_dialog::~Invalid_file_dialog()
{
    delete ui;
}

void Invalid_file_dialog::set_source_code(Source_code sc)
{
    source_code = sc;
}


void Invalid_file_dialog::set_error_list(Error_lines el)
{
    error_lines = el;
    ui->textEdit->toHtml();
    ui->textEdit->insertHtml("<b> Errors Found in file!! See <font color=red>RED</font> lines</b><br>");

    auto eline = el.begin();
    for (auto line = 0; line < source_code.size(); line++ )
    {
        QString formated_line =
                QString::number(line+1) + ":  " +
                QString::fromStdString(source_code[line]) +
                "<br>";
        if ( *eline == line )
        {
            formated_line = "<font color=red><b>" + formated_line + "</b></font>";
            eline++;
        }
        ui->textEdit->insertHtml(formated_line);
    }
}

void Invalid_file_dialog::on_pushButton_clicked()
{
    this->close();
}
