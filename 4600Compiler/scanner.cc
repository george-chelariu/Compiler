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
    laChar= ' ';
    *inputfileptr >> noskipws;
}

Token Scanner::getToken()
{
    while (!inputfileptr->eof() && (laChar == ' ' || laChar == '\t'))
    {
       *inputfileptr >> laChar;
    }
    if(isAlpha(laChar))
    {
       return recognizeName();
    }
    if(isNumeric(laChar))
    {
        return recognizeNumeral();
    }
    if(laChar == '$')
    {
        recognizeComment();
        Token endline(NEWLINE, -1, "nl");
	*inputfileptr  >> laChar; 
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
    if (achar == ' ')
        return true;
    return false;
}

bool Scanner::isAlpha (char achar)
{
    //A-Z and a-z find themselves between [65, 90] and [97, 122] respectively
   if ((achar >= 'A' && achar <= 'Z') || (achar >= 'a' && achar <=  'z'))
      return true;
   
   return false;
}
 
bool Scanner::isNumeric(char achar)
{
   //if achar is between the ascii values for 0 and 9 return true else false
   if ((achar >= '0') && (achar <= '9'))
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
    if (achar == '&' || (achar >= '(' && achar <= '/')
        || (achar >= ':' && achar <= '>') || (achar >= '[' && achar <= ']')
        || achar == '|' || achar == '~' || achar == '\n')
    {
        return true;
    }
    return false;
}

Token Scanner::recognizeName()
{
   string theName ="";
   theName += laChar;
 
   while((isAlpha(laChar)  || isNumeric(laChar) || laChar == '_') )
    {

        *inputfileptr >> laChar;
        theName+=laChar;

    }
    Token recogName(ID, symtableptr->insert(theName), theName);
    return recogName;
}

Token Scanner::recognizeSpecial()
{
   string theSpecial ="";
   theSpecial += laChar;
    // laChar moves forward no matter what
    *inputfileptr >> laChar;
    // the -> case
    if(laChar == '>' && theSpecial == "-")
    {
        theSpecial+=laChar;
        Token recogPt(ID, symtableptr->insert(theSpecial), theSpecial);
        return recogPt;
    }
    // the := case
    if(laChar == '=' && theSpecial == ":")
    {
        theSpecial+=laChar;
        Token recogColon(ID, symtableptr->insert(theSpecial), theSpecial);
        return recogColon;
    }
    // the [] case
    if(laChar == ']' && theSpecial == "[")
    {
        theSpecial+=laChar;
        Token recogArray(ID, symtableptr->insert(theSpecial), theSpecial);
        return recogArray;
    }

    if(theSpecial == ";")
    {
        Token recogSpecial(SEMICOLON, -1, theSpecial);
        return recogSpecial;
    }
    if(theSpecial == "+")
    {
        Token recogPlus(PLUS, -1, theSpecial);
        return recogPlus;
    }
    if(theSpecial == "-")
    {
        Token recogMinus(MINUS, -1, theSpecial);
        return recogMinus;
    }
    if(theSpecial == "*")
    {
        Token recogTimes(TIMES, -1, theSpecial);
        return recogTimes;
    }
    if(theSpecial == "/")
    {
        Token recogDivide(DIVIDE, -1, theSpecial);
        return recogDivide;
    }
    if(theSpecial == "(")
    {
        Token recogLeftP(LEFTP, -1, theSpecial);
        return recogLeftP;
    }
    if(theSpecial == ")")
    {
        Token recogRightP(RIGHTP, -1, theSpecial);
        return recogRightP;
    }
    if(theSpecial == "\n"){
       Token newline (NEWLINE, -1, theSpecial);
       return newline;
    }
    Token unrecognizable(BADCHAR, -1, " ");
    return unrecognizable;
}

Token Scanner::recognizeNumeral()
{
   string theNumber="";
   theNumber += laChar;
    while(isNumeric(laChar))
    {
        *inputfileptr >> laChar;
        theNumber+=laChar;
    }
    stringstream conv(theNumber);
    int val = 0;
    conv >> val;
    Token recogNum(NUM, val, " ");
    return recogNum;
}

void Scanner::recognizeComment()
{
    while(laChar != '\n')
    {
        *inputfileptr >> laChar;
    }
}

