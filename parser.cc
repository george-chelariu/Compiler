#include <iostream>
#include "parser.h"
#include "admin.h"

/*a lot of repeated things so mainly commeting on the unique things so dont have to add 1000+
  comments that mainly reapeat on sections of code
  will have something like this when "done"
*/

using namespace std;

//constructor that gets called when we actually do things with it
//errors out without it
Parser::Parser(string h){
}

// function that gets the next token and puts the info in the holders
void Parser::adv(){
   // get returns the next valid token, reuses a lot of the old scanner code
   input = admin-> get();
   currentName = input.myName();
   currentLex = input.getLexeme();
   return;
}

int Parser::error(){
   //setting up what typpe of error
   string text = "ParseE ";
   // if ID we whant the Lex on the error report
   if(currentName == "ID")
      text.append(currentLex);
   // otherwise we want the Name as that gives more info
   else
      text.append(currentName);
   //sends it out to be reported
   admin->ParseError(text);
   //looks to see if this one is a Token that is handled
   int depth = stop();
   while(depth == 0){ //0 = not handled +num = Lux handled -num Name handled
      adv();
      depth= stop();
      //repeat until a good token is found
   }
   return depth;
   
}
// same as error but no passing is and swaping stop with stop2nd
int Parser::error2nd(){
   int depth = stop();
   while(depth == 0){
      adv();
      depth= stop2nd();
   }
   return depth;
   
}

//just error that returns nothing 
void Parser::Verror(){
   string text = "ParseE ";
   if(currentName == "ID")
      text.append(currentLex);
   else
      text.append(currentName);
   admin->ParseError(text);
   int depth = stop();
   while(depth == 0){
      adv();
      depth= stop();
   }
   return;
   
}
// looks through all of Lex to see if current Token is handled
//then if Name is hadled
// if not there return 0
int Parser::stop(){
   for (int i = stopName.size()-1; i<= stopLex.size(); i--){
      if (stopLex[i] == currentLex)
	 return i+1;
   }
   for (int i = stopName.size()-1; i>= 0; i--){
      if(stopName[i] == currentName)
	 return (-1*(i+1));
   }
   return 0;
}
//same as stop but Name first then Lex
// if nothing return 0
int Parser::stop2nd(){
   
   for (int i = stopName.size()-1; i>= 0; i--){
      if(stopName[i] == currentName)
	 return (-1*(i+1));
   }
   for (int i = stopName.size()-1; i<= stopLex.size(); i--){
      if (stopLex[i] == currentLex)
	 return i+1;
   }
   return 0;
}

void Parser::work(Administration &ad){
// to get admin to report errors and get tokens
   //done this way as parser is created before the admin
   admin = &ad;
   stopName.push_back("ENDOFFILE"); //setting up part of the follow sets
   stopName.push_back("DOT");

  
   adv(); // loads first Token
   if (!block()){
      //calls block and returns true if good and false if a unhandled error
	 return;
     
   }
   adv();
// should be on to the last token as only way it gets here is if block was good so looking at a end
   if(currentLex!= ".") 
      Verror();
   stopName.pop_back(); //pop EOF
   stopName.pop_back(); //pop DOT
   //pops things off the vectors as we are above there level now
   return;
}

bool Parser::block(){
   // setting the size for everything that was before us
   // so if in between these values a parent caused the error and we pass it up
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();

   //setting up our handling

   stopLex.push_back("begin");


   if (currentLex!="begin"){
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 //only gets in here if the catch was added in before we enterd block
	 //so EOF or "."
	 stopLex.pop_back(); //pop begin
	 return false;
      }
      // this would cycle *** begin past the 3 *'s and stops at begin
   }
   stopLex.pop_back(); // pop begin
   stopLex.push_back("end");

   // setting up first of  statment part
   //as we catch that if defn part get on of them
   stopLex.push_back("skip");
   stopLex.push_back("read");
   stopLex.push_back("write");
   stopLex.push_back("call");
   stopLex.push_back("if");
   stopLex.push_back("do");
   stopName.push_back("ID");  //for assign statment

