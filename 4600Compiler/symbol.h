#ifndef SYMBOL_H
#define SYMBOL_H

using namespace std;
// Modified by SH Jan 13, 2019
//
// Symbol type definition using enum to implement terminal symbols of the expression language.
//
// For convenience of scanning and error reporting additional symbols are used which are not terminal symbols. These are:
//      BADNUM      to indicate overflow
//      BADNAME     to indicate erroneous ID
//      NONAME      to be used in an "empty" token
//      ENDOFFILE   to indicate end-of-file condition
//      NEWLINE     to indicate newline in input
//      BADCHAR     to indicate erroneous char symbol

//      Apparently DIV and MOD are not supposed to be there because they aren't part of PL
enum Symbol {ID=256, NUM, BADNUM, BADNAME,          // 256 - 259
             NEWLINE, NONAME, ENDOFFILE,            // 260 - 262
             MODULUS, SEMICOLON, PLUS, MINUS,       // 263 - 266
             TIMES, DIVIDE, LEFTP, RIGHTP,          // 267 - 270
             BADCHAR, DOT, COMMA, LEFTB, RIGHTB,    // 271 - 275
             ASSIGN, ARROW, DOUBLEB, OR, AND,        // 276 - 280
             EQUALS, LESSTHAN, GREATERTHAN, NOT,     // 281 - 284
	     BEGIN, END, CONST, ARRAY, INT, BOOLEAN, //285 - 290
	     PROC, SKIP, READ, WRITE, CALL,         //291 - 295
	     IF, DO, FI,OD, FALSE, TRUE};           //296 - 301
#endif // SYMBOL_H
