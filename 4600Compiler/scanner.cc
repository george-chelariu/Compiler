#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scanner.h"
#ifndef SCANNER_H

using namespace std;

Scanner::Scanner(ifstream *instream, Symtable *symboltable)
{
    inputfileptr = instream;
    symtableptr = symboltable;
}

Token Scanner::getToken()
{
}

Token Scanner::recognizeName(){

}

Token Scanner::recognizeSpecial(){

}

Token Scanner::recognizeSpecial(){

}

void recognizeComment(){

}

#endif
