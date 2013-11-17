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
#include <stdexcept>

extern Gemini_system gemini_system;

CPU::CPU( )
{
    jmp_stack_depth = 0;
    fetch_thread = new Fetch_thread (this);
    decode_thread = new Decode_thread (this);
    execute_thread = new Execute_thread (this);
    store_thread = new Store_thread (this);
}

void CPU::tick( )
{
    //  cpu clock
    execute_instruction( );
}

//  Master control for thread continue
void CPU::execute_instruction( )
{
    fetch();
    decode();
    execute();
    store();
    return;
    mutex_running_count.lock ();
    while (running_count > 0)
    {
        mutex_running_count.unlock();
//        sleep(1);
        mutex_running_count.lock();
    }
    running_condition.wakeAll();
    mutex_running_count.unlock();
}

void CPU::fetch( )
{
    /* --  FETCH  BEGIN -- */
    {
        std::shared_ptr<Fetch_state> fetch_local_state;
        //  Return if we are stalled
        mutex_fetch_stalled.lock();
        if( fetch_stalled ) {
            mutex_fetch_stalled.unlock();
            return;
        }
        mutex_fetch_stalled.unlock();
        //  LOCK fetch stage local state mutex
        mutex_fetch_temp_state.lock();
        //  Do we have a state to execute
        if (fetch_local_state == NULL)
        {
            //  UNLOCK fetch stage temp state
            mutex_fetch_temp_state.unlock();
            //  Wait for the decode stage to be done with its temp state
            mutex_decode_temp_state.lock();
            while(!decode_temp_state_processed) {
                mutex_decode_temp_state.unlock();
                //  @@TODO sleep
                mutex_decode_temp_state.lock();
            }
            //  LOCK the decode stage temp state
            //  set the decode stage temp state to NULL
            decode_temp_state = NULL;
            decode_temp_state_processed = false;
            //  UNLOCK the decode stage temp state
            mutex_decode_temp_state.unlock();
            return;
        } else {
            //  copy our temp state to local fetch state
            fetch_local_state = fetch_temp_state;
            fetch_local_state->IR = (*byte_code)[PC];
            fetch_temp_state_processed = true;
            //  UNLOCK fetch stage temp state mutex
            mutex_fetch_temp_state.unlock();
        }

        instruction_count++;
        //  Setup a temporary decode state
        std::shared_ptr<Decode_state> decode_fetch_state = std::shared_ptr<Decode_state> (new Decode_state);
        decode_fetch_state->decode_IR = fetch_local_state->IR;

        //  Emit signal of Fetch state
        std::shared_ptr<fetch_signal_info> fsi = std::shared_ptr<fetch_signal_info>(new fetch_signal_info);
        fsi->PC = PC;
        fsi->instruction_count = instruction_count;

        //  Wait for decode to be done with its temp state
        mutex_decode_temp_state.lock();
        while(!decode_temp_state_processed) {
            mutex_decode_temp_state.unlock();
            //  @@TODO sleep
            mutex_decode_temp_state.lock();
        }
        //  LOCK decode stage temp state

        //  Set decode temp state to this decode state
        decode_temp_state = decode_fetch_state;
        decode_temp_state_processed = false;
        //  UNLOCK decode stage temp state
        mutex_decode_temp_state.unlock();
        emit fetch_done(fsi);
    }/* --  FETCH END -- */
}
void CPU::decode ( )
{
    std::shared_ptr<Decode_state> decode_local_state;
    /* --  DECODE BEGIN -- */
    {
        //  Return if we are stalled

        //  LOCK decode stage local state mutex
        //  Do we have a decodable state
        if (decode_local_state == NULL)
        {
            //  UNLOCK decode stage state lock
            //  Wait for the execute stage to be done with its temp
            //  LOCK execute stage temp state
            //  set the execute stage temp state to NULL
            //  UNLOCK the execute stage temp state
            return;
        } else {
            //  copy decode stage temp state to decode stage local state
            //  UNLOCK decode stage temp state
        }

        Instruction_register IR = decode_local_state->decode_IR;
        Gemini_access_type access_type = get_access_type( IR );
        Gemini_op decode_op = static_cast<Gemini_op>(get_op( IR ));
        //  Temps to hold a/an ...
        Value value = get_value( IR );

        std::shared_ptr<Execute_state> execute_decode_state = std::shared_ptr<Execute_state> (new Execute_state);
        execute_decode_state->execute_op = decode_op;
        execute_decode_state->access_type = access_type;
        execute_decode_state->execute_value = value;
        //  Emit state of decode
        std::shared_ptr<decode_signal_info> dsi = std::shared_ptr<decode_signal_info>(new decode_signal_info);
        dsi->IR = IR;
        //  Wait for execute stage to be done with its temp state
        //  LOCK execute stage temp state
        //  set execute stage temp state
        //  UNLOCK execute stage temp state
        emit decode_done(dsi);
        //  Signal decode stage is done

    }/* --  DECODE END   -- */
}
void CPU::execute ( )
{
    std::shared_ptr<Execute_state> execute_local_state;
    /* --  EXECUTE BEGIN -- */
    {
        //  Return if we are stalled

        //  LOCK execute stage local state
        //  Do we have an executable state
        if (execute_local_state == NULL) {
            //  UNLOCK execute stage local state
            //  Wait for store stage to be done with its temp state
            //  LOCK store stage temp state
            //  set store stage temp state to NULL
            //  UNLOCK store stage temp state
            return;
        } else {
           //  copy execute stage temp state to execute stage local state
           //  UNLOCK execute stage temp state
        }
        Value value = execute_local_state->execute_value;
        Value execute_value;
        Gemini_op execute_op = execute_local_state->execute_op;
        Instruction_register i32;
        Gemini_access_type access_type = execute_local_state->access_type;
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
                execute_value = memory->get_memory( value );
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
            Acc = execute_value;
            PC++;
            break;
        case Gemini_op::STA:
            PC++;
            break;
        case Gemini_op::ADD:
            Acc += execute_value;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::SUB:
            Acc -= execute_value;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::MUL:
            //  Overflow detection
            mull = Acc;
            mull *= execute_value;
            if ( ((mull & 0x7FFF0000) >> 16) != 0)
                OVF = 1;
            else OVF = 0;
            Acc = (mull & 0xFFFF0000) >> 16;
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::DIV:
            //  Overflow detection
            divl = Acc;
            divl /= execute_value;
            if ( ((divl & 0x7FFF0000) >> 16) != 0)
                OVF = 1;
            else OVF = 0;
            Acc = (mull & 0xFFFF0000) >> 16;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::AND:
            Acc &= execute_value;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::OR:
            Acc |= execute_value;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::NOTA:
            Acc = ~Acc;
            //  Set less then or equal
            if ( Acc > 0 )
                CC = 2;
            else if ( Acc < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if ( Acc == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::SETHI:
            //  0 or 1 specifies what register we are going to be using
            i32 = 0;
            i32 |= this->Acc;
            i32 <<= 16;
            i32 &= 0xFFFF0000;
            if (execute_value == 0)
                this->SL0 |= i32;
            else if ( execute_value == 1)
                this->SL1 |= i32;
            else
                throw (std::out_of_range("CPU SETHI register access violation"));
            PC++;
            break;
        case Gemini_op::SETLO:
            i32 = 0x00000000;
            i32 |= this->Acc;
            i32 &= 0x0000FFFF;
            if (execute_value == 0)
                this->SL0 |= i32;
            else if ( execute_value == 1)
                this->SL1 |= i32;
            else
                throw (std::out_of_range("CPU SETHLO register access violation"));
            PC++;
            break;
        case Gemini_op::LDHI:
            //  Load High part of either SL0 or SL1 into the accumilator
            i32 = 0x00000000;
            if (execute_value == 0)
                i32 = SL0;
            else if ( execute_value == 1)
                i32 = SL1;
            else
                throw (std::out_of_range("CPU SETHLO register access violation"));
            i32 >>= 16;
            i32 &= 0x0000FFFF;
            Acc = 0x00000000;
            Acc |= i32;
            PC++;
            break;
        case Gemini_op::LDLO:
            //  Load the low part of either SL0 or SL1 into the accumilator
            //  Load High part of either SL0 or SL1 into the accumilator
            i32 = 0;
            if (execute_value == 0)
                i32  = SL0;
            else if ( execute_value == 1)
                i32 = SL1;
            else
                throw (std::out_of_range("CPU SETHLO register access violation"));
            i32 &= 0x0000FFFF;
            Acc = 0x00000000;
            Acc |= i32;
            PC++;
            break;
        case Gemini_op::ADDSL:
            //  Add SL0 and SL1 and put the result in SL0
            SL0 += SL1;
            //  Set less then or equal
            if ( SL0 > 0 )
                CC = 2;
            else if ( SL0 < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if (  SL0 == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::SUBSL:
            //  Sub SL0 and SL1 and put the result in SL0
            SL0 -= SL1;
            //  Set less then or equal
            if ( SL0 > 0 )
                CC = 2;
            else if ( SL0 < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if (  SL0 == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::MULSL:
            //  Multiply SL0 and SL1 and put the result in SL0
            SL0 *= SL1;
            //  Set less then or equal
            if ( SL0 > 0 )
                CC = 2;
            else if ( SL0 < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if (  SL0 == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::DIVSL:
            //  Divide SL0 and SL1 and put the result in SL0
            SL0 /= SL1;
            //  Set less then or equal
            if ( SL0 > 0 )
                CC = 2;
            else if ( SL0 < 0 )
                CC = 1;
            else
                CC = 0;
            //  Set equal
            if (  SL0 == 0 )
                CE = 1;
            else
                CE = 0;
            PC++;
            break;
        case Gemini_op::NOP:
            Acc += 0;
            PC++;
            break;
        case Gemini_op::JMP:
            if (++jmp_stack_depth > JMP_STACK_MAX_DEPTH)
                throw (std::out_of_range("Stack overflow"));
            jmp_stack.push(PC+1);
            PC = execute_value;
            break;
        case Gemini_op::RET:
            if (--jmp_stack_depth < 0)
                throw (std::out_of_range("Stack underflow"));
            PC = jmp_stack.top();
            jmp_stack.pop();
            break;
        case Gemini_op::BA:
            PC = execute_value;
            break;
        case Gemini_op::BE:
            if ( CE == 1 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::BL:
            if ( CC == 1 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::BG:
            if ( CC == 2 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::BGE:
            if ( CC == 2 && CE == 1 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC ++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::BLE:
            if ( CC == 1 && CE == 1 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::BNE:
            if ( CE == 0 ) {
                PC = execute_value;
                branch_ways -= 1;
            } else {
                PC++;
                branch_ways += 1;
            }
            break;
        case Gemini_op::HLT:
            PC = (*byte_code).size ();
            fetch_temp_state = NULL;
            break;
        case Gemini_op::LABEL:
        case Gemini_op::EMPTY:
        case Gemini_op::INVALID:
            break;
        }
        std::shared_ptr<Store_state> store_execute_state = std::shared_ptr<Store_state> (new Store_state);
        store_execute_state->store_Acc = Acc;
        store_execute_state->store_value = execute_value;
        store_execute_state->store_op = execute_op;
        //  Emit Signal of cpu state
        std::shared_ptr<execute_signal_info> esi =
                std::shared_ptr<execute_signal_info>(new execute_signal_info);
        esi->A = this->A;
        esi->B = this->B;
        esi->Acc = this->Acc;
        esi->Zero = this->Zero;
        esi->One = this->One;
        esi->MAR = this->MAR;
        esi->MDR = this->MDR;
        esi->TEMP = this->TEMP;
        esi->CC = this->CC;
        esi->CE = this->CE;
        esi->OVF = this->OVF;
        esi->jmp_stack_depth = this->jmp_stack_depth;
        esi->SL0 = this->SL0;
        esi->SL1 = this->SL1;
        //  Check if store stage temp state is available
        //  LOCK store stage temp state
        //  set lock stage temp state
        //  UNLOCK store state temp state
        emit execute_done(esi);
    }/* --  EXECUTE END   -- */

    if ( PC >= ( *byte_code ).size( ) )
    {
        fetch_temp_state = NULL;
    }

}
void CPU::store( )
{
    std::shared_ptr<Store_state> store_local_state;
    /* --  STORE BEGIN -- */
    {
        //  LOCK store stage temp state
        if (store_local_state == NULL)
        {
            //  UNLOCK store stage temp state
            return;
        } else {
           //  Copy store stage temp state to store stage local state
           //  UNLOCK store stage temp state
        }

        Gemini_op store_op = store_local_state->store_op;
        Value store_value = store_local_state->store_value;
        Register_value store_Acc = store_local_state->store_Acc;
        switch ( store_op )
        {
        case Gemini_op::STA:
            memory->set_memory( store_value, store_Acc );
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
        //  Emit signal of store state
        std::shared_ptr<store_signal_info> ssi =
                std::shared_ptr<store_signal_info>(new store_signal_info);
        ssi->cache_hits = memory->hits;
        ssi->cache_misses = memory->misses;
        emit store_done(ssi);
    }/* --  STORE END   -- */
}

Register_value CPU::get_value( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x0000FFFF;
    return ( (Register_value)tmp );
}

Gemini_op_type CPU::get_op( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0xFF000000;
    tmp >>= 24;
    return ( (Gemini_op_type)tmp );
}

Gemini_access_type CPU::get_access_type( Instruction_register ir )
{
    Instruction_register tmp = ir;
    tmp &= 0x00FF0000;
    tmp >>= 16;
    return ( (Gemini_access_type) tmp );
}

void CPU::initialize( )
{
    Zero = 0;
    One = 1;
    PC = 0;
    fetch_temp_state = std::shared_ptr<Fetch_state> ( new Fetch_state );
    fetch_temp_state->IR = (*byte_code)[PC];
    decode_temp_state = NULL;
    execute_temp_state = NULL;
    store_temp_state = NULL;
}

void CPU::halt( )
{
    PC = byte_code->size( );
    fetch_temp_state = NULL;
}

bool CPU::done()
{
    return ( fetch_temp_state == NULL );
    //    return ( PC == byte_code->size () );
}

void CPU::setView(gemini *view)
{
    this->gemini_view = view;
    //  The only reason we need the gemini view is to connect our signals to it
    connect(this, SIGNAL(fetch_done(std::shared_ptr<fetch_signal_info>)),
            view, SLOT(show_fetch_done(std::shared_ptr<fetch_signal_info>)));
    connect(this, SIGNAL(decode_done(std::shared_ptr<decode_signal_info>)),
            view, SLOT(show_decode_done(std::shared_ptr<decode_signal_info>)));
    connect(this, SIGNAL(execute_done(std::shared_ptr<execute_signal_info>)),
            view, SLOT(show_execute_done(std::shared_ptr<execute_signal_info>)));
    connect(this, SIGNAL(store_done(std::shared_ptr<store_signal_info>)),
            view, SLOT(show_store_done(std::shared_ptr<store_signal_info>)));
}

void CPU::load_byte_code( std::shared_ptr<Byte_code> bc )
{
    byte_code = bc;
    instruction_count = 0;
}

void CPU::set_memory( std::shared_ptr<Memory> mem )
{
    memory = mem;
}

void CPU::set_cache_mode(Cache_type cache_type)
{
    memory->set_cache_type( cache_type );
}