//advances past Begin
   adv();
   //starts defn part
   // returns true if no unhandled Token false if unhadled at that level
   if(!defnPart()){
      int issue = stop();
      int issue2nd = stop2nd();
      //gets the points in the vectors that have good Tokens
      if (issue <= topName && issue2nd >= topLex){
	 //sees if that token is handled by this level if not return
	 //after pop what this level handles
	 stopName.pop_back(); //pop ID/Assign
	 stopLex.pop_back(); //pop do
	 stopLex.pop_back(); //pop if
	 stopLex.pop_back(); //pop call
	 stopLex.pop_back(); //pop write
	 stopLex.pop_back(); //pop read
	 stopLex.pop_back(); //pop skip
	 stopLex.pop_back(); // pop end

	 
	 return false;
      }
   }
   // removing the first of statment part as this level no longer handles that
   stopName.pop_back(); //pop ID/Assign
   stopLex.pop_back(); //pop do
   stopLex.pop_back(); //pop if
   stopLex.pop_back(); //pop call
   stopLex.pop_back(); //pop write
   stopLex.pop_back(); //pop read
   stopLex.pop_back(); //pop skip

   //same as defn part
   if(!statePart()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back(); //pop end
	 return false;
      }
   }
  
   // to make sure that the end token is at the end of the block part 
   if (currentLex != "end"){
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back(); //pop end
	    return false;
      }
      else
	 adv();
   }
   stopLex.pop_back(); // pop end
   return true;
}

/* every function from here on is generally doing the same as block
   just with differant things pushed and poped, and differant hard comparisons
   like "end"
*/
bool Parser::defnPart(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("const");
   stopLex.push_back("proc");
   stopLex.push_back("integer");
   stopLex.push_back("Boolean");
   stopLex.push_back(";");
   if (currentLex == ";")
      adv();
   if(currentLex == "const"){
      if(constDef()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back(); //pop ;
	    stopLex.pop_back(); //pop boolean
	    stopLex.pop_back(); //pop int
	    stopLex.pop_back(); //pop proc
	    stopLex.pop_back(); //const

	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentLex == "proc"){
      if(!procDef()){ 
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back(); //pop ;
	    stopLex.pop_back(); //pop boolean
	    stopLex.pop_back(); //pop int
	    stopLex.pop_back(); //pop proc
	    stopLex.pop_back(); //const
	    return false;
	 }
	 else
	    adv();
      }

   }
   else if( currentLex == "Boolean" || currentLex == "integer"){
      if(!varibleDef()){
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back(); //pop ;
	    stopLex.pop_back(); //pop boolean
	    stopLex.pop_back(); //pop int
	    stopLex.pop_back(); //pop proc
	    stopLex.pop_back(); //const
	    return false;
	 }
	 else
	    adv();
      }
   }
   else
	 return true;
   if (currentLex == ";"){
      adv();
   }
   if (!defnPart()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back(); //pop ;
	    stopLex.pop_back(); //pop boolean
	    stopLex.pop_back(); //pop int
	    stopLex.pop_back(); //pop proc
	    stopLex.pop_back(); //const
	 return false;
      }
      else
	 adv();
   }
   stopLex.pop_back(); //pop ;
   stopLex.pop_back(); //pop boolean
   stopLex.pop_back(); //pop int
   stopLex.pop_back(); //pop proc
   stopLex.pop_back(); //const
   return true;
}

bool Parser::constDef(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("EQUALS"); // adding equals
   stopName.push_back("ID");
   adv();
   if(!name()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //ID
	 stopName.pop_back();//=
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); //ID
   adv();
   if (currentName != "EQUALS"){
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop equals
	 return false;
      }
      else
	 adv();
   }
   adv();
   if(!constant()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); // equals
	 return false;
      }
      else
	 adv();
   }

   adv();
   stopName.pop_back(); // removing equals
   return true;

}

bool Parser::name(){
   if(currentName == "ID")
      return true;
   else
      Verror();
   return false;
}


bool Parser::constant(){
   if (currentName == "NUM"){
      return true;
   }
   else if( currentLex =="true" || currentLex == "false"){
      return true;
   }
   else if (currentName == "ID"){
      return true;
   }
   return false;

}

bool Parser::procDef(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("ID");
   adv();
   if(!name()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); // ID
	 return false;
      }
      else
	 adv();
   }
   adv();
   stopName.pop_back(); //ID
   if(!block()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }

   adv();
   return true;
}

