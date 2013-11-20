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
 * *
 * Project 2: Gemini Enhancements, Implement the following
 * - Gemini Assembler: Create bytecode files from our Gemini assembly files
 * - Support Loading of binary files into the Gemini Simulator
 * - Add >=, <= Condition Jump checks
 * - Add JMP and RET commands for functions
 * - Add Cache implementation for One block Direct and 2 Way Set
 * - Extra credit: SETHI and SETLO instruction to handle 32 bit numbers
 * - Extra credit: Overflow on Multiplication and Divide
 * - Extra credit: JMP and RET instructions to support 25 levels of recursion
 * - Extra credit: Allow cache to support 4 memory block grabing at a time
 * *
 * Project 3: Gemini Pipelining, Implement the following
 * - Part 1: A pipelining architecture within the CPU
 *   Consists of [Fetch|Decode|eXecute|Memory] stages.
 *   Seperate each stage into its own thread
 * - Part 2: Branch prediction;
 *   Based on past results predict future branching
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
#include <QStringList>
#include <QInputDialog>
#include <QTimer>

#include <fstream>

extern Gemini_system gemini_system;

gemini::gemini( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::gemini )
{
    ui->setupUi( this );
    run_all_timer = new QTimer(this);
    connect(run_all_timer, SIGNAL(timeout()), this, SLOT(on_pushButton_clicked()));
}

gemini::~gemini( )
{
    delete ui;
}


/*
 * Close the application
 */
void gemini::on_actionQuit_triggered( )
{
    qApp->quit( );
}

void gemini::on_actionLoad_triggered( )
{
    this->enable_user_interaction( false );
    QString file_name = QFileDialog::getOpenFileName( this, tr( "Open File" ), QString( ),
                                                      tr( "Gemini Files (*.*)" ) );

    if ( file_name.isEmpty( ) )
        return;

    this->ui->label_file_name ->setText(file_name);

    std::ifstream file( file_name.toStdString( ), std::ios::binary );
    if ( !file )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Could not open bytecode file" ) );
        return;
    }

    std::shared_ptr<Byte_code> byte_code = std::shared_ptr<Byte_code>( new Byte_code );
    Byte_code_segment bcs;
    while ( file.read( (char *)&bcs, sizeof( Byte_code_segment ) ) )
    {
        ( *byte_code ).push_back( bcs );
    }

    file.close( );

    //  Send pseudo Byte_code to gemini system
    gemini_system.load_byte_code( byte_code );

    //  Promp user to select a cache type
    QString dob = "Direct One Block", dfb = "Direct Four Block", sob = "Two Way Set One Block",
            sfb = "Two Way Set Four Block";
    QString sact = "Select a Cache Type";
    QStringList list;
    list << dob << dfb << sob << sfb;
    QString text = QInputDialog::getItem( this, sact, sact, list, 0, false );

    //  Set the cache mode
    if ( text == dob )
    {
        gemini_system.set_cache_type( Cache_type::DIRECT_ONEBLOCK );
        ui->cache_mode_label_value->setText( QString::fromStdString(
            gemini_cache_type_to_std_string( Cache_type::DIRECT_ONEBLOCK ) ) );
    }
    else if ( text == dfb )
    {
        gemini_system.set_cache_type( Cache_type::DIRECT_FOURBLOCK );
        ui->cache_mode_label_value->setText( QString::fromStdString(
            gemini_cache_type_to_std_string( Cache_type::DIRECT_FOURBLOCK ) ) );
    }
    else if ( text == sob )
    {
        gemini_system.set_cache_type( Cache_type::TWOWAYSET_ONEBLOCK );
        ui->cache_mode_label_value->setText( QString::fromStdString(
            gemini_cache_type_to_std_string( Cache_type::TWOWAYSET_ONEBLOCK ) ) );
    }
    else if ( text == sfb )
    {
        gemini_system.set_cache_type( Cache_type::TWOWAYSET_FOURBLOCK );
        ui->cache_mode_label_value->setText( QString::fromStdString(
            gemini_cache_type_to_std_string( Cache_type::TWOWAYSET_FOURBLOCK) ) );
    }

    //  Prompt for branch prediction
    QString ebp = "Enable Branch Prediction", dbp = "Disable Branch Prediction";
    sact = "Select a Branch Prediction Option";
     QStringList bplist;
     bplist << ebp << dbp;
    text = QInputDialog::getItem( this, sact, sact, bplist, 0, false );

    //  Set the cache mode
    if ( text == ebp )
    {
        gemini_system.set_branch_prediction(true);
        ui->branch_prediction->setText( "Enabled" );
    } else {
        gemini_system.set_branch_prediction(false);
        ui->branch_prediction->setText( "Disabled");
    }
    //  Set CPU to ready
    gemini_system.power_on( );

    //  Enable the 'next' button
    this->enable_user_interaction( true );
}

