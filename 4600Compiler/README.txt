CPSC 4620 Project - Parser
Group Members: Brett Cassidy, George Chelariu
Brett's Contribution: Wrote the implementation for the parser and included the error handling for as many cases as possible, testing the parser rigorously. Helped write the technical documentation file by providing information on the functions and classes.
George's Contribution: Drew the entire parse tree based on the description in the PDF to make it easier to read. The parse tree was very helpful in implementing the parser by visualizing it. Wrote the entire plGrammar.txt file to clearly lay out the grammar and adjust it so it's LL(1). Wrote the entire design section of the technical documentation file and contributed to some other functions. Also spell checked the document to ensure the descriptions are laid out clearly.

FILE NAMES
admin.cc
admin.h
driver.cc
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

HOW TO COMPILE
0. Unpack provided contents
1. Open up the GNOME terminal and change directory to content location
2. type the following commands
   
   make
   driver <source file> <destination file>
  
   The program will scan through the source file and put all the tokens it finds in the destination file. Any erroneous tokens in get output to terminal as a scan error.
   The parser will then parse the tokens deposited by the scanner to ensure it follows the PL grammar. Parse errors will be output to the terminal alongside scan errors.
   There is already a sample source file called test.txt and a destination file called output.txt
   The way we have tested our program is by typing the following

   driver test.txt output.txt

test.txt is the main test file it has comments and is referanced in the Tech document.

The code is free of compile-time and run-time errors.

This part of the project took about 24 hours to complete, cumulatively among both people.
