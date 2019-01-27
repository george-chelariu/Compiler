#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "scanner.h"

using namespace std;

Scanner::Scanner(ifstream *instream, Symtable *symboltable)
{
    inputfileptr = instream;
    symtableptr = symboltable;
}

Token Scanner::getToken()
{
    while (!inputfileptr.eof() && !(laChar == " " || laChar == "\t"))
    {
        inputfileptr >> laChar;
    }
    if(isAlpha(laChar))
    {
        return recognizeName();
    }
    if(isNumeric(laChar))
    {
        return recognizeNumeral();
    }
    if(laChar == "$")
    {
        recognizeComment();
        Token endline(NEWLINE, -1, "nl");
        return endline;
    }
    if(isSpecial(laChar))
    {
        return recognizeSpecial();
    }
    Token endtoken(ENDOFFILE, -1, "eof");
    return endtoken;
}

bool Scanner::isWhitespace (char achar)
{
    if (achar == " ")
        return true;
    return false;
}

bool Scanner::isAlpha (char achar)
{
    //A-Z and a-z find themselves between [65, 90] and [97, 122] respectively
    if ((achar > 64 && achar < 91) || (achar > 96 && achar 123))
        return true;
    return false;
}

bool Scanner::isNumeric(char achar)
{
    //if achar is between the ascii values for 0 and 9 return true else false
    if ((achar > 47) && (achar < 58))
        return true;
    return false;
}

bool Scanner::isSpecial(char achar)
{
    /*
        if achar is any of these in the respective ascii values return true else false
        the symbols according to the project description
        . , ; [ ] & | ~ < = > + - * / \ ( ) := [] -> $
        the symbols according to the if condition
        $ & ( ) * + , - . / : ; < = > [ \ ] | ~
    */
    if (achar == 38 || (achar > 39 && achar < 48)
        || (achar > 57 && achar < 63) || (achar > 90 && achar < 94)
        || achar == 124 || achar == 126)
    {
        return true;
    }
    return false;
}

Token Scanner::recognizeName()
{
    string theName = laChar;
    while(isAlpha(laChar)  || isNumeric(laChar) || laChar == '_')
    {
        inputfileptr >> laChar;
        theName.append(laChar);
    }
    Token recogName(ID, symtableptr.insert(theName), theName);
    return recogName;
}

Token Scanner::recognizeSpecial()
{
    string theSpecial = laChar;
    // laChar moves forward no matter what
    inputfileptr >> laChar;
    // the -> case
    if(laChar = 62 && theSpecial = 45)
    {
        theSpecial.append(laChar);
        Token recogPt(ID, symtableptr.insert(theSpecial), theSpecial);
        return recogPt;
    }
    // the := case
    if(laChar = 61 && theSpecial = 58)
    {
        theSpecial.append(laChar);
        Token recogColon(ID, symtableptr.insert(theSpecial), theSpecial);
        return recogColon;
    }
    // the [] case
    if(laChar = 93 && theSpecial = 91)
    {
        theSpecial.append(laChar);
        Token recogArray(ID, symtableptr.insert(theSpecial), theSpecial);
        return recogArray;
    }

    if(theSpecial = ";")
    {
        Token recogSpecial(SEMICOLON, -1, theSpecial);
        return recogSpecial;
    }
    if(theSpecial = "+")
    {
        Token recogPlus(PLUS, -1, theSpecial);
        return recogPlus;
    }
    if(theSpecial = "-")
    {
        Token recogMinus(MINUS, -1, theSpecial);
        return recogMinus;
    }
    if(theSpecial = "*")
    {
        Token recogTimes(TIMES, -1, theSpecial);
        return recogTimes;
    }
    if(theSpecial = "/")
    {
        Token recogDivide(DIVIDE, -1, theSpecial);
        return recogDivide;
    }
    if(theSpecial = "(")
    {
        Token recogLeftP(LEFTP, -1, theSpecial);
        return recogLeftP;
    }
    if(theSpecial = ")")
    {
        Token recogRightP(RIGHTP, -1, theSpecial);
        return recogRightP;
    }
    Token unrecognizable(BADCHAR, -1, " ");
    return unrecognizable;
}

Token Scanner::recognizeNumeral()
{
    string theNumber = laChar;
    while(isNumeric(laChar))
    {
        inputfileptr >> laChar;
        theNumber.append(laChar);
    }
    stringstream conv(theNumber);
    int val = 0;
    conv >> val;
    Token(NUM, val, " ");
    return recogNum;
}

void recognizeComment()
{
    while(laChar != "\n")
    {
        inputfileptr >> laChar;
    }
}

