#include <iostream>
#include <fstream>
#include <cstdlib> //exit()
#include <lispel/lispel.hh>

using namespace std;

int main( int argc, char *argv[])
{
  Interpreter interp;
  NodeFactory &nodeFactory = *(interp.context().factory);

  initLexerModule();

  istream *infile = &cin;
  if (argc == 2)
     infile = new ifstream( argv[1]);

  try {
    cerr << "free memory: " << nodeFactory.freeCount() << endl;
    SimpleReader reader( interp.context());
    Handle_ptr retval;
    while (0 != (retval = reader.read( infile))) {
      cout << "> read returned " << retval << endl;
      if (retval->hasType( Handle::ntCONS))
        printList( retval, cout);
      //nodeFactory.display( retval, &cout);
    }
    cerr << "free memory: " << nodeFactory.freeCount() << endl;
  }
  catch( Exception lex) {
    cerr << "\nEXCEPTION: " << lex << endl;
  }

  catch( const char *msg) {
    cerr << "\nEXCEPTION: " << msg << endl;
  }
  catch( ...) {
    cerr << "\nEXCEPTION CAUGHT!" << endl;
  }

  //if (infile != &cin)
  //   delete infile;

  return 0;
}

