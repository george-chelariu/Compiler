#include "blocktable.h"

//Constructor for table entries
BlockTable::TableEntry(int i, mkind k, int s, mType t, int v, int bl)
{
   index = i;
   kind = k;
   size = s;
   type = t;
   value = v;
   bLevel = bl;
}

//The define function inserts a TableEntry if the symbol table does not have
//the index that we've passed in
bool BlockTable::define(int index, mkind kind, int size, mType type, int val, int block)
{
   //As long as we're not at the base block level, we recursively insert
   //more table entries until we've reached the controller
   if (blockLevel != 0){
      if (!search(index))
      {
	 return table[blockLevel].define(index, kind, size, type, val, blockLevel);	 
      }
   }
   //push the defined table entry onto the vector
   else
   {
      BlockEntry.push_back(index, kind, size, type, val, block);
      return true;
   }

   //if we reach this point the index already exists, and we return false
   return false;
}

TableEntry BlockTable::find(int index, bool& error)
{
   //search through the current block entry to see if the indices within
   //match the index passed in
   for(int i=0; i< BlockEntry.size(); i++)
   {
      if (BlockEntry[i].index == index)
      {
	 //return the table entry at the specific block if it's found
	 return BlockEntry[i];
      }
   }
   //otherwise we search through the remaining tables
   for(int i=0; i<table.size(); i++)
   {
      //recursively call find until we get to the deepest level or find the
      //table entry we're looking for
      table[i].find(index, error);
   }
   
   //not finding the table entry means we have an error, so set it to true
   error = true;
   //then return a dummy table entry since it expects a TableEntry
   return (-1, CONSTANT, 0, Universal, -1, 0);
   
}

bool BlockTable::newBlock()
{
   //if we're at the maximum number of blocks, return false, i.e. we can't
   //make any more blocks
   if(table.size() == MAXBLOCK)
   {
      return false;
   }

   //otherwise we go one block level deeper and create a fresh block table
   blockLevel++;
   table[blockLevel] = freshBlockTable();

   //then declare the new block successfully made
   return true;
}

bool BlockTable::endBlock()
{
   //we've implemented an emptyBlock helper function to empty the current block
   table[blockLevel].emptyBlock();
   //decrement the block level, and declade the block successfully ended
   blockLevel--;
   retrun true;
}

//simple getter function that returns the current level. Mostly for debugging
int BlockTable::currentLevel()
{
   return blockLevel;
}

void BlockTable::emptyBlock()
{
   //While the table entry is not empty we pop it's contents off the vector
   while(!TableEntry.empty())
   {
      TableEntry.pop_back();
   }
}
