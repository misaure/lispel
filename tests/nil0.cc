#include <lispel/lispel.hh>

using namespace std;

int main( void)
{
   Interpreter interp;
   const Context &ctx = interp.context();

   if (0 != ctx.NIL)
     cout << "NIL != (Handle_ptr)0" << endl;
   else 
     cout << "ERROR: NIL == (Handle_ptr)0" << endl;

   if (ctx.NIL->hasType( Handle::ntCONS))
     cout << "type( NIL) == CONS" << endl;
   else
     cout << "ERROR: type( NIL) != CONS" << endl;
   
   if (ctx.NIL->isNilRep())
     cout << "isNil( NIL) == true" << endl;
   else
     cout << "ERROR: isNil( NIL) == false" << endl;
   
   return 0;
}

