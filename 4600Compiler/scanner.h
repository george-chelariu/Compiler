#ifndef SCANNER_H
#define SCANNER_H

// The scaller class interface.
// The main interface function to the scanner is:
//      Token getToken()
// which reads the input stream and recognizes and returns the next token.

#include <iostream>
#include <fstream>
#include <string>
#include "token.h"
#include "symtable.h"

using namespace std;

class Scanner{
  public:
    // Use input from a given stream and use a given symbol table.
    Scanner(){} // This is the default constructor that does nothing

    // The scanner works intimately with the input stream and therefore
    // the PL program file is a matter of scanner.
    //
    // Also, scanner works directly with the symbol table
    // so that the symbol table is a member of the class.
    //
    // Scanner is always one character ahead in the input stream

    // Constructor to initialize a scanner object with specific input
    // stream and a symbol table

    Scanner(ifstream &instream, Symtable &symboltable);

    // The destructor function currently does nothing
    ~Scanner();

//    The most important function. The main scanner interface.
//    It scans the next sequence of characters in the input and tries
//    to recognize if the characters match the "pattern" of some terminal
//    symbol in the language. If a matching pattern is recognized, it constructs
//    a token initializing appropriate attribute fields, and returns the token
//    to the caller.
    Token getToken();
  private:
    ifstream *inputfileptr; //input stream linked with the PL source file
    Symtable *symtableptr;  //the symbol table
    char laChar; //look-ahead character
    //private functions not needed by clients of scanner
    bool isWhitespace (char achar); // returns true if the parameter is a white-space
                                    // char; returns false otherwise
    bool isAlpha (char achar);      // returns true if the parameter is an alphabetic
                                    // char; returns false otherwise
    bool isNumeric(char achar);     // returns true if the parameter is a numeric
                                    // char; returns false otherwise
    bool isSpecial(char achar);     // returns true if the parameter is a special
                                    // char (punctuation, operator etc); returns false otherwise
    Token recognizeName();          // Recognize an ID or reserved word and return the token
    Token recognizeSpecial();       // Recognize a special terminal symbol and return the token
    Token recognizeNumeral();       // Recognize a decimal whole number and return the token
    void recognizeComment();        // Recognize and ignore the comments
};
#endif // SCANNER_H
