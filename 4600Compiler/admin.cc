#include "admin.h"
#include "token.h"



Administration::Administration(ifstream& in, ofstream &out, Scanner &sc){
   outputfileptr = &out;
   scanr= &sc;
   lineNo = 0;
   correctline = true;
   errorCount = 0;
}


void Administration::NewLine(){
 
       lineNo++;
       correctline = true;
  *outputfileptr <<"LINE "<< lineNo << " : " << endl; 
    }
void Administration::error (string text){

   cout << "line: " << lineNo << " " << '\"' << text <<  '\"' << endl;
   correctline = false;
   return;
}



int Administration::scan(){
   Token holder;
   NewLine();
   while (errorCount < MAXERRORS){
      holder = scanr->getToken();
      if (holder.getSymbol() == 260){
	 NewLine();
	 cout << "afjsbdfjkasf";
      }
      else if (holder.getSymbol() == 262){
	 cout << "asfgsadf";
	    return errorCount;
      }
      else if (holder.getSymbol() == 258 || holder.getSymbol()==259 || holder.getSymbol()==272)
	 error(holder.getSymbol() + " ScanE");
      holder.insert(*outputfileptr);
      
   }
   cout << errorCount;
   return errorCount;
	      
}
