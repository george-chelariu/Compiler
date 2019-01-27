#include "admin.h"
#ifndef ADMIN_H
#define ADMIN_H

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

int Administration::scan (){
   token holder;
   while (errorCount < MAXERRORS){
      token = scanr.gettoken();
      if (holder.getSymbol() == "NEWLINE")
	 NewLine();
      else if (holder.getSymbol() == "ENDOFFILE")
	 return errorCount;
      else if (holder.getSymbol() == "BADNUM" || "BADNAME" || "BADCHAR")
	 error(holder.getSymbol() + " ScanE");
   }

   return errorCount;
	      
}

#endif 
