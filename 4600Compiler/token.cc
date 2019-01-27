#include "token.h"

string Token::spellS(Symbol S){

   switch(S){
      case 256 :
	 return "ID";
	 break;
      case 257 :
	 return "NUM";
      case 258 :
	 return "BADNUM";
	 break;
      case 259 :
	 return "BADNAME";
      case 260 :
	 return "NEWLINE";
      case 261 :
	 return "NONAME";
      case 262 :
	 return "ENDOFFILE";
      case 263 :
	 return "DIV";
      case 264 :
	 return "MOD";
      case 265 :
	 return "SEMICOLON";
      case 266 :
	 return "PLUS";
      case 267 :
	 return "MINUS";
      case 268 :
	 return "TIMES";
      case 269 :
	 return "DEVIDE";
      case 270 :
	 return "LEFTP";
      case 271 :
	 return "RIGHTP";
      case 272 :
	 return "BADCHAR";

				    
   }
   return "error no symbol name";
}
	    
