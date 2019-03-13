#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include "typekindenum.h"
//typekindenum.h contains the enum declarations for type & kind of objects
#define MAXBLOCK 10
typedef struct TableEntry
{
   TableEntry(int i, mkind k, int s, mType t, int v, int bl);
   int index;  //hash table index of the object
   mkind kind; //array, constant, variable, procedure
   int size;   //only for array
   mType type; //Boolean, integer, universal
   int value;  //only relevant to constants, not variables
   int bLevel; //Block Level, it's the level of the block where the
}              //definitions are encountered
                
   class BlockTable
   {
     public:
      BlockTable();
      ~BlockTable();
      //true if ind is found in current block, else go to next level and
      //search there, else false if not found in any deeper level
      bool search (int ind);
      
      //bool define inserts a TableEntry with necessary field values if index
      //doesn't exist and return true; else return false
      bool define (int ind, mkind kind, int size, mType type, int val, int block);
      //all the fields of TableEntry get passed within the params of define
      
      TableEntry find (int ind, bool& error);
      
      //creates an empty block & pushes it onto the vector of Block
      bool newBlock();
      
      //pops current block from vector
      bool endBlock();
      
      //return current level (for debugging, mostly)
      int currentLevel();
      
      //helper function that empties the block before popping for cleanliness
      void emptyBlock();
      
     private:    
      vector<TableEntry> BlockEntry;
      BlockTable table[MAXBLOCK];
      int blockLevel;
   };

#endif
