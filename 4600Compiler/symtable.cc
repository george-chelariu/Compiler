#ifndef SYMTABLE_H
#ifndef TOKEN_H
#include "symtable.h"
#include "token.h"


void Symtable::loadResvd(){
   
   int holder;
   holder = insert("begin");
   holder =insert("end");
   holder =insert("const");
   holder = insert("array");
   holder =insert("integer");
   holder =insert("Boolean");
   holder =insert("proc");
   holder =insert("skip");
   holder =insert("read");
   holder =insert("write");
   holder =insert("call");
   holder =insert("if");
   holder =insert("do");
   holder =insert("fi");
   holder =insert("od");
   holder =insert("false");
   holder =insert("true");
}
int Symtable::search(string s){
   int number=0;
   int hash = hashfn(s);
   for(int i= 0; i< SYMTABLESIZE; i++){ // loop to look for the closest hole
      number += hash; // hasing the value
      number = number % SYMTABLESIZE; // making it inside the scope and adding old vaue to it
      if(htable[number].getLexeme() == s)
	 return number;
      else if(&htable[number] == NULL)
	 return -1;
   }
   return -1;
}

int Symtable::insert (string s){
   if (full()) //making sure there is a hole before doing all the math
      return -1; // -1 meaning the table is full
   else{
      int number=0; // holder for the value in the table for the string
      int hash = hashfn(s); // holder for hash value
      for(int i= 0; i< SYMTABLESIZE; i++){ // loop to look for the closest hole
	 number += hash; // increasing thenumber
	 number = number % SYMTABLESIZE; // making it inside the scope and adding old vaue to it
	 if(&htable[number] == NULL){ //looking for hole
	    Token holder(ID, number, s);
	    htable[number] = holder;
	    occupied++;   // incrementing the counter
	    return number;
	 } // end if
      } // end for loop
   } // end else
   occupied = SYMTABLESIZE;
   return -1;
}

void Symtable::printTable(){
   for(int i =0; i< SYMTABLESIZE; i++){
      if (&htable[i] != NULL) 
	 cout << "1: \"" << htable[i].getLexeme() << '\"' << endl;
   }
   return;

}

int Symtable::hashfn(string s){
   int number=0;
   for(size_t i=0; i<s.size();i++){
      number += s[i];
   }

   return number;
}

#endif
#endif