bool Parser::varibleDef(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   if(currentLex == "array"){
      adv();
      stopName.push_back("LEFTB"); //[
      if(!varList()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); // [
	    return false;
	 }
	 else
	    adv();
      }

      if (currentName != "LEFTB" ){ //[
	 int issue = error();
	 int issue2nd = error2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop [
	    return false;
	 }
	 else
	    adv();
      }
      adv();
      stopName.pop_back(); //poping [
      stopName.push_back("RIGHTB"); //adding ]
      stopName.push_back("NUM");
      stopName.push_back("ID");
      stopLex.push_back("false");
      stopLex.push_back("true");
      if(!constant()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); // ]
	    stopName.pop_back();//num
	    stopName.pop_back();//id
	    stopLex.pop_back();//false
	    stopLex.pop_back();//true
	    return false;
	 }
	 else
	    adv();
      }
      stopName.pop_back();//num
      stopName.pop_back();//id
      stopLex.pop_back();//false
      stopLex.pop_back();//true

      
      adv();
      if(currentLex !="]"){
	 int issue = error();
	 int issue2nd = error2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ]
	    return false;
	 }
	 else
	    adv();
      }

      adv();
      stopName.pop_back(); // pop ]
     
   }
   else
      if(!varList()){
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    return false;
	 }
	 else
	    adv();
      }
   return true;
}

bool Parser::varList(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   if(!name()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }

   adv();
   while(currentLex == ","){
      adv();
      if(!name()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    return false;
	 }
	 else
	    adv();
      }
      adv();
   }
   return true;
}


bool Parser::statePart(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("skip");
   stopLex.push_back("read");
   stopLex.push_back("write");
   stopLex.push_back("call");
   stopLex.push_back("if");
   stopLex.push_back("do");
   stopName.push_back("ID");  //for assign statment
   stopName.push_back("SEMICOLON");
   
   if (currentLex == "end"|| currentLex == "fi" || currentLex == "od"){
      stopName.pop_back(); //pop ID/Assign
      stopLex.pop_back(); //pop do
      stopLex.pop_back(); //pop if
      stopLex.pop_back(); //pop call
      stopLex.pop_back(); //pop write
      stopLex.pop_back(); //pop read
      stopLex.pop_back(); //pop skip
      stopName.pop_back(); // ;
      return true;
   }
   if (currentLex == ";")
      adv();
   if(currentLex == "skip"){
      if(!emptyState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentLex == "read"){
      if(!readState()){
	 
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if( currentLex == "write"){
      if(!writeState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentLex == "call"){
      if(!precedureState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentLex== "if"){
      if(!ifState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentLex == "do"){
      if(!doState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else if (currentName == "ID"){
      if(!assignState()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ID/Assign
	    stopLex.pop_back(); //pop do
	    stopLex.pop_back(); //pop if
	    stopLex.pop_back(); //pop call
	    stopLex.pop_back(); //pop write
	    stopLex.pop_back(); //pop read
	    stopLex.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
	 else
	    adv();
      }
   }
   else {
      stopName.pop_back(); //pop ID/Assign
      stopLex.pop_back(); //pop do
      stopLex.pop_back(); //pop if
      stopLex.pop_back(); //pop call
      stopLex.pop_back(); //pop write
      stopLex.pop_back(); //pop read
      stopLex.pop_back(); //pop skip
      stopName.pop_back(); // ;
      return true;
   }
   if(currentLex== ";")
      adv();
   if(!statePart()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop ID/Assign
	 stopLex.pop_back(); //pop do
	 stopLex.pop_back(); //pop if
	 stopLex.pop_back(); //pop call
	 stopLex.pop_back(); //pop write
	 stopLex.pop_back(); //pop read
	 stopLex.pop_back(); //pop skip
	 stopName.pop_back(); // ;
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); //pop ID/Assign
   stopLex.pop_back(); //pop do
   stopLex.pop_back(); //pop if
   stopLex.pop_back(); //pop call
   stopLex.pop_back(); //pop write
   stopLex.pop_back(); //pop read
   stopLex.pop_back(); //pop skip
   stopName.pop_back(); // ;
   return true;
}

bool Parser:: emptyState(){
   while (!(currentName == "SEMICOLON" || currentName== "DOT" || currentName== "ENDOFFILE")) //;
      adv();
   return true;
}
      
bool Parser::readState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   if(!varAccList()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }
   return true;
}

bool Parser::varAccList(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   if(!varAcc()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }

   while (currentLex == ","){
      adv();
      if(!varAcc()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    return false;
	 }
	 else
	    adv();
      }

   }
   return true;
}

bool Parser::varAcc(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("ID");
   if(!name()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back();
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back();
   adv();
   if (currentLex == "["){
      stopName.push_back("RIGHTB"); // push ]
      adv();
     
      if (!expression()){
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); // ]
	    return false;
	 }
	 else
	    adv();
      }
     
      if (currentLex == "]")
	 adv();
      else{
	 int issue = error();
	 int issue2nd = error2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back(); //pop ]
	    return false;
	 }
	 else
	    adv();
      }
      stopName.pop_back(); //]
   }
   return true;
}


bool Parser::expression(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("&");
   stopLex.push_back("|");
   if(!primeExp()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	  stopLex.pop_back();//&
	  stopLex.pop_back();//|
	  return false;
      }
      else
	 adv();
   }

   while (currentLex == "&" || currentLex == "|"){
      adv();
      if(!primeExp()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();//&
	    stopLex.pop_back();//|
	    return false;
	 }
	 else
	    adv();
      }

   }
   stopLex.pop_back();//&
   stopLex.pop_back();//|
   return true;
}

bool Parser::primeExp(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("<");
   stopLex.push_back("=");
   stopLex.push_back(">");
   if(!simpleExp()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back();//>
	 stopLex.pop_back();//=
	 stopLex.pop_back();//<
	 return false;
      }
      else
	 adv();
   }

   if (currentLex == "<" || currentLex == "=" || currentLex == ">"){
      adv();
      if(!simpleExp()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();//>
	    stopLex.pop_back();//=
	    stopLex.pop_back();//<
	    return false;
	 }
	 else
	    adv();
      }

   }
   stopLex.pop_back();//>
   stopLex.pop_back();//=
   stopLex.pop_back();//<
   return true;
}


