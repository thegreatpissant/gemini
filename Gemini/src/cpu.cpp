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

/*
 *  Gemini system CPU: Performs operations.
 *
 */

#include "cpu.h"
#include "gemini_system.h"

#include <QThread>

extern Gemini_system gemini_system;

CPU::CPU( )
{
    fetch_thread = NULL;
    decode_thread = NULL;
    execute_thread = NULL;
    store_thread = NULL;
}

void CPU::tick( )
{
    //  cpu clock
    execute_instruction( );
}

//  Master control for thread continue
void CPU::execute_instruction( )
{
    mutex_running_count.lock ();
    while (running_count > 0)
    {
        mutex_running_count.unlock();
        QThread::msleep(100);
        mutex_running_count.lock();
    }
    pipeline_stats_ptr psp = std::shared_ptr<pipeline_stats_info>(new pipeline_stats_info);
    psp->decode_count = this->decode_count;
    psp->decode_halt_count = this->decode_halt_count;
    psp->execute_count = this->execute_count;
    psp->execute_halt_count = this->execute_halt_count;
    psp->fetch_count = this->fetch_count;
    psp->fetch_halt_count = this->fetch_halt_count;
    psp->store_count = this->store_count;
    psp->fetch_null_count = this->fetch_null_count;
    psp->decode_null_count = this->decode_null_count;
    psp->execute_null_count = this->execute_null_count;
    psp->store_null_count = this->store_null_count;
    psp->branching_weight = this->branch_ways;

    fetch_stage_done = false;
    decode_stage_done = false;

    if( jump_stack_overflow ) {
        gemini_system.power_off();
        emit cpu_error(("Jump Stack overflow/underflow"));
    }
    if( memory_access_overflow ) {
        gemini_system.power_off();
        emit cpu_error(("Memory Access Violation"));
    }
    if( sethi_access_violation ) {
        gemini_system.power_off();
        emit cpu_error(("SETHI register access violation"));
    }
    if( setlo_access_violation ) {
        gemini_system.power_off();
        emit cpu_error(("SETLO register access violation"));
    }
    running_condition.wakeAll();
    mutex_running_count.unlock();
    clock_count++;
    psp->clock_count = this->clock_count;
    emit pipeline_stats(psp);
}


Register_value get_value( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x0000FFFF;
    return ( (Register_value)tmp );
}

Gemini_op_type get_op( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0xFF000000;
    tmp >>= 24;
    return ( (Gemini_op_type)tmp );
}

Gemini_access_type get_access_type( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x00FF0000;
    tmp >>= 16;
    return ( (Gemini_access_type) tmp );
}

void CPU::initialize( )
{
    //  @@TODO fix multiple runs and threads
    bool delay_start = false;
    if ( fetch_thread == NULL ) {
        delay_start = true;
        //  Create thread objects
        fetch_thread = std::shared_ptr<QThread>(new QThread);
        decode_thread = std::shared_ptr<QThread>(new QThread);
        execute_thread = std::shared_ptr<QThread>(new QThread);
        store_thread = std::shared_ptr<QThread>(new QThread);
        //  Create Worker Objects
        fetch_worker = std::shared_ptr<Fetch_worker> (new Fetch_worker (this));
        decode_worker = std::shared_ptr<Decode_worker>  (new Decode_worker (this));
        execute_worker = std::shared_ptr<Execute_worker> (new Execute_worker (this));
        store_worker = std::shared_ptr<Store_worker> (new Store_worker (this));
        fetch_worker->setObjectName("FETCH_THREAD");
        decode_worker->setObjectName("DECODE_THREAD");
        execute_worker->setObjectName("EXECUTE_THREAD");
        store_worker->setObjectName("STORE_THREAD");
        this->connect_slots();
        //  Dispatch workers to their threads
        fetch_worker->moveToThread(&(*fetch_thread));
        decode_worker->moveToThread(&(*decode_thread));
        execute_worker->moveToThread(&(*execute_thread));
        store_worker->moveToThread(&(*store_thread));
        //  Start thread event loops
        fetch_thread->start();
        decode_thread->start();
        execute_thread->start();
        store_thread->start();
    }
    //  Set mutex states All are held by the CPU
    Zero = 0;
    One = 1;
    PC = 0;
    jmp_stack_depth = 0;
    branch_ways = 1;
    //  Set initial thread states All are held by the CPU
    fetch_temp_state = std::shared_ptr<Fetch_state> (new Fetch_state );
    fetch_temp_state->PC = PC;
    decode_temp_state = NULL;
    execute_temp_state = NULL;
    store_temp_state = NULL;
    fetch_stalled = false;
    decode_stalled = false;
    execute_stalled = false;
    fetch_temp_state_processed = false;
    decode_temp_state_processed = false;
    execute_temp_state_processed = false;
    store_temp_state_processed = false;
    fetch_stage_done = false;
    decode_stage_done = false;
    running_count = 0;
    clock_count = 0;
    mutex_running_count.unlock();
    mutex_fetch_stalled.unlock();
    mutex_decode_stalled.unlock();
    mutex_execute_stalled.unlock();
    mutex_fetch_temp_state.unlock();
    mutex_decode_temp_state.unlock();
    mutex_execute_temp_state.unlock();
    mutex_store_temp_state.unlock();
    mutex_fetch_stage_done.unlock();
    mutex_decode_stage_done.unlock();
    fetch_count = 0;
    decode_count = 0;
    execute_count = 0;
    store_count = 0;
    fetch_halt_count = 0;
    decode_halt_count = 0;
    execute_halt_count = 0;
    fetch_null_count = 0;
    decode_null_count = 0;
    execute_null_count = 0;
    store_null_count = 0;
    //  Send thread run event
    if (delay_start) {
        running_count = 4;
        emit start_threads();
    }
    //  Error conditions
    jump_stack_overflow = false;
    memory_access_overflow = false;
    sethi_access_violation = false;
    setlo_access_violation = false;

}

