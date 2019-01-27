//Admin class is concerned with providing services such as error reporting
//For the overall admin of the scanner, its role will change as new phases are added

#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include <iostream>
#include <fstream>
#include "scanner.h"
#include "token.h"

// This is the maximum number of errors before the compiler bails out
#define MAXERRORS 10

// each phase has its kind of errors
enum errorkind {ScanE, ParseE, ScopeE, TypeE};

using namespace std;

class Administration
{
  public:
    // set up input and output files for scanning
    Administration(ifstream& in, ofstream &out, Scanner &sc);

    ~Administration() {}

    // begin a new line of input
    void NewLine(){lineNo++; correctline = true;}

    // Error function for the phases
    void error (string text);

    //call scanner from here
    int scan();

  private:

    //output file
    ofstream *outputfileptr;

    //scanner
    Scanner *scanr;

    //maintain the current line number
    int lineNo;

    //report error only if correct line is true; prevents multiple/redundant error/line
    bool correctline;

    //count # of errors
    int errorCount;
};
#endif // ADMIN_H
