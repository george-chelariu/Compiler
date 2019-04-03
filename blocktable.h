#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include "typekindenum.h"
#include <iostream>
//typekindenum.h contains the enum declarations for type & kind of objects
#define MAXBLOCK 10
struct TableEntry
{
   TableEntry(int i, mKind k, int s, mType t, int v, int bl);
   int index;  //hash table index of the object
   mKind kind; //array, constant, variable, procedure
   int size;   //only for array
   mType type; //Boolean, integer, universal
   int value;  //only relevant to constants, not variables
   int bLevel; //Block Level, it's the level of the block where the
};             //definitions are encountered
                
   class BlockTable
   {
     public:
      BlockTable(int h);
      ~BlockTable(){};
      //true if ind is found in current block, else go to next level and
      //search there, else false if not found in any deeper level
      bool search (int ind);

      void output();
      
      
      //bool define inserts a TableEntry with necessary field values if index
      //doesn't exist and return true; else return false
      bool define (int ind, mKind kind, int size, mType type, int val, int block);
      //all the fields of TableEntry get passed within the params of define

      //find function looks through the table and returns the table entry
      TableEntry find (int ind, bool& error);

      //findtype is a more specific find function that returns a type
      mType findType (int ind, bool& error);
      
      //creates an empty block & pushes it onto the vector of Block
      bool newBlock();
      
      //pops current block from vector
      bool endBlock();
      
      //return current level (for debugging, mostly)
      int currentLevel();
      
 
      
     private:
      //use an array of vectors for the block table
      vector<TableEntry> table[MAXBLOCK];//BlockEntry;
      //BlockTable table[MAXBLOCK];
      //keep track of current block level
      int blockLevel;
   };

#endif