bool Parser::simpleExp(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("-");
   stopLex.push_back("+");
   if (currentLex == "-")
      adv();
   if(!term()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back();//-
	 stopLex.pop_back();//+
	 return false;
      }
      else
	 adv();
   }

   while (currentLex == "+"|| currentLex == "-"){
      adv();
      if(!term()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();//-
	    stopLex.pop_back();//+
	    return false;
	 }
	 else
	    adv();
      }

   }
   stopLex.pop_back();//-
   stopLex.pop_back();//+
   return true;
}

bool Parser::term(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("*");
   stopLex.push_back("\\");
   stopLex.push_back("/");
   if(!factor()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back();// "/"
	 stopLex.pop_back();// "\"
	 stopLex.pop_back();// *
	 return false;
      }
      else
	 adv();
   }

   while (currentLex == "*" || currentLex == "/" || currentLex == "\\"){
      adv();
      if(!factor()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// "/"
	    stopLex.pop_back();// "\"
	    stopLex.pop_back();// *
	    return false;
	 }
	 else
	    adv();
      }

   }
   stopLex.pop_back();// "/"
   stopLex.pop_back();// "\"
   stopLex.pop_back();// *
   return true;

}

bool Parser::factor(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("~");
   stopName.push_back("ID");
   stopName.push_back("NUM");
   stopLex.push_back("true");
   stopLex.push_back("false");
   stopLex.push_back("(");
   stopLex.push_back(")");
   if (currentLex == "~"){
      adv();
      if(!factor()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// (
	    stopLex.pop_back();// ~
	    stopName.pop_back(); // ID
	    stopLex.pop_back();// )
	     stopLex.pop_back();// true
	    stopLex.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 else
	    adv();
      }

   }
   else if (currentLex== "("){
      adv();
      if(!expression()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// (
	    stopLex.pop_back();// ~
	    stopName.pop_back(); // ID
	    stopLex.pop_back();// )
	    stopLex.pop_back();// true
	    stopLex.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 else
	    adv();
      }

      if (currentLex == ")")
	 adv();
      else   {
	 int issue = error();
	 int issue2nd = error2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// (
	    stopLex.pop_back();// ~
	    stopName.pop_back(); // ID
	    stopLex.pop_back();// )
	    stopLex.pop_back();// true
	    stopLex.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 else
	    adv();
      }

   }
   else if(currentName == "ID"){
      if(!varAcc()){
       
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// ~
	    stopName.pop_back(); // ID
	    stopLex.pop_back();// (
	    stopLex.pop_back();// )
	    stopLex.pop_back();// true
	    stopLex.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 else
	    adv();
      }
   }
   else {
      if (!constant()){
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();// ~
	    stopName.pop_back(); // ID
	    stopLex.pop_back();// (
	    stopLex.pop_back();// )
	    stopLex.pop_back();// true
	    stopLex.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 else
	    adv();
      }
      adv();
   }
   stopLex.pop_back();// ~
   stopName.pop_back(); // ID
   stopLex.pop_back();// (
   stopLex.pop_back();// )
   stopLex.pop_back();// true
   stopLex.pop_back();// false
   stopName.pop_back(); // NUM
   return true;
}

      
bool Parser::writeState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   if (!expList()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }

   return true;
}

