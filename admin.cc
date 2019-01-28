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
   errorCount++;
   return;
}



int Administration::scan(){
   Token holder;
   NewLine();
   while (errorCount < MAXERRORS){
      holder = scanr->getToken();
      if (holder.getSymbol() == 260){
	 NewLine();
      }
      else if (holder.getSymbol() == 262){
	    return errorCount;
      }
      else if (holder.getSymbol() == 258 && correctline ==true)
	 error("BADNUM ScanE");
      else if( holder.getSymbol()==259  && correctline == true)
	 error("BADNAME ScanE");
      else if( holder.getSymbol()==271 && correctline== true)
	 error("BADCHAR ScanE");
      if (correctline && holder.getSymbol()!=260)
	 holder.insert(*outputfileptr);
      
   }
   return errorCount;
	      
}
