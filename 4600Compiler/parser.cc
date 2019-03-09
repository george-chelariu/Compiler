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
   current = input.getSymbol();
   return;
}

int Parser::error(){
   //setting up what typpe of error
   string text = "ParseE ";
   // if ID we whant the Lex on the error report
   if(current == 256) //ID
      text.append(input.getLexeme());
   // otherwise we want the Name as that gives more info
   else
      text.append(input.myName());
   //sends it out to be reported
   admin->ParseError(text);
   //looks to see if this one is a Token that is handled
   int depth = stop();
   while(depth == 0 & current != ENDOFFILE){ //0 = not handled +num = Lux handled -num Name handled
      adv();
      depth= stop();
      //repeat until a good token is found
   }
   return depth;
   
}

//just error that returns nothing 
void Parser::Verror(){
   bool holder = error();
   return;
}
// looks through all of Lex to see if current Token is handled
//then if Name is hadled
// if not there return 0
int Parser::stop(){
   for (int i = stopName.size()-1; i <= 0; i--){
      if (stopName[i] == current)
	 return i+1;
   }
   return 0;
}


void Parser::work(Administration &ad){
// to get admin to report errors and get tokens
   //done this way as parser is created before the admin
   admin = &ad;
   stopName.push_back(ENDOFFILE); //setting up part of the follow sets
   stopName.push_back(DOT);  
   adv(); // loads first Token
   if (!block()){
      //calls block and returns true if good and false if a unhandled error
      stopName.pop_back();
      stopName.pop_back();
      return;
     
   }
 
   adv();
// should be on to the last token as only way it gets here is if block was good so looking at a end


  
   if(current!= DOT) 
      Verror();
   stopName.pop_back(); //pop EOF
   stopName.pop_back(); //pop DOT
   //pops things off the vectors as we are above there level now
   return;
}

bool Parser::block(){
   // setting the size for everything that was before us
   // so if in between these values a parent caused the error and we pass it up
   int topName = stopName.size();

   //setting up our handling

   stopName.push_back(BEGIN);
   if (current!=BEGIN){
     if(error() <= topName){
	 //only gets in here if the catch was added in before we enterd block
	 //so EOF or "."
	 stopName.pop_back(); //pop begin
	 return false;
      }
      // this would cycle *** begin past the 3 *'s and stops at begin
   }
   stopName.pop_back(); // pop begin
   stopName.push_back(END);

   // setting up first of  statment part
   //as we catch that if defn part get on of them
   stopName.push_back(SKIP);
   stopName.push_back(READ);
   stopName.push_back(WRITE);
   stopName.push_back(CALL);
   stopName.push_back(IF);
   stopName.push_back(DO);
   stopName.push_back(ID);  //for assign statment

//advances past Begin
   adv();
   //starts defn part
   // returns true if no unhandled Token false if unhadled at that level
   if(!defnPart()){
      int issue = stop();
      //gets the points in the vectors that have good Tokens
      if (issue <= topName){
	 //sees if that token is handled by this level if not return
	 //after pop what this level handles
	 stopName.pop_back(); //pop ID/Assign
	 stopName.pop_back(); //pop do
	 stopName.pop_back(); //pop if
	 stopName.pop_back(); //pop call
	 stopName.pop_back(); //pop write
	 stopName.pop_back(); //pop read
	 stopName.pop_back(); //pop skip
	 stopName.pop_back(); // pop end

	 
	 return false;
      }
   }
   // removing the first of statment part as this level no longer handles that
   stopName.pop_back(); //pop ID/Assign
   stopName.pop_back(); //pop do
   stopName.pop_back(); //pop if
   stopName.pop_back(); //pop call
   stopName.pop_back(); //pop write
   stopName.pop_back(); //pop read
   stopName.pop_back(); //pop skip

   //same as defn part
   if(!statePart()){
     if(stop() <= topName){
	 stopName.pop_back(); //pop end
	 return false;
      }
   }
  
   // to make sure that the end token is at the end of the block part 
   if (current != END){
     if(error() <= topName){
	    stopName.pop_back(); //pop end
	    return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop end
   return true;
}

/* every function from here on is generally doing the same as block
   just with differant things pushed and poped, and differant hard comparisons
   like "end"
*/
bool Parser::defnPart(){
   int topName = stopName.size();
   stopName.push_back(CONST);
   stopName.push_back(PROC);
   stopName.push_back(INT);
   stopName.push_back(BOOLEAN);
   stopName.push_back(SEMICOLON);
 


 
   if (current == SEMICOLON)
      adv();
   if(current == CONST){
      if(!constDef()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ;
	    stopName.pop_back(); //pop boolean
	    stopName.pop_back(); //pop int
	    stopName.pop_back(); //pop proc
	    stopName.pop_back(); //const

	    return false;
	 }
      }
   }
   else if (current == PROC){
      if(!procDef()){ 
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ;
	    stopName.pop_back(); //pop boolean
	    stopName.pop_back(); //pop int
	    stopName.pop_back(); //pop proc
	    stopName.pop_back(); //const
	    return false;
	 }
      }

   }
   else if( current == BOOLEAN || current == INT){
      if(!varibleDef()){
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ;
	    stopName.pop_back(); //pop boolean
	    stopName.pop_back(); //pop int
	    stopName.pop_back(); //pop proc
	    stopName.pop_back(); //const
	    return false;
	 }
      }
   }
   else{
      stopName.pop_back(); //pop ;
      stopName.pop_back(); //pop boolean
      stopName.pop_back(); //pop int
      stopName.pop_back(); //pop proc
      stopName.pop_back(); //const
      return true;
   }
   stopName.pop_back(); //pop ;
   stopName.pop_back(); //pop boolean
   stopName.pop_back(); //pop int
   stopName.pop_back(); //pop proc
   stopName.pop_back(); //const
   if (!defnPart()){
       
     if(stop() <= topName){
	 return false;
      }
   }

   return true;
}

