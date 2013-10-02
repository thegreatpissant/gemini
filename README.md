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
 - Direct Mapped (1-way Set Associative) Cache model
 - 2-way Set Associative Cache model
 - Cache hit or miss notification in GUI
 - Cache hit counter in GUI
 - Generate bytecode from Gemini asm files
 - Read bytecode
 - New Instructions: BGE, BLE, BNE
 - New Instructions: MUL, DIV
 - New Instructions: JMP, RET
 - Display current Instruction in hex
 - Display interpreted meaning of bytecode instruction
 - EXC: Implement SET and SETHI instructions to support 32bit values
 - EXC: Modify MUL and DIV to support 32bit values
