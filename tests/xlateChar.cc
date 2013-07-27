#include <iostream>
#include <cstdlib> //exit()
#include <lispel/lexer.hh>

using namespace std;

int main( int argc, char *argv[])
{
   if (1 == argc) {
      cout << "usage: " << argv[0] << " {<charnames>}+" << endl;
      cout << " where charname might be a Scheme character identifier"
           << endl;
      exit( EXIT_FAILURE);
   }

   initLexerModule();
   int i;
   for (i=1; i<argc; ++i)
     cout << argv[i] << " = " << translateCharacter( argv[i]) << endl;

   exit( EXIT_SUCCESS);
}