bool Parser::constDef(){
   int topName = stopName.size();
   stopName.push_back(EQUALS); // adding equals
   stopName.push_back(ID);
   adv();
   if(!name()){
       
     if(stop() <= topName){
	 stopName.pop_back(); // = 
	 stopName.pop_back(); //ID
	 return false;
      }
   }
   
   stopName.pop_back(); //ID
   if (current==ID)
      adv();
   if (current != EQUALS){
     if(error() <= topName){
	 stopName.pop_back(); //pop equals
	 return false;
      }
   }
   adv();
   stopName.pop_back(); //equals
   stopName.push_back(ID);
   stopName.push_back(NUM);
   stopName.push_back(TRUE);
   stopName.push_back(FALSE);
   if(!constant()){
       
     if(stop() <= topName){
	 stopName.pop_back();//num
	 stopName.pop_back();//ID
	 stopName.pop_back();// true
	 stopName.pop_back();// false
	 return false;
      }
   }
   stopName.pop_back();//num
   stopName.pop_back();//ID
   stopName.pop_back();// true
   stopName.pop_back();// false
   adv();
   return true;

}

bool Parser::name(){
   if(current == ID)
      return true;
   else
      Verror();
   return false;
}


bool Parser::constant(){
   if (current == NUM){
      return true;
   }
   else if( current == TRUE || current == FALSE){
      return true;
   }
   else if (current == ID){
      return true;
   }
   return false;

}

bool Parser::procDef(){
   int topName = stopName.size();
   stopName.push_back(ID);
   adv();
   if(!name()){
     if(stop() <= topName){
	 stopName.pop_back(); // ID
	 return false;
      }
   }
   adv();
   stopName.pop_back(); //ID
   
   if(!block()){
     if(stop() <= topName){
	 return false;
      }
   }
   adv();
   return true;
}

