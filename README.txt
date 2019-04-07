CPSC 4620 Project - Compiler
Group Members: Brett Cassidy, George Chelariu
Brett's Contribution: Implemeted the code generation phase as per the hints provided on Moodle (a bigger contribution than it may sound.) Retrofitted the compiler to work with the interpreter that was provided as well. Contributed to technical documentation.
George's Contribution: Wrote emit functions, copied down nested.txt and deduced what each label does. Provided assistance through pair programming to help catch some syntax errors on the fly and suggestions for overcoming obstacles. Pair programming also helped overcome the frustration with these obstacles. Contributed to technical documentation and spell checked it to make sure it makes sense.

FILE NAMES
admin.cc
admin.h
Assembler.cc
Assembler.h
blocktable.cc
blocktable.h
driver.cc
Interpreter/ (folder)
  -driver.cc
  -interp.cc
  -interp.h
  -Makefile
  -out.txt
linearSearchPL.txt
Makefile
nested.txt
parser.cc
parser.h
plGrammar.txt
README.txt(this file)
scanner.cc
scanner.h
symbol.h
symtable.cc
symtable.h
test.txt
test1.txt
test2.txt
token.cc
token.h
typekindenum.h

HOW TO COMPILE
0. Unpack provided contents
1. Open up the GNOME terminal and change directory to content location
2. type the following commands
   
   make
   driver <source file> <token file name> <PLAM assembly code file name> <machine code file name>
   cd Interpreter
   make
   driver <machine code file name>
  
   Upon first using the driver command, the compiler scans and parses through the source file to generate
   1. The list of tokens it takes in
   2. The assembly code for PLAM
   3. The machine code that the interpreter runs

   From there, going into interpreter and using driver a second time is what allows for proper code execution of the translated program.

   The way we have tested our program is by typing the following

   driver test.txt output.txt PLAM.txt Interpreter/out.txt
   cd Interpreter
   driver out.txt

test.txt, test1.txt, test2.txt linearSearchPL.txt, and nested.txt are all test files where test1.txt tests everything.

The code is free of compile-time and run-time errors.

This part of the project took about 11 hours to complete, cumulatively among both people.