void CPU::halt( )
{
    fetch_temp_state = NULL;
    decode_temp_state = NULL;
    execute_temp_state = NULL;
    store_temp_state = NULL;
}

bool CPU::done()
{
    return ( fetch_temp_state == NULL && decode_temp_state == NULL
             && execute_temp_state == NULL && store_temp_state == NULL);
}

void CPU::setView(gemini *view)
{
    this->gemini_view = view;
}
void CPU::connect_slots()
{
    //  Register datatypes with signal system
    qRegisterMetaType<fetch_signal_ptr>("fetch_signal_ptr");
    qRegisterMetaType<decode_signal_ptr>("decode_signal_ptr");
    qRegisterMetaType<execute_signal_ptr>("execute_signal_ptr");
    qRegisterMetaType<store_signal_ptr>("store_signal_ptr");
    qRegisterMetaType<pipeline_stats_ptr>("pipeline_stats_ptr");

    //  Connect view update signals
    connect(&(*fetch_worker), SIGNAL(fetch_done(fetch_signal_ptr)),
            this->gemini_view, SLOT(show_fetch_done(fetch_signal_ptr)));

    connect(&(*decode_worker), SIGNAL(decode_done(decode_signal_ptr)),
            this->gemini_view, SLOT(show_decode_done(decode_signal_ptr)));

    connect(&(*execute_worker), SIGNAL(execute_done(execute_signal_ptr)),
            this->gemini_view, SLOT(show_execute_done(execute_signal_ptr)));

    connect(&(*store_worker), SIGNAL(store_done(store_signal_ptr)),
            this->gemini_view, SLOT(show_store_done(store_signal_ptr)));

    connect(this, SIGNAL(pipeline_stats(pipeline_stats_ptr)),
            this->gemini_view, SLOT(show_pipeline_stats(pipeline_stats_ptr)));

    //  Connect thread running signals
    connect(this, SIGNAL(start_threads()), &(*fetch_worker), SLOT(doWork()));
    connect(this, SIGNAL(start_threads()), &(*decode_worker), SLOT(doWork()));
    connect(this, SIGNAL(start_threads()), &(*execute_worker), SLOT(doWork()));
    connect(this, SIGNAL(start_threads()), &(*store_worker), SLOT(doWork()));

    connect(this, SIGNAL(stop_threads()), &(*fetch_thread), SLOT(quit()));
    connect(this, SIGNAL(stop_threads()), &(*decode_thread),SLOT(quit()));
    connect(this, SIGNAL(stop_threads()), &(*execute_thread), SLOT(quit()));
    connect(this, SIGNAL(stop_threads()), &(*store_thread), SLOT(quit()));

    connect(this, SIGNAL(cpu_error(QString)), this->gemini_view, SLOT(show_cpu_error(QString)));
}


void CPU::load_byte_code( std::shared_ptr<Byte_code> bc )
{
    byte_code = bc;
}

void CPU::set_memory( std::shared_ptr<Memory> mem )
{
    memory = mem;
}

void CPU::set_cache_mode(Cache_type cache_type)
{
    memory->set_cache_type( cache_type );
}