bool Parser::varibleDef(){
   int topName = stopName.size();
   adv();
   if(current == ARRAY){
      adv();
      stopName.push_back(LEFTB); //[
      if(!varList()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); // [
	    return false;
	 }
      }

      if (current != LEFTB ){ //[
	 int issue = error();
	 if (issue <= topName){
	    stopName.pop_back(); //pop [
	    return false;
	 }
      }
      adv();
      stopName.pop_back(); //poping [
      stopName.push_back(RIGHTB); //adding ]
      stopName.push_back(NUM);
      stopName.push_back(ID);
      stopName.push_back(FALSE);
      stopName.push_back(TRUE);
      if(!constant()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); // ]
	    stopName.pop_back();//num
	    stopName.pop_back();//id
	    stopName.pop_back();//false
	    stopName.pop_back();//true
	    return false;
	 }
      }
      stopName.pop_back();//num
      stopName.pop_back();//id
      stopName.pop_back();//false
      stopName.pop_back();//true

      
      adv();
      if(current !=RIGHTB){

	 if (error() <= topName){
	    stopName.pop_back(); //pop ]
	    return false;
	 }
      }

      adv();
      stopName.pop_back(); // pop ]
     
   }
   else{
      if(!varList()){
	 int issue = stop();
	 if (issue <= topName){
	    return false;
	 }
      }
   }
   return true;
}

bool Parser::varList(){
   int topName = stopName.size();
   stopName.push_back(ID);//num
   stopName.push_back (COMMA); //,
   if(!name()){
     if(stop() <= topName){
	 stopName.pop_back(); //ID
	 stopName.pop_back(); //,
	 return false;
      }
   }
   
   if( current==ID){
      adv();
   }
   while(current == COMMA){
      adv();
      if(!name()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //ID
	    stopName.pop_back(); //,
	    return false;
	 }
      }
      
      if (current==ID)
	 adv();
   }
   stopName.pop_back(); //ID
   stopName.pop_back(); //,
   return true;
}


