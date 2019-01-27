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
             NEWLINE, NONAME, ENDOFFILE, DIV,       // 260 - 263
             MOD, SEMICOLON, PLUS, MINUS,           // 264 - 267
             TIMES, DIVIDE, LEFTP, RIGHTP,          // 268 - 271
             BADCHAR, DOT, COMMA, LEFTB, RIGHTB,    // 272 - 276
             COLON, ARROW, DOUBLEB, OR, AND,        // 277 - 281
             EQUALS, LESSTHAN, GREATERTHAN};        // 282 - 284
#endif // SYMBOL_H