void Fetch_worker::doWork()   {
    cpu->mutex_running_count.lock();
    cpu->running_count--;
    cpu->mutex_running_count.unlock();
    std::shared_ptr<Fetch_state> fetch_local_state;
    forever{
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        /* --  FETCH  BEGIN -- */
        {
            //  Return if we are stalled
            cpu->mutex_fetch_stalled.lock();
            if( cpu->fetch_stalled ) {
                cpu->fetch_halt_count += 1;
                cpu->fetch_stalled = false;
                cpu->mutex_fetch_stalled.unlock();
                goto fetch_end;
            }
            cpu->mutex_fetch_stalled.unlock();
            //  LOCK fetch stage temp state mutex
            cpu->mutex_fetch_temp_state.lock();
            //  Set fetch stage temp state processed
            cpu->fetch_temp_state_processed = true;
            //  Do we have a state to execute
            if (cpu->fetch_temp_state == NULL)
            {
                cpu->fetch_temp_state_processed = false;
                cpu->fetch_null_count++;
                //  UNLOCK fetch stage temp state
                cpu->mutex_fetch_temp_state.unlock();
                //  Wait for the decode stage to be done with its temp state
                cpu->mutex_decode_temp_state.lock();
                while(!cpu->decode_temp_state_processed) {
                    cpu->mutex_decode_temp_state.unlock();
                    //  @@TODO sleep
                    QThread::msleep(16);
                    cpu->mutex_decode_temp_state.lock();
                }
                //  LOCK the decode stage temp state (LOCKED)
                //  set the decode stage temp state to NULL
                cpu->decode_temp_state = NULL;
                cpu->decode_temp_state_processed = false;
                //  UNLOCK the decode stage temp state
                cpu->mutex_decode_temp_state.unlock();
                goto fetch_end;
            } else {
                cpu->fetch_count++;
                //  copy fetch temp state to local fetch state
                fetch_local_state = cpu->fetch_temp_state;
                //  UNLOCK fetch stage temp state mutex
                cpu->mutex_fetch_temp_state.unlock();
            }

            //  Setup a temporary decode state
            std::shared_ptr<Decode_state> decode_fetch_state = std::shared_ptr<Decode_state> (new Decode_state);
            decode_fetch_state->PC = fetch_local_state->PC;
            decode_fetch_state->decode_IR = (*cpu->byte_code)[fetch_local_state->PC];


            //  Setup Decode stage temp state
            {
//                //  Wait for decode to be done with its temp state
//                cpu->mutex_decode_temp_state.lock();
//                while(!cpu->decode_temp_state_processed) {
//                    cpu->mutex_decode_temp_state.unlock();
//                    //  @@TODO sleep
//                    QThread::msleep(16);
//                    cpu->mutex_decode_temp_state.lock();
//                }
//                //  LOCK decode stage temp state (LOCKED)
//                //  Set decode temp state to this decode state
//                cpu->decode_temp_state = decode_fetch_state;
//                cpu->decode_temp_state_processed = false;
//                //  UNLOCK decode stage temp state
//                cpu->mutex_decode_temp_state.unlock();
                //  Wait for decode stage to finish
                cpu->mutex_decode_stage_done.lock();
                while( !cpu->decode_stage_done )
                {
                    cpu->mutex_decode_stage_done.unlock();
                    QThread::msleep(16);
                    cpu->mutex_decode_stage_done.lock();
                }
                cpu->mutex_decode_stage_done.unlock();
                cpu->decode_temp_state = decode_fetch_state;
                cpu->decode_temp_state_processed = false;
            }

            //  Update our next state
            cpu->mutex_fetch_temp_state.lock();
            cpu->fetch_temp_state_processed = false;
            cpu->fetch_temp_state->PC = fetch_local_state->PC;
            //  Branch prediction
            if ( cpu->branch_prediction )
            {
                if ( cpu->branch_ways >= 0 ) {
                cpu->fetch_temp_state->PC++;
                } else {
                cpu->fetch_temp_state->PC = get_value( cpu->decode_temp_state->decode_IR );
                }
            } else {
                cpu->fetch_temp_state->PC++;
            }

            if ( cpu->fetch_temp_state->PC >= ( *(cpu->byte_code) ).size( ) )
            {
                cpu->fetch_temp_state = NULL;
            }
            cpu->mutex_fetch_temp_state.unlock();
        }/* --  FETCH END -- */
fetch_end:
        //  Setup signal of Fetch state
        fetch_signal_ptr fsi = std::shared_ptr<fetch_signal_info>(new fetch_signal_info);
        if (fetch_local_state != NULL)
            fsi->PC = fetch_local_state->PC;
        else
            fsi->PC = 0;
        //  Send message to the VIEW
        emit fetch_done(fsi);
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
        cpu->mutex_fetch_stage_done.lock();
        cpu->fetch_stage_done = true;
        cpu->mutex_fetch_stage_done.unlock();
    }
}

