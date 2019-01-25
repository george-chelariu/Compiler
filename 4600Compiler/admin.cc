Administration(ifstream& in, ofstream &out){
   outputfileptr = out;
   scanr= Scanner(in , &sybols);
   lineNo = 0;
   correctline = true;
   errorCount = 0;
}

void error (string text){

   cout << "line: " << lineNo << " " << '\"' << text <<  '\"' << endl;
   correctline = false;
   return;




}

int scan (){
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
