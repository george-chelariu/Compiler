#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "admin.h"
#include "symbol.h"
#include "blocktable.h"
#include "typekindenum.h"
#include <iostream>
#include <vector>


using namespace std;
class Administration;

const int MAXLABEL = 1000;

class Parser{
  public:

   /* two constructions even though the string added in one does nothing was not letting me procide with only one when it set some values to 0
    */
   Parser(){}
   Parser(BlockTable *h);

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
   void typeError();


   
   // error that returns void for things like bool name() where the are just looking at if something is happing and dont make anything at that level
   void Verror();
   // the main error function that looks for Lex first then Name and returns when it has found a Token that is in the follow set, returns with that tokens depth
   int error();
   //a double loop function seeing if the current token is in the follow vectors, looking at Lex then name
   int stop();
   // advances the two Token holders so that we can go father through the file 1 token at a time
   void adv();
   // handles the block portion of the program always starting with begin and ending with end
   bool block();
   //the area of the program when we define everything all of them end with a ; and will repeat until you call something in the statment part
   bool defnPart(int* varibles);
   // handles the definitions of constant varibleswith assigning them varibles as well
   bool constDef(int* varibles);
   // helper function to see if a name is Valid
   bool name();
   // helper function to see if what we are looking at, is a boolean definition name or number
   bool constant();
   //handles the definition of procedures by calling block() withen itself
   bool procDef();
   // handles the creation of varibles wither that be a series of int, or booleans, can also handle arrays ( no assigning)
   bool varibleDef(int* varibles);
   //helper function to handle a group of names
   bool varList(vector<int>& names);
   // statement part of the program
   bool statePart();
   // empty states skips everything til the statment is done
   bool emptyState();
   // read function can accesses varibles from a list or an array or both at the same time
   bool readState();
   //helper function for accsessing lists of varibles 
   bool varAccList(vector<mType>& holder);
   // helper function for acessessing single varibles
   bool varAcc();
   // helper function for handling all exprestions
   bool expression(mType& expholder);
   // helper function for handling all prime expretions
   bool primeExp(mType& expholder);
   // helper function for handling all simple exprestions
   bool simpleExp(mType& expholder);
   // helper functions for handling all terms
   bool term(mType& expholder);
   // helper function for handling all factors
   bool factor(mType& expholder);
   //write statement handler, uses exprestions lists
   bool writeState();
   // exprestion list helper function, lets a series of expretions work
   bool expList(vector<mType>& exp);
   //procedure statment, helps call any defined procidures eventually
   bool precedureState();
   // if statment handler making sure if statment are handled properally
   bool ifState();
   // a list of exprestions and statments, mainly a helper function
   bool guardedComList();
   //a comand by the logic of an exprestion -> then statments to follow up with
   bool guardedCom();
   // very much like the if statments
   bool doState();
   // assigning values to varibles
   bool assignState();

   int newLabel();

   //holds the follow set when that thing is a varible thing but has a constant Lex, like "ID" "NUM" "SEMICOLON"
   vector <Symbol> stopName;
   // the token that we are currently at
   Token input;
   // two holder varibles to help readability so its not input.spellS a lot
   Symbol current;
   // a pointer to admin so we can get it to get tokens and report errors to it
   Administration  *admin;

   BlockTable *Table;
};
#endif // PARSER_H
