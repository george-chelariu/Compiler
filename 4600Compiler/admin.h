//Admin class is concerned with providing services such as error reporting
//For the overall admin of the scanner, its role will change as new phases are added

#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include <iostream>
#include <fstream>
#include "scanner.h"
#include "token.h"
#include "parser.h"

// This is the maximum number of errors before the compiler bails out
#define MAXERRORS 10
class Parser;
// each phase has its kind of errors
enum errorkind {ScanE, ParseE, ScopeE, TypeE};

using namespace std;

class Administration
{
  public:
    // set up input and output files for scanning
   Administration(ifstream& in, ofstream &out, Scanner &sc, Parser &pc, ofstream &outasm);

    ~Administration() {}


    Token get();


    //call scanner from here
    int scan();

    //call parser from here
    int parse();

    void ParseError (string text);

    //the three emit functions
    void emit1(string op);
    void emit2(string op, int arg1);
    void emit3(string op, int arg1, int arg2);
  
  private:

  // Error function for the phases
    void error (string text);

    // begin a new line of input
    void NewLine();

    //output file
    ofstream *outputfileptr;

    //output assembler files
    ofstream *outputasm;

    //scanner
    Scanner *scanr;

    //parser
    Parser *parsr;

    //maintain the current line number
    int lineNo;

    //report error only if correct line is true; prevents multiple/redundant error/line
    bool correctline;

    //count # of errors
    int errorCount;

    //starts true and turns false when  we find an error while emitting
    bool emitting;
};
#endif // ADMIN_H
