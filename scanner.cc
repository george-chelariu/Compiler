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
    symtableptr->loadResvd();
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
      return recognizeSpecial();
 
 
  
    
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
        || achar == '|' || achar == '~' || achar == '\n' || achar == '~')
    {
       return true;
    }
    return false;
}

Token Scanner::recognizeName()
{
   string theName ="";
  
 
   while((isAlpha(laChar)  || isNumeric(laChar) || laChar == '_') )
   {
      theName += laChar;
      *inputfileptr >> laChar;
   }

   if(theName.length() > 80)
   {
      Token tooLong(BADNAME, -1, " ");
      return tooLong;
   }
       
     
 
   Token recogName(ID, symtableptr->insert(theName), theName);
   return recogName;
}

Token Scanner::recognizeSpecial()
{
   string theSpecial ="";
   theSpecial += laChar;
   // laChar moves forward no matter what
   if (!(*inputfileptr >> laChar)){
     
      Token endtoken(ENDOFFILE, -1, "eof");
      return endtoken;
   }
   // the -> case
   if(laChar == '>' && theSpecial == "-")
   {
      theSpecial+=laChar;
      *inputfileptr >> laChar;
      Token recogPt(ARROW, symtableptr->insert(theSpecial), theSpecial);
      return recogPt;
   }
   // the := case
   if(laChar == '=' && theSpecial == ":")
   {
      theSpecial+=laChar;
      *inputfileptr >> laChar;
      Token recogAssign(ASSIGN, symtableptr->insert(theSpecial), theSpecial);
      return recogAssign;
   }
   // the [] case
   if(laChar == ']' && theSpecial == "[")
   {
      theSpecial+=laChar;
      *inputfileptr >> laChar;
      Token recogArray(DOUBLEB, symtableptr->insert(theSpecial), theSpecial);
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
    if(theSpecial == "\\"){
        Token recogMod(MODULUS, -1, theSpecial);
        return recogMod;
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

 //dot
    if(theSpecial == ".")
    {
       Token recogDot(DOT, -1, theSpecial);
       return recogDot;
    }
    //comma
    if(theSpecial == ",")
    {
       Token recogComma(COMMA, -1, theSpecial);
        return recogComma;
    }
    //left bracket
    if(theSpecial == "[")
    {
       Token recogLeftB(LEFTB, -1, theSpecial);
        return recogLeftB;
    }
    //right bracket
    if(theSpecial == "]")
    {
       Token recogRightB(RIGHTB, -1, theSpecial);
        return recogRightB;
    }
    //or operator
    if(theSpecial == "|")
    {
       Token recogOr(OR, -1, theSpecial);
        return recogOr;
    }
    //and operator
    if(theSpecial == "&")
    {
       Token recogAnd(AND, -1, theSpecial);
        return recogAnd;
    }
    //equals operator
    if(theSpecial == "=")
    {
       Token recogEqual(EQUALS, -1, theSpecial);
        return recogEqual;
    }
    //less than operator
    if(theSpecial == "<")
    {
       Token recogLess(LESSTHAN, -1, theSpecial);
        return recogLess;
    }
    //greater than operator
    if(theSpecial == ">")
    {
       Token recogMore(GREATERTHAN, -1, theSpecial);
       return recogMore;
       
    }
    //tildy
    if(theSpecial == "~")
    {
       Token recogNot(NOT , -1, theSpecial);
       return recogNot;
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
    if(val > 32768)
    {
       Token tooBig(BADNUM, -1, " ");
       return tooBig;
    }
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

