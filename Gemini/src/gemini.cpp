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

#include "gemini.h"
#include "ui_gemini.h"
#include "gemini_types.h"
#include "gemini_system.h"

#include <stdexcept>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>

#include <fstream>

extern Gemini_system gemini_system;

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

/*
 * Grabs the cpu registers from the gemini system
 * Converts them into human readable form
 * Sets the GUI lables appropriatly for coresponding register
 */
void gemini::gemini_display_callback()
{
    Gemini_system_info gemini_system_info = gemini_system.get_system_info();
    //  Set all the registers
   ui->reg_A    ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.A)));
   ui->reg_B    ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.B)));
   ui->reg_Acc  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.Acc )));
   ui->reg_Zero ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.Zero )));
   ui->reg_One  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.One )));
   ui->reg_PC   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.PC )));
   ui->reg_MAR  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.MAR )));
   ui->reg_MDR  ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.MDR )));
   ui->reg_TEMP ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.TEMP )));
   ui->reg_IR   ->setText( QString::fromStdString( gemini_instruction_register_value_to_std_string(gemini_system_info.IR )));
   ui->inst_label_value ->setText( QString::fromStdString( gemini_instruction_register_to_std_string(gemini_system_info.IR )));
   ui->reg_CC   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.CC )));
   ui->reg_CE   ->setText( QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.CE )));
   ui->jmp_stack_depth_label_value ->setText( (QString::fromStdString( gemini_register_value_to_std_string(gemini_system_info.jmp_stack_depth))));

    //  Set the Instruction index label
   ui->inst_label_index ->setText( QString::fromStdString(gemini_register_value_to_std_string(gemini_system_info.instruction_index)));
}


/*
 * Close the application
 */
void gemini::on_actionQuit_triggered()
{
    qApp->quit();
}


void gemini::on_actionLoad_triggered()
{
    ui->pushButton->setEnabled(false);
    QString file_name = QFileDialog::getOpenFileName(
                this,
                tr("Open File"), QString (), tr ("Gemini Files (*.*)"));

    if ( file_name.isEmpty() )
        return;

    std::ifstream file( file_name.toStdString(), std::ios::binary);
    if ( !file ) {
        QMessageBox::critical (this, tr("Error"), tr("Could not open bytecode file") );
        return;
    }

    std::shared_ptr<Byte_code> byte_code = std::shared_ptr<Byte_code> (new Byte_code);
    Byte_code_segment bcs;
    while (file.read((char*)&bcs, sizeof(Byte_code_segment)))
    {
        (*byte_code).push_back(bcs);
    }

    file.close();


    //  Send pseudo Byte_code to gemini system
    gemini_system.load_byte_code(byte_code);

    //  Set CPU to ready
    gemini_system.power_on();

    //  Enable the 'next' button
    ui->pushButton->setEnabled(true);

    //  Update the display to show the current cpu state
    this->gemini_display_callback();
}

/*
 * User invokes a clock event sent to the CPU
 */
void gemini::on_pushButton_clicked()
{
    //  Make sure to check for a buffer overflow
    try {
        gemini_system.cycle_clock();
        this->gemini_display_callback();
    }
    catch (std::out_of_range excp) {
        QMessageBox *mb = new QMessageBox(this);
        mb->setText(QString::fromStdString(excp.what()));
        mb->show();  // Display there was a buffer overflow
        this->gemini_display_callback();
        //  invoke an error state
        set_cpu_error ();
        //  shutdown the system
        gemini_system.power_off();
    }
}

/*
 * On error disable further user input
 */
void gemini::set_cpu_error ()
{
    ui->pushButton->setEnabled(false);
}
