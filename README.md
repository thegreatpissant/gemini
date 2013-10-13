gemini
======

James A. Feister jfeister@udel.edu, openjaf@gmail.com
- Project located at www.github.com/thegreatpissant/gemini.git
- Class: CISC 360 - Computer Architecture
- Instructor: Seth Morecraft
- See: http://www.cis.udel.edu/~morecraf/cisc360/ for more information

Project 1: Gemini architecture, Implement the following
 * GUI: Showing registers and instruction
 * Parsing of program for syntax errors: Alert user of failure
 * Running of program instructions, non bytecode translation
 * Detection of memory access errors, Alert user of failure
 * This is the base of the project.
 * Project1 writeup is located at "./doc/jfeister-Project1-writeup.pdf"
 * Test Gemini assembly files are in "./doc" and "./gemini_asm_files"

Project 2: Cache, Instruction Bytecode
 - Project2 writeup is located at "./doc/jfeister-Project2-writeup.pdf"
 - 'GeminiAssempler.pro' - Gemini Assembler project.
 - './GeminiAssembler' project source directory
 - 'Gemini.pro'- Gemini simulator.
 - './Gemini' project source directory
 - "./gemini_asm_files" Test Gemini assembly files.
 - Direct Mapped (1-way Set Associative) Cache model
 - 2-way Set Associative Cache model
 - Cache hit or miss counter in GUI
 - Assembler to Generate bytecode from Gemini asm files
 - Simulator will now read bytecode
 - New Instructions: BGE, BLE, BNE
 - New Instructions: MUL, DIV
 - New Instructions: JMP, RET
 - Display current Instruction in hex
 - Display interpreted meaning of bytecode instruction
 - ExtraCredit: Implement SET and SETHI instructions to support 32bit values
 - ExtraCredit: Modify MUL and DIV to support 32bit values
 - ExtraCredit: 25 deep stack for JMP and RET
 - ExtraCredit: Cache to grab 4 memory blocks durring each fetch.