/*
 * User invokes a clock event sent to the CPU
 */
void gemini::on_pushButton_clicked( )
{
    //  Make sure to check for a buffer overflow
    try
    {
        gemini_system.cycle_clock( );
        if (gemini_system.done()) {
            run_all_timer->stop();
            this->enable_user_interaction( false );
        }
    }
    catch ( std::out_of_range excp )
    {
        QMessageBox *mb = new QMessageBox( this );
        mb->setText( QString::fromStdString( excp.what( ) ) );
        mb->show( ); // Display there was a buffer overflow
        //  invoke an error state
        set_cpu_error( );
        //  shutdown the system
        gemini_system.power_off( );
    }
}

/*
 * On error disable further user input
 */
void gemini::set_cpu_error( )
{
    this->enable_user_interaction( false );
}

void gemini::on_action_sel_triggered( )
{
}

void gemini::on_pushButton_runall_clicked()
{
    run_all_timer->start(250);
    return;
}

void gemini::enable_user_interaction( bool e)
{
    ui->pushButton->setEnabled(e);
    ui->pushButton_runall->setEnabled(e);
}

void gemini::show_fetch_done(fetch_signal_ptr fsi)
{
    ui->reg_PC->setText(
                QString::fromStdString( gemini_register_value_to_std_string( fsi->PC ) ) );

}

void gemini::show_decode_done(decode_signal_ptr dsi)
{
    ui->reg_IR->setText(
                QString::fromStdString( gemini_instruction_register_value_to_std_string( dsi->IR ) ) );
}

void gemini::show_execute_done(execute_signal_ptr esi)
{
    ui->reg_A->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->A ) ) );
    ui->reg_B->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->B ) ) );
    ui->reg_Acc->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->Acc ) ) );
    ui->reg_Zero->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->Zero ) ) );
    ui->reg_One->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->One ) ) );
    ui->reg_MAR->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->MAR ) ) );
    ui->reg_MDR->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->MDR ) ) );
    ui->reg_TEMP->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->TEMP ) ) );
    ui->reg_SL0->setText(
                QString::fromStdString( gemini_instruction_register_value_to_std_string(esi->SL0)));
    ui->reg_SL1->setText(
                QString::fromStdString( gemini_instruction_register_value_to_std_string(esi->SL1)));
    ui->reg_CC->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->CC ) ) );
    ui->reg_CE->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->CE ) ) );
    ui->reg_OVF->setText(
                QString::fromStdString( gemini_register_value_to_std_string( esi->OVF ) ) );
    //  Set Stack information
    ui->jmp_stack_depth_label_value->setText( ( QString::fromStdString(
                                                    gemini_register_value_to_std_string( esi->jmp_stack_depth ) ) ) );
}

void gemini::show_store_done(store_signal_ptr ssi)
{
    ui->cache_hits_label_value->setText(
                QString::fromStdString( std::to_string( ssi->cache_hits ) ) );
    ui->cache_misses_label_value->setText(
                QString::fromStdString( std::to_string( ssi->cache_misses ) ) );
}

void gemini::show_pipeline_stats(pipeline_stats_ptr psp)
{
    ui->clock_count_value->setText(
                QString::fromStdString(std::to_string( psp->clock_count)));
    ui->decode_count->setText(
                QString::fromStdString( std::to_string( psp->decode_count)));
    ui->decode_halt_count->setText(
                QString::fromStdString( std::to_string( psp->decode_halt_count)));
    ui->execute_count->setText(
                QString::fromStdString( std::to_string( psp->execute_count)));
    ui->execute_halt_count->setText(
                QString::fromStdString( std::to_string( psp->execute_halt_count)));
    ui->fetch_count->setText(
                QString::fromStdString( std::to_string( psp->fetch_count)));
    ui->fetch_halt_count->setText(
                QString::fromStdString( std::to_string( psp->fetch_halt_count)));
    ui->store_count->setText(
                QString::fromStdString( std::to_string( psp->store_count)));
    ui->fetch_null_count->setText(
                QString::fromStdString( std::to_string( psp->fetch_null_count)));
    ui->decode_null_count->setText(
                QString::fromStdString( std::to_string( psp->decode_null_count)));
    ui->execute_null_count->setText(
                QString::fromStdString( std::to_string( psp->execute_null_count)));
    ui->store_null_count->setText(
                QString::fromStdString( std::to_string( psp->store_null_count)));
    ui->branching_weight->setText(
                QString::fromStdString( std::to_string( psp->branching_weight )));
}

void gemini::show_cpu_error(QString error_string)
{
    //  invoke an error state
    set_cpu_error( );
    QMessageBox *mb = new QMessageBox( this );
    mb->setText( error_string );
    mb->show( ); // Display there was a buffer overflow
}

