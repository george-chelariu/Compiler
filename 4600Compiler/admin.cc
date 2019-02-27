#include "admin.h"
#include "token.h"



Administration::Administration(ifstream& in, ofstream &out, Scanner &sc, Parser &pc){
   outputfileptr = &out;
   scanr= &sc;
   lineNo = 0;
   correctline = true;
   errorCount = 0;
   parsr= &pc;
}


void Administration::NewLine(){
 
       lineNo++;
       correctline = true;
  *outputfileptr <<"LINE "<< lineNo << " : " << endl; 
}

void Administration:: ParseError(string text){
   error(text);
   correctline = true;
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
      else if (correctline){
	 holder.insert(*outputfileptr);
	 
      }
   }
   return errorCount;
	      
}

Token Administration::get(){
   Token holder;
   while(errorCount < MAXERRORS){
      holder = scanr->getToken();
       if (holder.getSymbol() == 260){
	 NewLine();
      }
       else if (holder.getSymbol() == 262){
	    return holder;
      }
      else if (holder.getSymbol() == 258 && correctline ==true)
	 error("BADNUM ScanE");
      else if( holder.getSymbol()==259  && correctline == true)
	 error("BADNAME ScanE");
      else if( holder.getSymbol()==271 && correctline== true)
	 error("BADCHAR ScanE");
      else if (correctline){
	 holder.insert(*outputfileptr);
	 return holder;
      }
   }

   Token ended(ENDOFFILE, -1,"eof");
   return ended;



}

int Administration::parse(){
   NewLine();
   parsr->work(*this);
   return errorCount;
}
