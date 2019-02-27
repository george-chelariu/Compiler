#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "admin.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Administration;

class Parser{
  public:

   /* two constructions even though the string added in one does nothing was not letting me procide with only one when it set some values to 0
    */
   Parser(){}
   Parser(string h);

   ~Parser(){}


   /* since the way we decided to do the parser was as the scanner went through it put everything in a vector for us to parse through later
      this is the insert function that putes the tokens in a place that we can use them and in order
      when also skipping all comments newlines etc so we only get useful ones
      takes in line as well so that when making an error reporst we say what line the error is on
   */
  

   /* main function that starts all the work it does returns a int that used to be an error count, but now its 0 if good 1 if errors are there
    */
   void work(Administration& ad);


  
  private:
   /* if no error has been reported will cout what info about the token that did it like name Lexeme and line to help figure out the error*/
   void error();
   // advances the two Token holders so that we can go father through the file 1 token at a time
   void adv();
   // handles the block portion of the program always starting with begin and ending with end
   void block();
   //the area of the program when we define everything all of them end with a ; and will repeat until you call something in the statment part
   void defnPart();
   // handles the definitions of constant varibleswith assigning them varibles as well
   void constDef();
   // helper function to see if a name is Valid
   void name();
   // helper function to see if what we are looking at, is a boolean definition name or number
   void constant();
   //handles the definition of procedures by calling block() withen itself
   void procDef();
   // handles the creation of varibles wither that be a series of int, or booleans, can also handle arrays ( no assigning)
   void varibleDef();
   //helper function to handle a group of names
   void varList();
   // statement part of the program
   void statePart();
   // empty states skips everything til the statment is done
   void emptyState();
   // read function can accesses varibles from a list or an array or both at the same time
   void readState();
   //helper function for accsessing lists of varibles 
   void varAccList();
   // helper function for acessessing single varibles
   void varAcc();
   // helper function for handling all exprestions
   void expression();
   // helper function for handling all prime expretions
   void primeExp();
   // helper function for handling all simple exprestions
   void simpleExp();
   // helper functions for handling all terms
   void term();
   // helper function for handling all factors
   void factor();
   //write statement handler, uses exprestions lists
   void writeState();
   // exprestion list helper function, lets a series of expretions work
   void expList();
   //procedure statment, helps call any defined procidures eventually
   void precedureState();
   // if statment handler making sure if statment are handled properally
   void ifState();
   // a list of exprestions and statments, mainly a helper function
   void guardedComList();
   //a comand by the logic of an exprestion -> then statments to follow up with
   void guardedCom();
   // very much like the if statments
   void doState();
   // assigning values to varibles
   void assignState();

   Token input;
   string currentName;
   string currentLex;
   Administration  *admin;
};
#endif // PARSER_H
