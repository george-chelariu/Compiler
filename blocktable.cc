#include "blocktable.h"

//Constructor for table entries
TableEntry::TableEntry(int i, mKind k, int s, mType t, int v, int bl)
{
   index = i;
   kind = k;
   size = s;
   type = t;
   value = v;
   bLevel = bl;
}
BlockTable::BlockTable(int h){	
   blockLevel = 0;
   TableEntry filler(-1, CONSTANT, -1,Universal, -1, -1);
   for(int i = 0; i< MAXBLOCK; i++){
      table[i].push_back(filler);
      table[i].pop_back();
   }


      
};
bool BlockTable::search(int ind){  
   for(int j=0; j < table[blockLevel].size(); j++){
      if(table[blockLevel][j].index == ind)
	 return true;
   }
   return false;
}

void BlockTable::output(){
   cout<< "blocks " << blockLevel<< endl;
   for(int i=MAXBLOCK-1; i >=0; i--)
   {
      for (int j=0; j < table[i].size(); j++)
      {
	 cout << i << " " << j << endl;
	 cout << table[i][j].index << endl;
	 cout << endl;
      }
   }

}


//The define function inserts a TableEntry if the symbol table does not have
//the index that we've passed in
bool BlockTable::define(int index, mKind kind, int size, mType type, int val, int block)
{
   if (!search(index)){
      TableEntry result(index, kind, size, type,val, blockLevel);
      table[blockLevel].push_back(result);
      return true;
   }
   //if we reach this point the index already exists, and we return false
   return false;
}

TableEntry BlockTable::find(int index, bool& error)
{
   for(int i=MAXBLOCK-1; i >= 0; i--)
   {
      for (int j=0; j < table[i].size(); j++)
      {
	 if(table[i][j].index == index)
	 {
	    //return the type from that table location
	    return table[i][j];
	 }
      }
   }
   
   //not finding the table entry means we have an error, so set it to true
   error = true;
   //then return a dummy table entry since it expects a TableEntry
   TableEntry holder (-1, CONSTANT, 0, Universal, -1, 0);
   return holder;
   
}

mType BlockTable::findType(int index, bool& error)
{
   //going through the 2 dimensional table array to search for the
   //index we're passing in
   for(int i=MAXBLOCK-1; i >= 0; i--)
   {
      for (int j=0; j < table[i].size(); j++)
      {
	 if(table[i][j].index == index)
	 {
	    //return the type from that table location
	    return table[i][j].type;
	 }	 
      }  
      }
   //after going through table and finding nothing we deduce there is
   //an error and return the universal type
   error = true;
   return Universal;
}
bool BlockTable::newBlock()
{
   //if we're at the maximum number of blocks, return false, i.e. we can't
   //make any more blocks
   if(blockLevel == MAXBLOCK)
   {
      return false;
   }
   //otherwise we go one block level deeper and create a fresh block table
   blockLevel++;
   

   //then declare the new block successfully made
   return true;
}

bool BlockTable::endBlock()
{
   
   while(!table[blockLevel].empty())
    table[blockLevel].pop_back();
   //decrement the block level, and declade the block successfully ended
   blockLevel--;
   return true;
}

//simple getter function that returns the current level. Mostly for debugging
int BlockTable::currentLevel()
{
   return blockLevel;
}

