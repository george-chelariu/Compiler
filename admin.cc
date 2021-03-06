#include "admin.h"




Administration::Administration(ifstream& in, ofstream &out, Scanner &sc, Parser &pc, ofstream &outasm){
   outputasm = &outasm;
   emitting = true;
   outputfileptr = &out;
   scanr= &sc;
   lineNo = 0;
   lineNumber = 1;
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

void Administration::TypeError(TableEntry info){
   if (errorCount < MAXERRORS){
      cout << "line: " << lineNo << " " << "Kind: " << info.kind << '\t'
	   << "type: " << info.type << '\t' << "Value: " << info.value << endl; 
      errorCount++;
   }
   return;


}

void Administration::emit1(string op)
{
   //remains true until we find an error
   if(emitting){
      //output just the op
	 *outputasm << op << endl;
   
   
   }
     
   lineNumber++;
   
}

void Administration::emit2(string op, int arg1)
{
   //remains true until we find an error 
   if(emitting){
      //output the op and argument
	 *outputasm << op << endl;
      *outputasm  << arg1 << endl;
   }
   lineNumber++;
   lineNumber++;
   
}

void Administration::emit3(string op, int arg1, int arg2)
{
   //remains true until we find an error
   if(emitting){
      //output the op and the 2 arguments
      

	 *outputasm << op << endl << arg1 << endl << arg2 << endl;
   }
   lineNumber++;
   lineNumber++;
   lineNumber++;
}

int Administration::getlineNumber()
{
   return lineNumber;
}


void Administration::error (string text){
   if (errorCount < MAXERRORS){
      cout << "line: " << lineNo << " " << '\"' << text <<  '\"' << endl;
      correctline = false;
      errorCount++;
   }
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


void Administration::fatal(string words){
   cout << words << endl;
   errorCount =100;
   emitting == false;
   return;

}

