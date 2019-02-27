

#include <iostream>
#include "parser.h"
#include "admin.h"

using namespace std;
Parser::Parser(string h){
}


void Parser::adv(){
   input = admin-> get();
   currentName = input.myName();
   currentLex = input.getLexeme();
   return;
}

void Parser::error(){
   admin->ParseError( currentLex);
   return;
   
}
   
   

void Parser::work(Administration &ad){
   admin = &ad;
// to actually parse return 1 when errors, if good return 0
   //can also return the amount of errors found if we go that way
   adv();
   block();
   adv();
   if(currentLex != ".")
      error();
   return;
}

void Parser::block(){
   if (currentLex != "begin")
      error();
   adv();
   defnPart();
   statePart();
   if (currentLex == "end"){
      return;
   }
   else{
      error();
   }
   return;
}

void Parser::defnPart(){
   if (currentLex == "end")
      return;
   if (currentLex == ";")
      adv();
   if(currentLex == "const"){
      constDef();
   }
   else if (currentLex == "proc"){
      procDef();
   }
   else if( currentLex == "Boolean" || currentLex == "integer")
      varibleDef();
   else
      return;
   adv();
   defnPart();
   return;
}

void Parser::constDef(){
   adv();
   name();
   adv();
   if (currentName == "EQUALS"){
      adv();
      constant();
      adv();
   }
   else
      error();
   return;

}

void Parser::name(){
   if(currentName == "ID")
      return;
   else
      error();
   return;
}


void Parser::constant(){
   if (currentName == "NUM")
      return;
   else if( currentLex =="true" || currentLex == "false")
      return;
   name();
   return;

}

void Parser::procDef(){
   adv();
   name();
   adv();
   block();
   adv();
   return;
}

void Parser::varibleDef(){
   adv();
   if( currentLex == "array"){
      adv();
      varList();
      if (currentLex == "[" ){
	 adv();
	 constant();
	 adv();
	 if(currentLex =="]")
	    adv();
	 else
	    error();
      }
      else
	 error();
   }
   else
      varList();
   return;
}

void Parser::varList(){
   name();
   adv();
   while( currentLex == ","){
      adv();
      name();
      adv();
   }
   return;
}


void Parser::statePart(){
   if (currentLex == "end"|| currentLex == "fi" || currentLex == "od")
      return;
   if (currentLex == ";")
      adv();
   if(currentLex == "skip")
      emptyState();
   else if (currentLex == "read")
      readState();
   else if( currentLex == "write")
      writeState();
   else if (currentLex == "call")
      precedureState();
   else if (currentLex== "if")
      ifState();
   else if (currentLex == "do")
      doState();
   else if (currentName == "ID")
      assignState();
   else {
      return;
   }
   adv();
   statePart();
   return;
}

void Parser:: emptyState(){
   while (currentLex != ";")
      adv();
   return;
}
      
void Parser::readState(){
   adv();
   varAccList();
   return;
}

void Parser::varAccList(){
   varAcc();
   while (currentLex == ","){
      adv();
      varAcc();
   }
   return;
}

void Parser::varAcc(){
   name();
   adv();
   if (currentLex == "["){
      adv();
      expression();
      if (currentLex == "]")
	 adv();
      else
	 error();
   }
   return;
}


void Parser::expression(){
   primeExp();
   while (currentLex == "&" || currentLex == "|"){
      adv();
      primeExp();
   }
   return;
}

void Parser::primeExp(){
   simpleExp();
   if (currentLex == "<" || currentLex == "=" || currentLex == ">"){
      adv();
      simpleExp();
   }
   return;
}


void Parser::simpleExp(){
   if (currentLex == "-")
      adv();
   term();
   while (currentLex == "+"|| currentLex == "-"){
      adv();
      term();
   }
   return;
}

void Parser::term(){
   factor();
   while (currentLex == "*" || currentLex == "/" || currentLex == "\\"){
      adv();
      factor();
   }
   return;

}

void Parser::factor(){
   if (currentLex == "~"){
      adv();
      factor();
   }
   else if (currentLex== "("){
      adv();
      expression();
      if (currentLex == ")")
	 adv();
      else
	 error();
   }
   else if( currentName == "ID")
      varAcc();
   else {
      constant();
      adv();
   }
   return;
}

      
void Parser::writeState(){
   adv();
   expList();
   return;
}

void Parser::expList(){
   expression();
   while( currentLex == ","){
      adv();
      expression();
   }
   return;
}


void Parser::precedureState(){
   adv();
   name();
   adv();
   return;
}

void Parser::ifState(){
   adv();
   guardedComList();
   if (currentLex == "fi"){
      adv();
   }
   else
      error();
   return;
}

void Parser::guardedComList(){
   guardedCom();
   while ( currentLex == "[]"){
      adv();
      guardedCom();
   }
}

void Parser::guardedCom(){
   expression();
   if (currentLex == "->")
      adv();
   else
      error();
   statePart();
   return;
}

void Parser::doState(){
   adv();
   guardedComList();
   if (currentLex == "od")
      adv();
   else
      error();
   return;
}


void Parser::assignState(){
   varAccList();
   if (currentLex == ":=")
      adv();
   else error();
   expList();
   return;
}
