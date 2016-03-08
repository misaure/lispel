/*
 * interp0.cc ---
 *
 *   Demonstrates the use of the Interpreter class to implement a small
 *   standalone interpreter.
 *
 * RCSID: $Id: interp0.cc,v 1.1.1.1 2001/10/31 15:44:43 michael Exp $
 */

#include <fstream>
#include <lispel/lispel.hh>

using namespace std;

int main( int argc, char *argv[])
{
   Interpreter interp;

   try {
     addCoreCommands( interp);
     addListCommands( interp);
     addMathCommands( interp);

     if (1 == argc)
       interp.repl( &cin, new SimpleShellInterface());
     else {
       ifstream *strm = new ifstream( argv[1]);
       interp.repl( strm, new SimpleShellInterface());
       // delete strm;
     }
   }

   catch( const Exception &e) {
     // subclasses of lispel_exceptions signal error situations that occured
     // during the execution of a Lispel program.
     cerr << "\nexception caught:" << endl;
     cerr << e << endl;
   }
   catch( const char *exc) {
     // strings containing error messages have been used during development
     cerr << "\nexception caught:" << endl;
     cerr << exc << endl;
   }
   catch( ...) {
     // exceptions thrown by the C++ library
     cerr << "\nunknown exception!" << endl;
   }

   return 0;
}