void Decode_worker::doWork()  {
    cpu->mutex_running_count.lock();
    cpu->running_count--;
    cpu->mutex_running_count.unlock();
    Instruction_register IR ;
    Gemini_access_type access_type ;
    Gemini_op decode_op ;
    Value value ;
    std::shared_ptr<Execute_state> execute_decode_state ;
    forever {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        /* --  DECODE BEGIN -- */
        {
            std::shared_ptr<Decode_state> decode_local_state;
            //  Return if we are stalled
            cpu->mutex_decode_stalled.lock();
            if( cpu->decode_stalled ) {
                cpu->decode_halt_count += 1;
                cpu->decode_stalled = false;
                cpu->mutex_decode_stalled.unlock();
                goto decode_end;
            }
            cpu->mutex_decode_stalled.unlock();
            //  LOCK decode stage temp state mutex
            cpu->mutex_decode_temp_state.lock();
            //  Set decode stage temp state processed
            cpu->decode_temp_state_processed = true;
            //  Do we have a decodable state
            if (cpu->decode_temp_state == NULL)
            {
                cpu->decode_null_count++;
                //  UNLOCK decode stage state lock
                cpu->mutex_decode_temp_state.unlock();
                //  Wait for the execute stage to be done with its temp
                cpu->mutex_execute_temp_state.lock();
                while(!cpu->execute_temp_state_processed) {
                    cpu->mutex_execute_temp_state.unlock();
                    //  @@TODO sleep
                    QThread::msleep(16);
                    cpu->mutex_execute_temp_state.lock();
                }
                //  LOCK execute stage temp state (LOCKED)
                //  set the execute stage temp state to NULL
                cpu->execute_temp_state = NULL;
                cpu->execute_temp_state_processed = false;
                //  UNLOCK the execute stage temp state
                cpu->mutex_execute_temp_state.unlock();
                goto decode_end;
            } else {
                cpu->decode_count++;
                //  copy decode stage temp state to decode stage local state
                decode_local_state = cpu->decode_temp_state;
                //  UNLOCK decode stage temp state
                cpu->mutex_decode_temp_state.unlock();
            }

            IR = decode_local_state->decode_IR;
            access_type = get_access_type( IR );
            decode_op = static_cast<Gemini_op>(get_op( IR ));
            //  Temps to hold a/an ...
            value = get_value( IR );
            execute_decode_state = std::shared_ptr<Execute_state> (new Execute_state);
            execute_decode_state->execute_op = decode_op;
            execute_decode_state->access_type = access_type;
            execute_decode_state->execute_value = value;
            execute_decode_state->PC = decode_local_state->PC;
            //  Wait for execute stage to be done with its temp state
            cpu->mutex_execute_temp_state.lock();
            while(!cpu->execute_temp_state_processed) {
                cpu->mutex_execute_temp_state.unlock();
                // @@TODO sleep
                QThread::msleep(16);
                cpu->mutex_execute_temp_state.lock();
            }
            //  LOCK execute stage temp state (LOCKED)

            //  set execute stage temp state
            cpu-> execute_temp_state = execute_decode_state;
            cpu->execute_temp_state_processed = false;
            //  UNLOCK execute stage temp state
            cpu->mutex_execute_temp_state.unlock();
        }/* --  DECODE END   -- */
decode_end:
        //  Emit state of decode
        decode_signal_ptr dsi = std::shared_ptr<decode_signal_info>(new decode_signal_info);
        dsi->IR = IR;
        //  Send message to the VIEW
        emit decode_done(dsi);
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
        cpu->mutex_decode_stage_done.lock();
        cpu->decode_stage_done = true;
        cpu->mutex_decode_stage_done.unlock();
    }
}

