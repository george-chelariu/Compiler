#include "token.h"

string Token::spellS(Symbol S){

   switch(S){
        case 256 :
           return "ID";
        case 257 :
            return "NUM";
        case 258 :
            return "BADNUM";
        case 259 :
            return "BADNAME";
        case 260 :
            return "NEWLINE";
        case 261 :
            return "NONAME";
        case 262 :
            return "ENDOFFILE";
        case 263 :
            return "MODULUS";
        case 264 :
            return "SEMICOLON";
        case 265 :
            return "PLUS";
        case 266 :
            return "MINUS";
        case 267 :
            return "TIMES";
        case 268 :
            return "DIVIDE";
        case 269 :
            return "LEFTP";
        case 270 :
            return "RIGHTP";
        case 271 :
            return "BADCHAR";
        case 272 :
            return "DOT";
        case 273 :
            return "COMMA";
        case 274 :
            return "LEFTB";
        case 275 :
            return "RIGHTB";
        case 276 :
            return "COLON";
        case 277 :
            return "ARROW";
        case 278 :
            return "DOUBLEB";
        case 279 :
            return "OR";
        case 280 :
            return "AND";
        case 281 :
            return "EQUALS";
        case 282 :
            return "LESSTHAN";
        case 283 :
            return "GREATERTHAN";

   }
   return "error: no symbol name";
}

