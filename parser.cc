#include <iostream>

#include "parser.h"
#include "admin.h"
#include "blocktable.h"

/*a lot of repeated things so mainly commeting on the unique things so dont have to add 1000+
  comments that mainly reapeat on sections of code
  will have something like this when "done"
*/

using namespace std;

//constructor that gets called when we actually do things with it
//errors out without it
Parser::Parser(BlockTable *h){
   Table =h;
   labelNumber = 0;
}

// function that gets the next token and puts the info in the holders
void Parser::adv(){
   // get returns the next valid token, reuses a lot of the old scanner code
   input = admin-> get();
   current = input.getSymbol();
   return;
}

int Parser::newLabel(){
   labelNumber++; 
   if (labelNumber > MAXLABEL) 
      admin->fatal("Exceeded maximum labels in code generation...Bailing out");
   return labelNumber;
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
void Parser::typeError(){
   bool holder;
   admin->TypeError(Table->find(input.getValue(), holder));
   return;
}

void Parser::work(Administration &ad){
// to get admin to report errors and get tokens
   //done this way as parser is created before the admin
   admin = &ad;
   
   int varibles, start;
   varibles= newLabel();
   start = newLabel();
   admin->emit3("PROG", varibles, start);
   if (!Table->newBlock()){
      typeError();
      return;
   }
   stopName.push_back(ENDOFFILE); //setting up part of the follow sets
   stopName.push_back(DOT);  
   adv(); // loads first Token
   if (!block(varibles, start)){
      //calls block and returns true if good and false if a unhandled error
      stopName.pop_back();
      stopName.pop_back();
      return;
      
   }
   
   if (!Table->endBlock()){
      typeError();
      return;
   }
   adv();
// should be on to the last token as only way it gets here is if block was good so looking at a end


  
   if(current!= DOT) 
      Verror();
   admin->emit1("ENDPROG");
   stopName.pop_back(); //pop EOF
   stopName.pop_back(); //pop DOT
   //pops things off the vectors as we are above there level now
   return;
}

bool Parser::block(int varend, int start){
   // setting the size for everything that was before us
   // so if in between these values a parent caused the error and we pass it up
   int varibles=0;
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
   int offset = 3;
   if(!defnPart(varend, varibles, offset)){
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

//decide stuff here for start
   admin->emit3("DEFARG", varend, varibles); 
   admin->emit2("DEFADDR", start);





   


   
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
bool Parser::defnPart(int startoffset, int& varibles, int& offset){
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
   else if(current == BOOLEAN || current == INT){
      if(!varibleDef(varibles, offset)){
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
   if (!defnPart(startoffset, varibles, offset)){
       
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
   int HTable= -1;
   int value= -1;
   bool typeerror = false;
   mType type= Universal;
   adv();
   if(!name()){
       
     if(stop() <= topName){
	 stopName.pop_back(); // = 
	 stopName.pop_back(); //ID
	 return false;
      }
   }
   
   stopName.pop_back(); //ID
   if (current==ID){
      HTable=input.getValue();
      adv();
   }
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
   switch(current){
      case NUM:
	 type= Integer;
	 value= input.getValue();
	 break;
      case ID:
	 type = Table->findType(input.getValue(), typeerror);
	 break;
      case TRUE:
	 type = Boolean;
	 value = 1;
	 break;
      case FALSE:
	 type=Boolean;
	 value = 0;
	 break;
   }
   if(!Table->define(HTable, CONSTANT, 0, type, value, 0, 0)){
      typeError();
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
   bool flag = false;
   stopName.push_back(ID);
   adv();
   if(!name()){
     if(stop() <= topName){
	 stopName.pop_back(); // ID
	 return false;
      }
   }
   int procLabel = newLabel();
   if (!(Table->define(input.getValue(),PROCEDURE, 0, Universal, 0, 0 ,procLabel)))
      typeError();
      
   if(!Table->newBlock()){
      typeError();
      flag = true;
   }
   // at name
   adv();
   stopName.pop_back(); //ID
   int varLabel = newLabel();
   int startLabel = newLabel();
   admin->emit2("DEFADDR", procLabel);
   admin->emit3("PROC", varLabel, startLabel);
   
   if(!block(varLabel, startLabel)){
      if(stop() <= topName){
	 if (!Table->endBlock())
	  typeError();
	 return false;
      }
   }
   if (flag == false)
      if (!Table->endBlock())
	 typeError();
   admin->emit1("ENDPROC");
   adv();
   return true;
}

bool Parser::varibleDef(int& varibles, int& varstart){
   
   int topName = stopName.size();
   mType type;
   vector<int> names;
   switch (current){
      case BOOLEAN:
	 type = Boolean;
	 break;
      case INT:
	 type = Integer;
	 break;
   }
   adv();
   if(current == ARRAY){
      adv();
      stopName.push_back(LEFTB); //[
      if(!varList(names)){
       
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

      varibles += names.size() * input.getValue();
// here
      while(!names.empty()){	    
	 if (!Table->define(names.back(),ARR, input.getValue(),type, 0, 0, varstart)){
	    typeError();
	 }
	 varstart+=input.getValue();
	 names.pop_back();
      }
      adv();
      if(current !=RIGHTB){ //]

	 if (error() <= topName){
	    stopName.pop_back(); //pop ]
	    return false;
	 }
      }

      adv();
      stopName.pop_back(); // pop ]
     
   }
   else{
      if(!varList(names)){
	 int issue = stop();
	 if (issue <= topName){
	    return false;
	 }
      }
      else{
	 varibles += names.size();
	 while( !names.empty()){	    
	    if (!Table->define(names.back(),VARIABLE,0 ,type, 0, 0, varstart))
	       typeError();	   
	    varstart++;
	     names.pop_back();
	 }
	  
	    
      }
   }
   return true;
}

bool Parser::varList(vector<int>& names){
   int topName = stopName.size();
   stopName.push_back(ID);//ID
   stopName.push_back (COMMA); //,
   if(!name()){
     if(stop() <= topName){
	 stopName.pop_back(); //ID
	 stopName.pop_back(); //,
	 return false;
      }
   }
   
   if( current==ID){
      names.push_back(input.getValue());
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
      
      if (current==ID){
	 names.push_back(input.getValue());
	 adv();
      }
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
   vector<mType> holder;
   adv();
   if(!varAccList(holder)){
     if(stop() <= topName){
	 return false;
      }
      else
	 adv();
   }
   admin->emit2("READ", holder.size());
   return true;
}

bool Parser::varAccList(vector<mType>& holder){
   int topName = stopName.size();
   bool error = false;
   stopName.push_back(COMMA);
   //here
   holder.push_back(Table->findType(input.getValue(), error));
   if (error){
      holder.pop_back();
      typeError();
   }
   if(!varAcc(holder.back())){
     if(stop() <= topName){
	 stopName.pop_back(); //,
	 return false;
      }
 
   }

   while (current == COMMA){
      adv();
      //here
      holder.push_back(Table->findType(input.getValue(), error));
      if (error){
	 holder.pop_back();
	 typeError();
      }
      if(!varAcc(holder.back())){
       
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

bool Parser::varAcc( mType& exp){
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
   bool error1 = false;
   TableEntry temp = Table->find(input.getValue(), error1);
   if (error1)
      typeError();
   stopName.pop_back(); //ID
   stopName.pop_back(); //[
   admin->emit3("VARIABLE", Table->currentLevel() - temp.bLevel, temp.displacement);
   if (current ==ID)
      adv();
   if (current == LEFTB){
      stopName.push_back(RIGHTB); // push ]
      adv();
      int holder = admin->getlineNumber();
      if (!expression(exp)){
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back(); // ]
	    return false;
	 }
      }
      admin->emit3("INDEX", temp.size, holder);
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


bool Parser::expression(mType& expholder){
   int topName = stopName.size();
   string type;
   stopName.push_back(AND);
   stopName.push_back(OR);
   if(!primeExp(expholder)){
     if(stop() <= topName ){
	  stopName.pop_back();//&
	  stopName.pop_back();//|
	  return false;
      }
   }

   while (current == AND || current == OR){
    
      if (current == AND)
	 type= "AND";	 
      else
	 type = "OR";
      adv();
      if(!primeExp(expholder)){      
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();//&
	    stopName.pop_back();//|
	    expholder = Boolean;
	    return false;
	 }
      }
      expholder = Boolean;
      admin->emit1(type);
   }
   stopName.pop_back();//&
   stopName.pop_back();//|
   return true;
}

bool Parser::primeExp(mType& expholder){
   int topName = stopName.size();
   string type;
   stopName.push_back(GREATERTHAN);
   stopName.push_back(EQUALS);
   stopName.push_back(LESSTHAN);
   if(!simpleExp(expholder)){
       
     if(stop() <= topName){
	 stopName.pop_back();//>
	 stopName.pop_back();//=
	 stopName.pop_back();//<
	 return false;
      }
   }
  
   if (current == GREATERTHAN || current == EQUALS ||current == LESSTHAN){
      if (current == LESSTHAN)
	 type= "LESS";
      else if (current == EQUALS)
	 type= " EQUAL";
      else
	 type= "GREATER";
      adv();
      if(!simpleExp(expholder)){
       
	 int issue = stop();
	 if (issue <= topName){
	     stopName.pop_back();//>
	     stopName.pop_back();//=
	     stopName.pop_back();//<
	     expholder = Boolean;
	    return false;
	 }
	 else
	    adv();

      }
      expholder= Boolean;
      admin->emit1(type);
   }
   stopName.pop_back();//>
   stopName.pop_back();//=
   stopName.pop_back();//<

   return true;
}


bool Parser::simpleExp(mType& expholder){
   int topName = stopName.size();
   stopName.push_back(MINUS);
   stopName.push_back(PLUS);
   string type;
   if (current == MINUS)
      adv();
   if(!term(expholder)){     
      if(stop() <= topName ){
	 stopName.pop_back();//-
	 stopName.pop_back();//+
	 return false;
      }
   }

   while (current == PLUS||current == MINUS){
      if (current == PLUS)
	 type = "ADD";
      else
	 type = "SUBTRACT";
      adv();
      if(!term(expholder)){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();//-
	    stopName.pop_back();//+
	    expholder = Integer;
	    return false;
	 }
      }
      expholder = Integer;
      admin->emit1(type);
   }
   stopName.pop_back();//-
   stopName.pop_back();//+
   return true;
}

bool Parser::term(mType& expholder){
   int topName = stopName.size();
   string type;
   stopName.push_back(TIMES);
   stopName.push_back(DIVIDE);
   stopName.push_back(MODULUS);
   if(!factor(expholder)){
       
     if(stop() <= topName){
	 stopName.pop_back();// "/"
	 stopName.pop_back();// "\"
	 stopName.pop_back();// *
	 return false;
      }
   }

   while (current ==TIMES|| current ==DIVIDE||current == MODULUS){
      if (current == TIMES)
	 type = "MULTIPLY";
      else if (current == DIVIDE)
	 type = "DIVIDE";
      else
	 type = "MODULO";
      adv();
      if(!factor(expholder)){
       
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back();// "/"
	    stopName.pop_back();// "\"
	    stopName.pop_back();// *
	    expholder = Integer;
	    return false;
	 }
      }
      expholder = Integer;
      admin->emit1(type);
   }
   stopName.pop_back();// "/"
   stopName.pop_back();// "\"
   stopName.pop_back();// *
   return true;

}

bool Parser::factor(mType& expholder){
   int topName = stopName.size();
   if (current == NOT){
     
      adv();
      if(!factor(expholder)){
       
	 int issue = stop();
	 if (issue <= topName ){
	    expholder = Boolean;
	    return false;
	 }
	
      }
      expholder = Boolean;
      admin->emit1("NOT");
   }
   else if (current==LEFTP){
      adv();
      stopName.push_back(RIGHTP); //)
      if(!expression(expholder)){
       
	 int issue = stop();
	 if (issue <= topName ){
	    stopName.pop_back();// )
	    return false;
	 }
      }

      if (current ==RIGHTP)
	 adv();
      else{
	 int issue = error();
	 if (issue <= topName){
	    stopName.pop_back();// )
	    return false;
	 }	 
      }
      stopName.pop_back(); // )

   }
   else if(current == ID){
      bool error = false;
      TableEntry I =Table->find(input.getValue(), error);
      if (error){
	 typeError();
      }
     
      expholder = I.type;
      if(!varAcc(expholder)){
       
	 int issue = stop();
	 if (issue <= topName ){
	    return false;
	 }
      }
      if (I.kind == CONSTANT){
	 admin->emit2("CONSTANT", I.value);

      }
      else{
	 admin->emit1("VALUE");	 
      }
   }
   else {
      stopName.push_back(ID);
      stopName.push_back(NUM);
      stopName.push_back(TRUE);
      stopName.push_back(FALSE);
      if (!constant()){
	 int issue = stop();
	 if (issue <= topName){
	    stopName.pop_back(); // ID
	    stopName.pop_back();// true
	    stopName.pop_back();// false
	    stopName.pop_back(); // NUM
	    expholder = Universal;
	    return false;
	 }
      }
      switch(current){
	 case ID:
	    bool error;
	    error = false;
	    mType I;
	    I =Table->findType(input.getValue(), error);
	    if (!error){
	       expholder = I;
	    }
	    break;
	 case TRUE:
	    admin->emit2("CONSTANT", 1);
	    expholder = Boolean;
	    break;
	 case FALSE:
	    admin->emit2("CONSTANT", 0);
	    expholder= Boolean;
	    break;
	 case NUM:
	    expholder = Integer;
	    admin->emit2("CONSTANT", input.getValue());
	    break;
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
   vector<mType> holder;
   int topName = stopName.size();
   adv();
   if (!expList(holder)){
       
     if(stop() <= topName){
	 return false;
      }
   }
   admin->emit2("WRITE",holder.size());
   return true;
}

bool Parser::expList(vector<mType>& exp){
   int topName = stopName.size();
   stopName.push_back(ID);
   stopName.push_back(COMMA);
   mType holder;
 
   if(!expression(holder)){
     if(stop() <= topName ){
	  stopName.pop_back(); // ID
	  stopName.pop_back();// ,
	 return false;
      }
      if (current == ID)
	 adv();
   }
   exp.push_back(holder);
   while( current == COMMA){
      adv();
      if(!expression(holder)){     
	 int issue = stop();
	 if (issue <= topName ){
	     stopName.pop_back(); // ID
	     stopName.pop_back();// ,
	    return false;
	 }
	 if(current ==ID)
	    adv();
      }
      exp.push_back(holder);
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
//here
   bool error = false;
   TableEntry temp = Table->find(input.getValue(), error);
   if (error)
      typeError();

   admin->emit3("CALL", Table->currentLevel() - temp.bLevel, temp.displacement);
   stopName.pop_back(); //ID
   adv();
   return true;
}

bool Parser::ifState(){
   int topName = stopName.size();
   int startLabel = newLabel(); 
   int doneLabel = newLabel();
   adv();
   stopName.push_back(FI);
   if(!guardedComList(startLabel, doneLabel)){
       
     if(error() <= topName){
	 stopName.pop_back(); //pop fi
	 return false;
      }
   }
   admin->emit2("DEFADDR", startLabel);
   admin->emit2("FI", admin->getlineNumber());
   admin->emit2("DEFADDR", doneLabel);
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

bool Parser::guardedComList(int& startLabel, int jumpLabel){
   int topName = stopName.size();
   stopName.push_back(DOUBLEB);
   if (!guardedCom(startLabel, jumpLabel)){
       
     if(error() <= topName ){
	 stopName.pop_back();//[]
	 return false;
      }
   }
   while ( current == DOUBLEB){
      adv();
      if (!guardedCom(startLabel, jumpLabel)){
	 
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

bool Parser::guardedCom(int& startLabel, int jumpLabel){
   mType exp = Universal;
   int topName = stopName.size();
   admin->emit2("DEFADDR", startLabel);
   stopName.push_back(ARROW);
   if(!expression(exp)){
       
     if(error() <= topName){
	stopName.pop_back(); //pop ->
	if (exp != Boolean)
	   typeError();
	 return false;
      }
   }
   startLabel = newLabel();
   if (exp != Boolean){
      typeError();
   }

   admin->emit2("ARROW", startLabel);
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
   admin->emit2("BAR", jumpLabel);
   return true;
}

bool Parser::doState(){
   int topName = stopName.size();
   int startLabel = newLabel();
   int loopLabel = newLabel();
   admin->emit2("DEFADDR", loopLabel);
   adv();
   stopName.push_back(OD);
   if (!guardedComList(startLabel, loopLabel)){
       
     if(error() <= topName ){
	 stopName.pop_back(); //pop od
	 return false;
      }
   }
   admin->emit2("DEFADDR", startLabel);
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
   vector<mType> assign;
   vector<mType> exp;
   int topName = stopName.size();
   stopName.push_back(ASSIGN);
   if (!varAccList(assign))   {
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
   if (!expList(exp)){
     if(stop() <=  topName){
	 return false;
      }
   }
   admin->emit2("ASSIGN", assign.size());
   if(exp.size() ==assign.size()){
      while(!exp.empty()){
	 if(exp.back() != assign.back())
	    typeError();
	 assign.pop_back();
	 exp.pop_back();
      }
	    
   }
   else
      typeError();

   return true;
}
