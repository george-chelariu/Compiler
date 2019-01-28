#ifndef SYMTABLE_H
#define SYMTABLE_H

// Implements a symbol table with linear probe hashing

// Define the size to be the next prime number after 300
#include <string>
#include <vector>
#include "token.h"
#define SYMTABLESIZE 307

using namespace std;

class Symtable{
  public:
    // create a table with SYMTABLESIZE entries
  Symtable():occupied(0), htable(SYMTABLESIZE){
      loadResvd();
   }

    //initialize the table with reserved words

    void loadResvd();

    // Destructor empty so far
    ~Symtable(){}

    // search htable for lexeme s;
    // return the position in the table if found, -1 if not
    int search(string);

    // Insert a token with lexeme s in the symbol table; if already in the
    // table return its location (index); otherwise create a token of
    // appropriate type; insert the token and return its location
    // May need to modify in later phases
    int insert(string s);


    // Return true if the table is full; false otherwise
    bool full(){return occupied == SYMTABLESIZE;}

    // Return the number of occupied cells (used in load factor calculation)
    int getOC(){return occupied;}

    // Print the table on standard output
    // Only for debugging purposes
    void printTable();

  private:
    vector<Token> htable;   // hash table
    int occupied;           // number of cells occupied
    int hashfn(string);     // has function
};
#endif // SYMTABLE_H
