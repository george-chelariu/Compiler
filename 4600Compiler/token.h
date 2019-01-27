#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include "symbol.h"
//
// Modified by SH Jan 14, 2019
//
using namespace std;
//  Implements lexical units or tokens of the same language.
//
//  A token is a terminal symbol with attributes
//
//  Possible attributes of a token are implemented by a C++
//  struct attval. For specific tokens, fields of attval may not
//  be defined. For example, the lexeme field of NUM
//  operator and punctuation symbols is empty.

// Prototype to spell the corresponding token
string spellS(Symbol);

class Token{
public:
    //Default constructor
    Token():sname(NONAME), svalue(attval(-1,string(""))){}
    //Constructor
    Token(Symbol s, int v, string l):sname(s),svalue(attval(v,l)){}
    //Destructor
    ~Token(){}
    //Return the terminal symbol
    Symbol getSymbol()const {return sname;}
    //return attribute values
    int getValue()const {return svalue.value;};
    string getLexeme()const {return svalue.lexeme;}
    //print the token
    void insert(ostream &os){os << "Token: " << getSymbol() << "\n"
                                << "\tAttributes: " << "\n"
                                << "\tLexeme: " << getLexeme() << "\n"
                                << "\tValue: " <<getValue() << endl;}
private:
    struct attval{
        attval(int i, string s):value(i),lexeme(s){}
        int value;  // for NUM it is the numerical value of the decimal string
                    // for ID/Reserved words, the location of the token in the hashtable
        string lexeme;  // for ID/Reserved words
        };
        attval svalue;  // terminal symbol value
        Symbol sname;   // terminal name
};
//overloaded stream insertion
ostream& operator<<(ostream&, Token&);
#endif // TOKEN_H
