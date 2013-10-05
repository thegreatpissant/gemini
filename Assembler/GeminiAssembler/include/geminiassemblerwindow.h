#ifndef GEMINIASSEMBLERWINDOW_H
#define GEMINIASSEMBLERWINDOW_H

#include <QMainWindow>

#include <memory>

#include "geminiassemblermodel.h"
#include "geminiassemblercontrol.h"
class GeminiAssemblerControl;



namespace Ui {
class GeminiAssemblerWindow;
}

class GeminiAssemblerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GeminiAssemblerWindow(QWidget *parent = 0);
    ~GeminiAssemblerWindow();
    
private slots:
    void on_pushButton_asmfile_clicked();

    void on_pushButton_convert_clicked();

    void on_pushButton_bytecode_file_clicked();

private:
    Ui::GeminiAssemblerWindow *ui;

    GeminiAssemblerModel *model;
    GeminiAssemblerControl *control;

    const QString default_assembly_button_text = "Click to load assembly file";
    const QString default_bytecode_button_text = "Click to load bytecode file";

public:
    void setModel (GeminiAssemblerModel *g);
    void setControl (GeminiAssemblerControl *c);

    void alert_assembly_file_open_failed ();
    void alert_bytecode_file_open_failed ();
    void alert_assembly_conversion_failed ();
    void alert_assembly_source_invalid();
};

#endif // GEMINIASSEMBLERWINDOW_H