void Execute_worker::doWork()  {
    cpu->mutex_running_count.lock();
    cpu->running_count--;
    cpu->mutex_running_count.unlock();
    bool process_bubble = false;
    bool execute_branch;
    Value branch_too;
    Value value ;
    Value execute_value;
    Gemini_op execute_op ;
    Instruction_register i32;
    Gemini_access_type access_type ;
    std::shared_ptr<Execute_state> execute_local_state;
    Value last_memory_value {-1};
    bool last_access_was_sta = false;
    forever {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();

        execute_branch = false;
        branch_too = 0;
        if( process_bubble ) {
            cpu->execute_count++;
            //  Lock the fetch stalled state
            process_bubble = false;
            goto bubble_resume;
        }
        /* --  EXECUTE BEGIN -- */
        {
            //  Return if we are stalled
            cpu->mutex_execute_stalled.lock();
            if( cpu->execute_stalled ) {
                cpu->execute_halt_count++;
                cpu->mutex_execute_stalled.unlock();
                goto execute_end;
            }
            cpu->mutex_execute_stalled.unlock();
            //  LOCK execute stage temp state
            cpu->mutex_execute_temp_state.lock();
            //  Set decode stage temp state processed
            cpu->execute_temp_state_processed = true;
            //  Do we have an executable state
            if (cpu->execute_temp_state == NULL) {
                cpu->execute_null_count++;
                //  UNLOCK execute stage local state
                cpu->mutex_execute_temp_state.unlock();
                //  Wait for store stage to be done with its temp state
                cpu->mutex_store_temp_state.lock();
                while(!cpu->store_temp_state_processed) {
                    cpu->mutex_store_temp_state.unlock();
                    //  @@TODO sleep
                    QThread::msleep(16);
                    cpu->mutex_store_temp_state.lock();
                }
                //  LOCK store stage temp state (LOCKED)
                //  set store stage temp state to NULL
                cpu->store_temp_state = NULL;
                //  UNLOCK store stage temp state
                cpu->mutex_store_temp_state.unlock();
                goto execute_end;
            } else {
                cpu->execute_count++;
                //  copy execute stage temp state to execute stage local state
                execute_local_state = cpu->execute_temp_state;
                cpu->execute_temp_state_processed = true;
                //  UNLOCK execute stage temp state
                cpu->mutex_execute_temp_state.unlock();
            }
            value = execute_local_state->execute_value;
            execute_op = execute_local_state->execute_op;
            access_type = execute_local_state->access_type;
bubble_resume:
            //  Grab the value used
            switch ( execute_op )
            {
            case Gemini_op::NOTA:
            case Gemini_op::RET:
            case Gemini_op::ADDSL:
            case Gemini_op::SUBSL:
            case Gemini_op::MULSL:
            case Gemini_op::DIVSL:
            case Gemini_op::NOP:
            case Gemini_op::HLT:
            case Gemini_op::LABEL:
            case Gemini_op::EMPTY:
            case Gemini_op::INVALID:
                break;
            case Gemini_op::LDA:
            case Gemini_op::ADD:
            case Gemini_op::SUB:
            case Gemini_op::MUL:
            case Gemini_op::DIV:
            case Gemini_op::AND:
            case Gemini_op::OR:
            case Gemini_op::SETHI:
            case Gemini_op::SETLO:
            case Gemini_op::LDHI:
            case Gemini_op::LDLO:
                if (access_type == Gemini_access_type::MEMORY )
                {
                    //  Set a bubble to wait for the memory write to occur
                    if ( last_access_was_sta && (value == last_memory_value))
                    {
                        last_access_was_sta = false;
                        //  Stall untill fetch stage is done
                        cpu->mutex_fetch_stage_done.lock();
                        while (!cpu->fetch_stage_done)
                        {
                            cpu->mutex_fetch_stage_done.unlock();
                            QThread::msleep(16);
                            cpu->mutex_fetch_stage_done.lock();
                        }
                        cpu->mutex_fetch_stage_done.unlock();
                        //  Stall untill decode stage is done
                        cpu->mutex_decode_stage_done.lock();
                        while (!cpu->decode_stage_done)
                        {
                            cpu->mutex_decode_stage_done.unlock();
                            QThread::msleep(16);
                            cpu->mutex_decode_stage_done.lock();
                        }
                        cpu->mutex_decode_stage_done.unlock();

                        cpu->mutex_fetch_stalled.lock();
                        cpu->fetch_stalled = true;
                        cpu->mutex_fetch_stalled.unlock();
                        cpu->mutex_decode_stalled.lock();
                        cpu->decode_stalled = true;
                        cpu->mutex_decode_stalled.unlock();

                        //  Process the bubble
                        process_bubble = true;
                        last_memory_value = -1;
                        goto execute_end;
                    }
                   try{
                        execute_value = cpu->memory->get_memory( value );
                    }
                    catch(...) {
                        cpu->memory_access_overflow = true;
                    }
                }
                else if (access_type == Gemini_access_type::VALUE )
                {
                    execute_value = value;
                }
                break;
            case Gemini_op::STA:
                execute_value = value;
                break;
            case Gemini_op::JMP:
                execute_value = value;
                break;
            case Gemini_op::BA:
            case Gemini_op::BE:
            case Gemini_op::BL:
            case Gemini_op::BG:
            case Gemini_op::BGE:
            case Gemini_op::BLE:
            case Gemini_op::BNE:
                execute_value = value;
                break;
                //  @@TODO BRANCHPREDICTION
                //        if (branch_ways >= 0) {
                //            PC++;
                //        } else
                //            PC = get_value( IR );
                //        value =
            }
            //  Perform the operation
            switch ( execute_op )
            {
            case Gemini_op::LDA:
                cpu->Acc = execute_value;
//                cpu->PC++;
                break;
            case Gemini_op::STA:
//                cpu->PC++;
                break;
            case Gemini_op::ADD:
                cpu->Acc += execute_value;
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::SUB:
                cpu->Acc -= execute_value;
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::MUL:
                //  Overflow detection
                cpu->mull = cpu->Acc;
                cpu->mull *= execute_value;
                if ( ((cpu->mull & 0x7FFF0000) >> 16) != 0)
                    cpu->OVF = 1;
                else cpu->OVF = 0;
                cpu->Acc = (cpu->mull & 0xFFFF0000) >> 16;
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::DIV:
                //  Overflow detection
                cpu->divl = cpu->Acc;
                cpu->divl /= execute_value;
                if ( ((cpu->divl & 0x7FFF0000) >> 16) != 0)
                    cpu->OVF = 1;
                else cpu->OVF = 0;
                cpu->Acc = (cpu->mull & 0xFFFF0000) >> 16;
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::AND:
                cpu->Acc &= execute_value;
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::OR:
                cpu->Acc |= execute_value;
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::NOTA:
                cpu->Acc = ~(cpu->Acc);
                //  Set less then or equal
                if ( cpu->Acc > 0 )
                    cpu->CC = 2;
                else if ( cpu->Acc < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if ( cpu->Acc == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::SETHI:
                //  0 or 1 specifies what register we are going to be using
                i32 = 0;
                i32 |= cpu->Acc;
                i32 <<= 16;
                i32 &= 0xFFFF0000;
                if (execute_value == 0)
                    cpu->SL0 |= i32;
                else if ( execute_value == 1)
                    cpu->SL1 |= i32;
                else
                    cpu->sethi_access_violation = true;
//                    throw (std::out_of_range("CPU SETHI register access violation"));
//                cpu->PC++;
                break;
            case Gemini_op::SETLO:
                i32 = 0x00000000;
                i32 |= cpu->Acc;
                i32 &= 0x0000FFFF;
                if (execute_value == 0)
                    cpu->SL0 |= i32;
                else if ( execute_value == 1)
                    cpu->SL1 |= i32;
                else
                    cpu->setlo_access_violation = true;
//                    throw (std::out_of_range("CPU SETHLO register access violation"));
//                cpu->PC++;
                break;
            case Gemini_op::LDHI:
                //  Load High part of either SL0 or SL1 into the accumilator
                i32 = 0x00000000;
                if (execute_value == 0)
                    i32 = cpu->SL0;
                else if ( execute_value == 1)
                    i32 = cpu->SL1;
                else
                    cpu->sethi_access_violation = true;
//                    throw (std::out_of_range("CPU SETHLO register access violation"));
                i32 >>= 16;
                i32 &= 0x0000FFFF;
                cpu->Acc = 0x00000000;
                cpu->Acc |= i32;
//                cpu->PC++;
                break;
            case Gemini_op::LDLO:
                //  Load the low part of either SL0 or SL1 into the accumilator
                //  Load High part of either SL0 or SL1 into the accumilator
                i32 = 0;
                if (execute_value == 0)
                    i32  = cpu->SL0;
                else if ( execute_value == 1)
                    i32 = cpu->SL1;
                else
                    cpu->setlo_access_violation = true;
//                    throw (std::out_of_range("CPU SETHLO register access violation"));
                i32 &= 0x0000FFFF;
                cpu->Acc = 0x00000000;
                cpu->Acc |= i32;
//                cpu->PC++;
                break;
            case Gemini_op::ADDSL:
                //  Add SL0 and SL1 and put the result in SL0
                cpu->SL0 += cpu->SL1;
                //  Set less then or equal
                if ( cpu->SL0 > 0 )
                    cpu->CC = 2;
                else if ( cpu->SL0 < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if (  cpu->SL0 == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::SUBSL:
                //  Sub SL0 and SL1 and put the result in SL0
                cpu->SL0 -= cpu->SL1;
                //  Set less then or equal
                if ( cpu->SL0 > 0 )
                    cpu->CC = 2;
                else if ( cpu->SL0 < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if (  cpu->SL0 == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::MULSL:
                //  Multiply SL0 and SL1 and put the result in SL0
                cpu->SL0 *= cpu->SL1;
                //  Set less then or equal
                if ( cpu->SL0 > 0 )
                    cpu->CC = 2;
                else if ( cpu->SL0 < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if (  cpu->SL0 == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::DIVSL:
                //  Divide SL0 and SL1 and put the result in SL0
                cpu->SL0 /= cpu->SL1;
                //  Set less then or equal
                if ( cpu->SL0 > 0 )
                    cpu->CC = 2;
                else if ( cpu->SL0 < 0 )
                    cpu->CC = 1;
                else
                    cpu->CC = 0;
                //  Set equal
                if (  cpu->SL0 == 0 )
                    cpu->CE = 1;
                else
                    cpu->CE = 0;
//                cpu->PC++;
                break;
            case Gemini_op::NOP:
                cpu->Acc += 0;
//                cpu->PC++;
                break;
            case Gemini_op::JMP:
                //  @@TODO HANDLE CASE
//                Q_ASSERT ( false );
                if (++(cpu->jmp_stack_depth) > cpu->JMP_STACK_MAX_DEPTH)
                {
                    cpu->jump_stack_overflow = true;
//                    throw (std::out_of_range("Stack overflow"));
                }
                cpu->jmp_stack.push(cpu->PC+1);
                execute_branch = true;
                value = execute_value;
                branch_too = value;
                break;
            case Gemini_op::RET:
                //  @@TODO HANDLE CASE
//                Q_ASSERT ( false );
                if (--(cpu->jmp_stack_depth) < 0)
                {
                    cpu->jump_stack_overflow = true;
//                    throw (std::out_of_range("Stack underflow"));
                }
                execute_branch = true;
                value = cpu->jmp_stack.top();
                execute_value = value;
                cpu->jmp_stack.pop();
                break;
            case Gemini_op::BA:
                execute_branch = true;
                branch_too = execute_value;
                break;
            case Gemini_op::BE:
                if ( cpu->CE == 1 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                    cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::BL:
                if ( cpu->CC == 1 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                    cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::BG:
                if ( cpu->CC == 2 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                    cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::BGE:
                if ( cpu->CC == 2 && cpu->CE == 1 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                    cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::BLE:
                if ( cpu->CC == 1 && cpu->CE == 1 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                    cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::BNE:
                if ( cpu->CE == 0 ) {
                    execute_branch = true;
                    branch_too = execute_value;
                   cpu->branch_ways -= 1;
                } else {
                    cpu->branch_ways += 1;
                }
                break;
            case Gemini_op::HLT:
                cpu->fetch_temp_state = NULL;
                cpu->decode_temp_state = NULL;
                cpu->execute_temp_state = NULL;
                cpu->store_temp_state = NULL;
                break;
            case Gemini_op::LABEL:
            case Gemini_op::EMPTY:
            case Gemini_op::INVALID:
                break;
            }
            //  Make sure we can stall the fetch and decode
            if (execute_op == Gemini_op::STA)
                last_access_was_sta = true;
            else
                last_access_was_sta = false;

            if( execute_branch )
            {
                //  Wait for Execute temp state processed to be false
                cpu->mutex_execute_temp_state.lock();
                while (cpu->execute_temp_state_processed) {
                    cpu->mutex_execute_temp_state.unlock();
                    QThread::msleep(16);
                    cpu->mutex_execute_temp_state.lock();
                }
                //  Checkout our predicted branch
                if (cpu->execute_temp_state->PC != branch_too )
                {
//                    //  Wait for decode temp state processed to be false
//                    cpu->mutex_decode_temp_state.lock();
//                    while( cpu->decode_temp_state_processed)
//                    {
//                        cpu->mutex_decode_temp_state.unlock();
//                        QThread::msleep(16);
//                        cpu->mutex_decode_temp_state.lock();
//                    }
//                    //  set decode temp state to NULL
//                    cpu->decode_temp_state = NULL;
//                    cpu->mutex_decode_temp_state.unlock();
//                    //  Wait for Fetch temp state processed to be false
//                    cpu->mutex_fetch_temp_state.lock();
//                    while ( cpu->fetch_temp_state_processed )
//                    {
//                        cpu->mutex_fetch_temp_state.unlock();
//                        QThread::msleep(16);
//                        cpu->mutex_fetch_temp_state.lock();
//                    }
//                    //  Null out Execute temp state
//                    cpu->execute_temp_state = NULL;
//                    //  set new fetch PC to branch_too
//                    cpu->fetch_temp_state = std::shared_ptr<Fetch_state>(new Fetch_state);
//                    cpu->fetch_temp_state->PC = branch_too;
//                    cpu->mutex_fetch_temp_state.unlock();
//
                    //  Wait for decode stage to finish
                    cpu->mutex_decode_stage_done.lock();
                    while( !cpu->decode_stage_done )
                    {
                        cpu->mutex_decode_stage_done.unlock();
                        QThread::msleep(16);
                        cpu->mutex_decode_stage_done.lock();
                    }
                    cpu->mutex_decode_stage_done.unlock();
                    //  Wait for fetch stage to finish
                    cpu->mutex_fetch_stage_done.lock();
                    while( !cpu->fetch_stage_done )
                    {
                        cpu->mutex_fetch_stage_done.unlock();
                        QThread::msleep(16);
                        cpu->mutex_fetch_stage_done.lock();
                    }
                    cpu->mutex_fetch_stage_done.unlock();
                    //  Null out Execute temp state
                    cpu->execute_temp_state = NULL;
                    //  set new fetch PC to branch_too
                    cpu->fetch_temp_state = std::shared_ptr<Fetch_state>(new Fetch_state);
                    cpu->fetch_temp_state->PC = branch_too;
                }
                cpu->mutex_execute_temp_state.unlock();
            }
            //  Check if store stage temp state is available
            std::shared_ptr<Store_state> store_execute_state = NULL;
            //  Do we need to pass a state to the store stage?
            if( execute_op == Gemini_op::STA) {
                store_execute_state = std::shared_ptr<Store_state> (new Store_state);
                store_execute_state->store_Acc = cpu->Acc;
                store_execute_state->store_value = execute_value;
                store_execute_state->store_op = execute_op;
                //  remember this value, we will not be able to access it later
                last_memory_value = execute_value;
            }
            cpu->mutex_store_temp_state.lock();
                while(!cpu->store_temp_state_processed) {
                    cpu->mutex_store_temp_state.unlock();
                    // @@TODO sleep
                    QThread::msleep(16);
                    cpu->mutex_store_temp_state.lock();
                }
                //  LOCK store stage temp state (LOCKED)
                //  set store stage temp state
                cpu->store_temp_state = store_execute_state;
                cpu->store_temp_state_processed = false;
                //  UNLOCK store state temp state
                cpu->mutex_store_temp_state.unlock();
        }/* --  EXECUTE END   -- */
execute_end:
        //  Emit Signal of cpu state
        execute_signal_ptr esi = std::shared_ptr<execute_signal_info>(new execute_signal_info);
        esi->A = cpu->A;
        esi->B = cpu->B;
        esi->Acc = cpu->Acc;
        esi->Zero = cpu->Zero;
        esi->One = cpu->One;
        esi->MAR = cpu->MAR;
        esi->MDR = cpu->MDR;
        esi->TEMP = cpu->TEMP;
        esi->CC = cpu->CC;
        esi->CE = cpu->CE;
        esi->OVF = cpu->OVF;
        esi->jmp_stack_depth = cpu->jmp_stack_depth;
        esi->SL0 = cpu->SL0;
        esi->SL1 = cpu->SL1;
        //  Send message to VIEW
        emit execute_done(esi);
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }
}

void Store_worker::doWork() {
    cpu->mutex_running_count.lock();
    cpu->running_count--;
    cpu->mutex_running_count.unlock();
    forever {
        cpu->mutex_running_count.lock();
        cpu->running_condition.wait(&(cpu->mutex_running_count));
        cpu->running_count++;
        cpu->mutex_running_count.unlock();
        /* --  STORE BEGIN -- */
        {
            std::shared_ptr<Store_state> store_local_state;
            //  LOCK store stage temp state
            cpu->mutex_store_temp_state.lock();
            //  Set store stage temp state processed
            cpu->store_temp_state_processed = true;
            //  Do we have a store state to execute?
            if (cpu->store_temp_state == NULL)
            {
                cpu->store_null_count++;
                //  UNLOCK store stage temp state
                cpu->mutex_store_temp_state.unlock();
                goto store_end;
            } else {
                cpu->store_count++;
                //  Copy store stage temp state to store stage local state
                store_local_state = cpu->store_temp_state;
                cpu->store_temp_state = NULL;
                //  UNLOCK store stage temp state
                cpu->mutex_store_temp_state.unlock();
            }

            Gemini_op store_op = store_local_state->store_op;
            Value store_value = store_local_state->store_value;
            Register_value store_Acc = store_local_state->store_Acc;
            switch ( store_op )
            {
            case Gemini_op::STA:
                try{
                cpu->memory->set_memory( store_value, store_Acc );
            }
                catch (...) {
                    cpu->memory_access_overflow = true;
                }

                break;
            case Gemini_op::LDA:
            case Gemini_op::ADD:
            case Gemini_op::SUB:
            case Gemini_op::MUL:
            case Gemini_op::DIV:
            case Gemini_op::AND:
            case Gemini_op::OR:
            case Gemini_op::NOTA:
            case Gemini_op::JMP:
            case Gemini_op::RET:
            case Gemini_op::BA:
            case Gemini_op::BE:
            case Gemini_op::BL:
            case Gemini_op::BG:
            case Gemini_op::BGE:
            case Gemini_op::BLE:
            case Gemini_op::BNE:
            case Gemini_op::SETHI:
            case Gemini_op::SETLO:
            case Gemini_op::LDHI:
            case Gemini_op::LDLO:
            case Gemini_op::ADDSL:
            case Gemini_op::SUBSL:
            case Gemini_op::MULSL:
            case Gemini_op::DIVSL:
            case Gemini_op::NOP:
            case Gemini_op::HLT:
            case Gemini_op::LABEL:
            case Gemini_op::EMPTY:
            case Gemini_op::INVALID:
                break;
            }
        }/* --  STORE END   -- */
store_end:
        //  Emit signal of store state
        store_signal_ptr ssi = std::shared_ptr<store_signal_info> (new store_signal_info);
        ssi->cache_hits = cpu->memory->hits;
        ssi->cache_misses = cpu->memory->misses;
        emit store_done(ssi);
        cpu->mutex_running_count.lock();
        cpu->running_count--;
        cpu->mutex_running_count.unlock();
    }

}
