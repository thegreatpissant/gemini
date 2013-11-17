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
 */

#ifndef CPU_H
#define CPU_H

#include "gemini_types.h"
#include "memory.h"

#include <memory>
#include <stack>

#include "QObject"
#include "QThread"
#include "QMutex"
#include "QWaitCondition"

/*
 *  Gemini system CPU: Performs operations and generates clock signals.
 *
 */
class gemini;
class Fetch_thread;
class Decode_thread;
class Execute_thread;
class Store_thread;

class CPU : public QObject
{
        Q_OBJECT
private:
    void execute_instruction();  //  Should end up as a function pointer that we use if powered on
    Register_value get_value(Instruction_register ir);
    Gemini_op_type get_op(Instruction_register ir);
    Gemini_access_type get_access_type(Instruction_register ir);

public:
    CPU();

    //  Memory from the gemini_system
    std::shared_ptr<Memory> memory;

    //  All gemini application bytecode stored here.
    std::shared_ptr<Byte_code> byte_code;

    //  onboard memory, "Registers" Did not see any problem with them
    //  being public.
    Register_value OP_RESULT;
    Register_value A;
    Register_value B;
    Register_value Acc;
    Register_value Zero;
    Register_value One;
    u_int16_t PC;
    Register_value MAR;
    Register_value MDR;
    Register_value TEMP;

    //  32 bit registers
    int32_t SL1;
    int32_t SL0;
    int32_t mull;
    int32_t divl;

    //  Condition Code
    Register_value CC;
    Register_value CE;

    //  Overflow checking
    Register_value OVF;

    //  Jump Stack
    std::stack <Register_value> jmp_stack;
    const int JMP_STACK_MAX_DEPTH = 25;
    int jmp_stack_depth;

    //  External action to initiate a clock tick
    void tick();

    //  Bring in bytecode to the CPU
    void load_byte_code ( std::shared_ptr<Byte_code> bc );
    
    //  Set internal handle to main memory section
    void set_memory (std::shared_ptr<Memory> m);

    //  Set the cache mode
    void set_cache_mode ( Cache_type cache_type );

    //  bring CPU to starting state
    void initialize ();

    //  Halt the cpu
    void halt();

    //  Has the program finished
    bool done();

    //  ==  PIPELINING  ==
    //  Thread vars
    int running_count = 0;
    QMutex mutex_running_count;
    QWaitCondition running_condition;
    bool fetch_stalled = false;
    QMutex mutex_fetch_stalled;
    bool decode_stalled = false;
    QMutex mutex_decode_stalled;
    bool execute_stalled = false;
    QMutex mutex_execute_stalled;
    QMutex mutex_fetch_temp_state;
    QMutex mutex_decode_temp_state;
    QMutex mutex_execute_temp_state;
    QMutex mutex_store_temp_state;
    bool fetch_temp_state_processed = false;
    bool decode_temp_state_processed = false;
    bool execute_temp_state_processed = false;
    bool store_temp_state_processed = false;



    //  States
    std::size_t instruction_count;
    //  Branch Prediction
    int branch_ways {1};
    //  Fetch State Vars
    struct Fetch_state {
        Instruction_register IR;
    };
    //  Decode State Vars
    struct Decode_state {
        Instruction_register decode_IR;
    };
    //  Execute State vars
    struct Execute_state {
        Value execute_value;
        Gemini_op execute_op;
        Gemini_access_type access_type;
    };
    //  Store State Vars
    struct Store_state {
        Register_value store_Acc;
        Gemini_op store_op;
        Value store_value;
    };
    Fetch_thread *fetch_thread;
    Decode_thread *decode_thread;
    Execute_thread *execute_thread;
    Store_thread *store_thread;

    //  Stage States
    std::shared_ptr<Fetch_state> fetch_temp_state;
    std::shared_ptr<Decode_state> decode_temp_state;
    std::shared_ptr<Execute_state> execute_temp_state;
    std::shared_ptr<Store_state> store_temp_state;
    //  Set the view
private:
    gemini * gemini_view;
public:
    void setView (gemini* view);
    void fetch();
    void decode();
    void execute();
    void store();
signals:
    void fetch_done (std::shared_ptr<fetch_signal_info>);
    void decode_done (std::shared_ptr<decode_signal_info>);
    void execute_done (std::shared_ptr<execute_signal_info>);
    void store_done (std::shared_ptr<store_signal_info>);
    //  ==  PIPELINING END  ==
};

class Fetch_thread :public QThread
{
    Q_OBJECT
    CPU *cpu;
public:
    void run() Q_DECL_OVERRIDE {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        cpu->fetch( );
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }
    Fetch_thread ( CPU *c ):cpu{c} {}
};
class Decode_thread :public QThread
{
    Q_OBJECT
    CPU *cpu;
public:
    void run() Q_DECL_OVERRIDE {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        cpu->decode( );
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }
    Decode_thread ( CPU *c ):cpu{c} {}
};
class Execute_thread :public QThread
{
    Q_OBJECT
    CPU *cpu;
public:
    void run() Q_DECL_OVERRIDE {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        cpu->execute( );
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }
    Execute_thread ( CPU *c ):cpu{c} {}
};
class Store_thread :public QThread
{
    Q_OBJECT
    CPU *cpu;
public:
    void run() Q_DECL_OVERRIDE {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        cpu->store( );
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }
    Store_thread ( CPU *c ):cpu{c} {}
};



#endif // CPU_H