bool Parser::statePart(){
   int topName = stopName.size();
   
   stopName.push_back(SKIP);
   stopName.push_back(READ);
   stopName.push_back(WRITE);
   stopName.push_back(CALL);
   stopName.push_back(IF);
   stopName.push_back(DO);
   stopName.push_back(ID);  //for assign statment
   stopName.push_back(SEMICOLON);

   
   if (current == SEMICOLON)
      adv();
   if(current == SKIP){
      if(!emptyState()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if (current == READ){
      if(!readState()){
	 
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if( current == WRITE){
      if(!writeState()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if (current == CALL){
      if(!precedureState()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if (current== IF){
      if(!ifState()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if (current == DO){
      if(!doState()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else if (current == ID){
      if(!assignState()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //pop ID/Assign
	    stopName.pop_back(); //pop do
	    stopName.pop_back(); //pop if
	    stopName.pop_back(); //pop call
	    stopName.pop_back(); //pop write
	    stopName.pop_back(); //pop read
	    stopName.pop_back(); //pop skip
	    stopName.pop_back(); // ;
	    return false;
	 }
      }
   }
   else {
        stopName.pop_back(); //pop ID/Assign
	stopName.pop_back(); //pop do
	stopName.pop_back(); //pop if
	stopName.pop_back(); //pop call
	stopName.pop_back(); //pop write
	stopName.pop_back(); //pop read
	stopName.pop_back(); //pop skip
	stopName.pop_back(); // ;
	return true;
   }
   stopName.pop_back(); //pop ID/Assign
   stopName.pop_back(); //pop do
   stopName.pop_back(); //pop if
   stopName.pop_back(); //pop call
   stopName.pop_back(); //pop write
   stopName.pop_back(); //pop read
   stopName.pop_back(); //pop skip
   stopName.pop_back(); // ;
   if(!statePart()){
       
     if(stop() <= topName){
	 return false;
      }
   }

   return true;
}

bool Parser:: emptyState(){
   while (!(current == SEMICOLON || current== DOT || current== ENDOFFILE)) //;
      adv();
   return true;
}
      
bool Parser::readState(){
   int topName = stopName.size();
   adv();
   if(!varAccList()){
     if(stop() <= topName){
	 return false;
      }
      else
	 adv();
   }
   return true;
}

bool Parser::varAccList(){
   int topName = stopName.size();
   stopName.push_back(COMMA);
   if(!varAcc()){
     if(stop() <= topName){
	 stopName.pop_back(); //,
	 return false;
      }
 
   }

   while (current == COMMA){
      adv();
      if(!varAcc()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); //,
	    return false;
	 }
      }

   }
   stopName.pop_back();//,
   return true;
}

bool Parser::varAcc(){
   int topName = stopName.size();
   stopName.push_back(ID);
   stopName.push_back(LEFTB);
   if(!name()){
       
     if(stop() <= topName){
	 stopName.pop_back(); //ID
	 stopName.pop_back(); //[
	 return false;
      }
   }
   stopName.pop_back(); //ID
   stopName.pop_back(); //[
   if (current ==ID)
      adv();
   if (current == LEFTB){
      stopName.push_back(RIGHTB); // push ]
      adv();
     
      if (!expression()){
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back(); // ]
	    return false;
	 }
      }
      
      if (current != RIGHTB){
	 int issue = error();
	 if (issue <= topName ){
	    stopName.pop_back(); //pop ]
	    return false;
	 }
      }
      stopName.pop_back(); //]
      adv();
      
      
   }

   return true;
}


bool Parser::expression(){
   int topName = stopName.size();
   stopName.push_back(AND);
   stopName.push_back(OR);
   if(!primeExp()){
     if(stop() <= topName ){
	  stopName.pop_back();//&
	  stopName.pop_back();//|
	  return false;
      }
   }

   while (current == AND || current == OR){
      adv();
      if(!primeExp()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();//&
	    stopName.pop_back();//|
	    return false;
	 }
      }

   }
   stopName.pop_back();//&
   stopName.pop_back();//|
   return true;
}

bool Parser::primeExp(){
   int topName = stopName.size();
   stopName.push_back(GREATERTHAN);
   stopName.push_back(EQUALS);
   stopName.push_back(LESSTHAN);
   if(!simpleExp()){
       
     if(stop() <= topName){
	 stopName.pop_back();//>
	 stopName.pop_back();//=
	 stopName.pop_back();//<
	 return false;
      }
   }
  
   if (current == GREATERTHAN || current == EQUALS ||current == LESSTHAN){
      adv();
      if(!simpleExp()){
       
	 int issue = stop();
	 if (issue <= topName){
	     stopName.pop_back();//>
	     stopName.pop_back();//=
	     stopName.pop_back();//<
	    return false;
	 }
	 else
	    adv();

      }

   }
   stopName.pop_back();//>
   stopName.pop_back();//=
   stopName.pop_back();//<

   return true;
}


bool Parser::simpleExp(){
   int topName = stopName.size();
   stopName.push_back(MINUS);
   stopName.push_back(PLUS);
   if (current == MINUS)
      adv();
   if(!term()){     
      if(stop() <= topName ){
	 stopName.pop_back();//-
	 stopName.pop_back();//+
	 return false;
      }
   }

   while (current == PLUS||current == MINUS){
      adv();
      if(!term()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();//-
	    stopName.pop_back();//+
	    return false;
	 }
      }

   }
   stopName.pop_back();//-
   stopName.pop_back();//+
   return true;
}

bool Parser::term(){
   int topName = stopName.size();
   stopName.push_back(TIMES);
   stopName.push_back(DIVIDE);
   stopName.push_back(MODULUS);
   if(!factor()){
       
     if(stop() <= topName){
	 stopName.pop_back();// "/"
	 stopName.pop_back();// "\"
	 stopName.pop_back();// *
	 return false;
      }
   }

   while (current ==TIMES|| current ==DIVIDE||current == MODULUS){
      adv();
      if(!factor()){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back();// "/"
	    stopName.pop_back();// "\"
	    stopName.pop_back();// *
	    return false;
	 }
      }

   }
   stopName.pop_back();// "/"
   stopName.pop_back();// "\"
   stopName.pop_back();// *
   return true;

}

bool Parser::factor(){
   int topName = stopName.size();
   if (current == NOT){
      adv();
      if(!factor()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    return false;
	 }
      }

   }
   else if (current==LEFTP){
      adv();
      stopName.push_back(RIGHTP); //)
      if(!expression()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();// )
	    return false;
	 }
      }

      if (current ==RIGHTP)
	 adv();
      else   {
	 int issue = error();
	 if (issue <= topName){
	    stopName.pop_back();// )
	    return false;
	 }	 
      }
      stopName.pop_back(); // )

   }
   else if(current == ID){
      if(!varAcc()){
       
	 int issue = stop();
	 if (issue <= topName ){
	    return false;
	 }
      }
   }
   else {
      stopName.push_back(ID);
      stopName.push_back(NUM);
      stopName.push_back(TRUE);
      stopName.push_back(FALSE);
      if (!constant()){
	 int issue = stop();;
	 if (issue <= topName){
	    stopName.pop_back(); // ID
	    stopName.pop_back();// true
	    stopName.pop_back();// false
	    stopName.pop_back(); // NUM
	    return false;
	 }
	 

      }
      stopName.pop_back(); // ID
      stopName.pop_back();// true
      stopName.pop_back();// false
      stopName.pop_back(); // NUM
      adv();
   }
   return true;
}

      
bool Parser::writeState(){

   int topName = stopName.size();
   adv();
   if (!expList()){
       
     if(stop() <= topName){
	 return false;
      }
   }

   return true;
}

bool Parser::expList(){

   int topName = stopName.size();
   stopName.push_back(ID);
   stopName.push_back(COMMA);
   if(!expression()){
     if(stop() <= topName ){
	  stopName.pop_back(); // ID
	  stopName.pop_back();// ,
	 return false;
      }
      if (current == ID)
	 adv();
   }
   while( current == COMMA){
      adv();
      if(!expression()){     
	 int issue = stop();
	 if (issue <= topName ){
	     stopName.pop_back(); // ID
	     stopName.pop_back();// ,
	    return false;
	 }
	 if(current ==ID)
	    adv();
      }
   }
   stopName.pop_back(); // ID
   stopName.pop_back();// ,
   return true;
}


bool Parser::precedureState(){
   int topName = stopName.size();
   adv();
   stopName.push_back(ID);
   if(!name()){
       
     if(stop() <= topName ){
	    stopName.pop_back();//id
	    return false;
      }
   }
   stopName.pop_back(); //ID
   adv();
   return true;
}

bool Parser::ifState(){
   int topName = stopName.size();
   adv();
   stopName.push_back(FI);
   if(!guardedComList()){
       
     if(error() <= topName){
	 stopName.pop_back(); //pop fi
	 return false;
      }
   }
   if (current == FI){
      adv();
   }
   else{
     if(error() <= topName ){
	 stopName.pop_back(); //pop fi
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop fi
   return true;
}

bool Parser::guardedComList(){
   int topName = stopName.size();
   stopName.push_back(DOUBLEB);
   if (!guardedCom()){
       
     if(error() <= topName ){
	 stopName.pop_back();//[]
	 return false;
      }
   }
   while ( current == DOUBLEB){
      adv();
      if (!guardedCom()){
	 
	 int issue = error();
	 if (issue <= topName){
	    stopName.pop_back();//[]
	    return false;
	 }
      }
   }
   stopName.pop_back();//[]
   return true;
	 
}

bool Parser::guardedCom(){
   int topName = stopName.size();
   stopName.push_back(ARROW);
   if(!expression()){
       
     if(error() <= topName){
	 stopName.pop_back(); //pop ->
	 return false;
      }
   }
   if (current == ARROW)
      adv();
   else{
     if(error() <= topName){
	 stopName.pop_back(); //pop ->
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop ->
   if (!statePart()){     
     if(error() <= topName){
	 return false;
      }
   }
   return true;
}

bool Parser::doState(){
   int topName = stopName.size();
   adv();
   stopName.push_back(OD);
   if (!guardedComList()){
       
     if(error() <= topName ){
	 stopName.pop_back(); //pop od
	 return false;
      }
   }
   if (current == OD)
      adv();
   else{
     if(error() <= topName ){
	 stopName.pop_back(); //pop od
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); //pop od
   return true;
}


bool Parser::assignState(){
   int topName = stopName.size();
   stopName.push_back(ASSIGN);
   if (!varAccList())   {
     if(error() <= topName){
	 stopName.pop_back(); //pop :=
	 return false;
      }
   }
   if (current == ASSIGN)
      adv();
   else{
     if(error() <= topName){
	 stopName.pop_back(); //pop :=
	 return false;
      }
      else
	 adv();
   }
   stopName.pop_back(); // pop :=
   if (!expList()){
     if(stop() <=  topName){
	 return false;
      }
   }
   return true;
}