bool Parser::expList(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("ID");
   stopLex.push_back(",");
   if(!expression()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	  stopName.pop_back(); // ID
	  stopLex.pop_back();// ,
	 return false;
      }
      else
	 adv();
   }
   while( currentLex == ","){
      adv();
      if(!expression()){     
	 int issue = stop();
	 int issue2nd = stop2nd();
	 if (issue <= topName && issue2nd >= topLex){
	     stopName.pop_back(); // ID
	     stopLex.pop_back();// ,
	    return false;
	 }
	 else
	    adv();
      }
   }
   stopName.pop_back(); // ID
   stopLex.pop_back();// ,
   
   return true;
}


bool Parser::precedureState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   stopName.push_back("ID");
   if(!name()){
       
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	    stopName.pop_back();//id
	    return false;
      }
      else
	 adv();
   }
   adv();
   return true;
}

bool Parser::ifState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   stopLex.push_back("fi");
   if(!guardedComList()){
       
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back(); //pop fi
	 return false;
      }
      else
	 adv();
   }
   if (currentLex == "fi"){
      adv();
   }
   else{
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back(); //pop fi
	 return false;
      }
      else
	 adv();
   }
   stopLex.pop_back(); // pop fi
   return true;
}

bool Parser::guardedComList(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopLex.push_back("[]");
   if (!guardedCom()){
       
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back();//[]
	 return false;
      }
      else
	 adv();
   }
   while ( currentLex == "[]"){
      adv();
      if (!guardedCom()){
	 
	 int issue = error();
	 int issue2nd = error2nd();
	 if (issue <= topName && issue2nd >= topLex){
	    stopLex.pop_back();//[]
	    return false;
	 }
	 else
	    adv();
      }
   }
   stopLex.pop_back();//[]
   return true;
	 
}

bool Parser::guardedCom(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("ARROW");
   if(!expression()){
       
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop ->
	 return false;
      }
      else
	 adv();
   }
   if (currentLex == "->")
      adv();
   else{
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop ->
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop ->
   if (!statePart()){     
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }
   return true;
}

bool Parser::doState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   adv();
   stopLex.push_back("od");
   if (!guardedComList()){
       
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back(); //pop od
	 return false;
      }
      else
	 adv();
   }
   if (currentLex == "od")
      adv();
   else{
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopLex.pop_back(); //pop od
	 return false;
      }
      else
	 adv();
   }
   stopLex.pop_back(); //pop od
   return true;
}


bool Parser::assignState(){
   int topName = (-1*(stopName.size()));
   int topLex = stopLex.size();
   stopName.push_back("ASSIGN");
   if (!varAccList())   {
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop :=
	 return false;
      }
      else
	 adv();
   }
   if (currentLex == ":=")
      adv();
   else{
      int issue = error();
      int issue2nd = error2nd();
      if (issue <= topName && issue2nd >= topLex){
	 stopName.pop_back(); //pop :=
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop :=
   if (!expList()){
      int issue = stop();
      int issue2nd = stop2nd();
      if (issue <= topName && issue2nd >= topLex){
	 return false;
      }
      else
	 adv();
   }
   return true;
}
