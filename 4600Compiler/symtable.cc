#ifndef SYMTABLE_H
#include "symtable.h"

void Symtable::loadResvd(){
   insert("begin");
   insert("end");
   insert("const");
   insert("array");
   insert("integer");
   insert("Boolean");
   insert("proc");
   insert("skip");
   insert("read");
   insert("write");
   insert("call");
   insert("if");
   insert("do");
   insert("fi");
   insert("od");
   insert("false");
   insert("true");
}
int Symtable::search(string s){
   int number=0;
   int hash = hashfn(s);
   for(int i= 0; i< SYMTABLESIZE; i++){ // loop to look for the closest hole
	 number += hash // hasing the value
	 number = number % SYMTABLESIZE; // making it inside the scope and adding old vaue to it
	 if(htable[number] == string)
	    return number;
	 else if(htable[number] == NULL)
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
	 number += hash; // hasing the value
	 number = number % SYMTABLESIZE; // making it inside the scope and adding old vaue to it
	 if(htable[number] == NULL){ //looking for hole
	    htable[number] = s; // filling hole
	    occupied++;   // incrementing the counter
	    return number;
	 } // end if
      } // end for loop
   } // end else
   occupied = SYMTABLESIZE;
   return -1;
}

void Symtable::insert (string s){
   int filler;
   filler = insert(s);
   return;
}

void Symtable::printTable(){
   for(int i =0; i< SYMTABLESIZE; i++){
      if (htable[i] != NULL) 
	 cout << "1: \"" << htable[i] << '\"' << endl;
   }
   return;

}

int Symtable::hashfn(string s){
   hash<string> str_hash; //setting up hash
   int number = static_cast<int> (str_hash(s)); // doing hash and converting from size_t to int
   if (number< 0) // confirming positive value after convertion
      number = number * -1; // making it positive
   return number;
}

#endif
