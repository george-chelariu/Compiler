#include "admin.h"
#include "token.h"



Administration::Administration(ifstream& in, ofstream &out, Scanner &sc){
   outputfileptr = &out;
   scanr= &sc;
   lineNo = 0;
   correctline = true;
   errorCount = 0;
}

void Administration::error (string text){

   cout << "line: " << lineNo << " " << '\"' << text <<  '\"' << endl;
   correctline = false;
   return;
}



int Administration::scan(){
   Token holder;
   while (errorCount < MAXERRORS){
      holder = scanr->getToken();
      if (holder.getSymbol() == 260)
	 NewLine();
      else if (holder.getSymbol() == 262)
	 return errorCount;
      else if (holder.getSymbol() == 258 || 259 || 272)
	 error(holder.getSymbol() + " ScanE");
      holder.insert(*outputfileptr);
   }

   return errorCount;
	      
}

