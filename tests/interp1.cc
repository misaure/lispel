/*
 * interp1.cc ---
 *
 *   Check if toplevel bindings work and manually make a call equivalent to
 *   (display "hello, world!").
 *
 * RCSID: $Id: interp1.cc,v 1.1.1.1 2001/10/31 15:44:43 michael Exp $
 */

#include <iostream>
#include <lispel/lispel.hh>

using namespace std;

char *builtins[] = {
  "lambda", "display", "car", "cdr", 
  0
};

int main( void)
{
  Interpreter interp;

  try {
    addCoreCommands( interp);
    addListCommands( interp);
    addMathCommands( interp);
    
    int i;
    for (i=0; builtins[i] != 0; ++i) {
      Handle_ptr n = interp.toplevelLookup( builtins[i]);
      if (0 == n)
	cout << builtins[i] << "UNDEFINED" << endl;
      else
	cout << builtins[i] << " == " << (*n) << endl;
    }
    
    /*
     * Create an s-expression manually and try to execute it
     */
    NodeFactory *factory = interp.nodeFactory();
    Handle_ptr exprCons = factory->makeCons( factory->makeSymbol( "display"));
    Handle_ptr argCons = factory->makeCons( factory->makeString( "hello, world"));
    exprCons->setCdr( argCons);
    printList( exprCons, cout);

    // So far, the following structure has been built:
    //   +---------+          +-----------------+
    //   | display |          | "hello, world!" |
    //   +---------+          +-----------------+
    //      ^                    ^
    //   +--|--+---+          +--|--+---+
    //   |  *  | *----------> |  *  | *----------> NIL 
    //   +-----+---+          +-----+---+
    interp.evaluator()->eval( exprCons);
  }

  catch( Exception e) {
    cerr << "EXCEPTION: " << e << endl;
  }
  catch( const char *s) {
    cerr << "EXCEPTION: " << s << endl;
  }
  catch( ...) {
    cerr << "unknown exception" << endl;
  }

  return 0;
}
