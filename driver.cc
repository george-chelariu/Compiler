#include <iostream>
#include <fstream>

#include "admin.h"
#include "scanner.h"
#include "symtable.h"
#include "token.h"
#include "symbol.h"
#include "parser.h"
#include "blocktable.h"
#include "interp.h"     


// The original grammar for infix2postfix translator
//
// Stmt --> list eof
//
// list --> expr ; list | empty
//
// expr --> expr + term | expr - term | term
//
// term --> term * factor | term / factor | term div factor | term mod factor | factor
//
// factor --> (expr) | id | num

using namespace std;

bool file_exists(const char* filename);




int main(int argc, char *argv[])
{

string program_filename; //PLAM instruction file
bool stepping = false;   //for debugging purposes; use switch -s
   
   
   
if (argc != 4){
cout << "Usage: " << argv [0] << " PL_file Output_file PLAM_holder" << endl;
return 0;
}
// Open the input and output files.
ifstream inputfile (argv[1]);
if(!inputfile){
cerr << "PL_file " << argv[1] << " could not be opened" << endl;
return 1;
}
ofstream outputfile(argv[2]);
if(!outputfile)
{
cerr << "Output_file" << argv[2] << " could not be opened" << endl;
return 1;
}
ofstream PLAMfile(argv[3]);
if ( !file_exists( argv[3]))
{  
cout << "Program file '" << argv[3] << "' does not exist \n" << endl;
return 1;
}
else
   program_filename = argv[3];


    // create a symbol table


    Symtable st;


    //create a scanner
    Scanner sc(&inputfile, &st);
    BlockTable bt(0);
    //create a parcer
    Parser pc(&bt);
    
    //Get the compiler running.
Administration compiler(inputfile, outputfile, sc, pc, PLAMfile);
    int status = compiler.parse();
    if (status ==0)
       cout << "Scanning and parsing successful" << endl;
    else{
       cerr << "Program contains error(s)" << endl;
       return 0;
    }
    cout << "starting interpretation" << endl;
    

    Interpreter interpreter(program_filename, stepping);

    
    return 0;
}

bool file_exists( const char* file_name)
{
  filebuf fb;
  if ( fb.open ( file_name, ios::in))
  {
    fb.close();
    return true;
  }
  else
    return false;
}
