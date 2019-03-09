#include <iostream>
#include <fstream>

#include "admin.h"
#include "scanner.h"
#include "symtable.h"
#include "token.h"
#include "symbol.h"
#include "parser.h"


// The original grammar for infix2postfix translator
//
// Stmt --> list eof
//
// list --> expr ; list | empty
//
// expr --> expr + term | expr - term | term
//
// term --> term * factor | term / factor | term div factor | term mod factor | factor
//
// factor --> (expr) | id | num

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3){
        cout << "Usage: " << argv [0] << " PL_file Output_file" << endl;
        return 0;
    }
    // Open the input and output files.
    ifstream inputfile (argv[1]);
    if(!inputfile){
        cerr << "PL_file " << argv[1] << " could not be opened" << endl;
        return 1;
    }
    ofstream outputfile(argv[2]);
    if(!outputfile)
    {
        cerr << "Output_file" << argv[2] << " could not be opened" << endl;
        return 1;
    }
    // create a symbol table


    Symtable st;


    //create a scanner
    Scanner sc(&inputfile, &st);

    //create a parcer
    Parser pc("a");

    
    //Get the compiler running.
    Administration compiler(inputfile, outputfile, sc, pc);
    int status = compiler.parse();
    if (status ==0)
       cout << "Scanning and parsing successful" << endl;
    else{
       cerr << "Program contains error(s)" << endl;
       return 0;
    }
    return 0;
}

