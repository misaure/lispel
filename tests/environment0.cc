#include <cstdio> //sprintf
#include <lispel/lispel.hh>

using namespace std;

int main( int argc, char *argv[])
{
  Interpreter interp;
  Environment *toplevel, *current;
  NodeFactory &factory = *(interp.context().factory);

  toplevel = current = new Environment();

  int i;

  for (i=0; i<argc; ++i)
    toplevel->put( string( argv[i]), factory.makeString( argv[i]));

  for (i=0; i<100; ++i) {
     char buffer[100];
     sprintf( buffer, "%d", i);
     string s( buffer);
     current->put( s, factory.makeString( buffer));
     if (0 == (i % 10))
       current = current->makeChildEnvironment();
  }

  for (i=100; i>=0; --i) {
    char buffer[100];
    sprintf( buffer, "%d", i);
    string s( buffer);
    Handle_ptr res = current->lookup( s);
    if (0 != res)
      cout << s << " == " << *res << endl;
    else
      cout << s << " == (null)" << endl;
  }

  return 0;
}

