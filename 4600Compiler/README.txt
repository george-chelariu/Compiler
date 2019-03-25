CPSC 4620 Project - Scope and Type Checking
Group Members: Brett Cassidy, George Chelariu
Brett's Contribution: Augmented the implementation for the parser to work with the block table. Fixed syntax errors and other bugs that didn't allow scope and type checking with the test files. Vast amounts of code optimization done to the parser. Helped write the technical documentation file by providing information on the functions and classes.
George's Contribution: Implemented the block table which later got improved upon by Brett. Wrote the majority of the additions to design, classes, and function sections of the technical documentation. Some spell checking and ensuring that the document describes the program as clearly as possible. Performed buddy programming alongside Brett, catching errors and providing suggestions while coding, which helped speed up debugging process.

FILE NAMES
admin.cc
admin.h
blocktable.h
blocktable.cc
driver.cc
linearSearchPL.txt
Makefile
output.txt
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
token.cc
token.h
typekindenum.h

HOW TO COMPILE
0. Unpack provided contents
1. Open up the GNOME terminal and change directory to content location
2. type the following commands
   
   make
   driver <source file> <destination file>
  
   The program will scan through the source file and put all the tokens it finds in the destination file. Any erroneous tokens in get output to terminal as a scan error.
   The parser will then parse the tokens deposited by the scanner to ensure it follows the PL grammar. Parse errors will be output to the terminal alongside scan errors.
   There is already a sample source file called test.txt and a destination file called output.txt
   The way we have tested our program is by typing the following two commands

   driver test.txt output.txt

   driver linearSearchPL.txt output.txt

test1.txt is the old test file that tests only the scanner.

The code is free of compile-time and run-time errors.

This part of the project took about 13 hours to complete, cumulatively among both people.
