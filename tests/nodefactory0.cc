/*
 * nodefactory0.cc ---
 *
 *  Checks NodeFactory's make* and display functions and stlvect2List and 
 *  printList from reader (nodefactory1 uses stllist2List instead of
 *  stlvect2List).
 *
 * RCSID: $Id: nodefactory0.cc,v 1.1.1.1 2001/10/31 15:44:43 michael Exp $
 */

#include <cassert>
#include <lispel/lispel.hh>

using namespace std;

typedef vector<Handle_ptr> HandleVector;

#define assert_type( TYPE) assert( newNode->hasType( TYPE))

int main( void)
{
  Interpreter interp;
  NodeFactory &factory = *(interp.context().factory);
  HandleVector handles;

  try {
    /*
     * Test if NIL is generated correctly.
     */
    cout << "NIL: ";
    factory.display( interp.context().NIL, &cout);
    cout << endl;
    
    /*
     * Create some nodes and display them one by one.
     */
    cout << "*** creating single nodes:" << endl;
    cout << "free memory: " << factory.freeCount() << endl;
    Handle_ptr newNode;
    
    handles.push_back( newNode=factory.makeCharacter( 'a'));
    assert_type( Handle::ntCHARACTER);
    
    handles.push_back( newNode=factory.makeBoolean( true));
    assert_type( Handle::ntBOOLEAN);

    handles.push_back( newNode=factory.makeBoolean( false));
    assert_type( Handle::ntBOOLEAN);

    handles.push_back( newNode=factory.makeInteger( 1001));
    assert_type( Handle::ntINTEGER);

    handles.push_back( newNode=factory.makeDouble( 1001.0));
    assert_type( Handle::ntDOUBLE);

    handles.push_back( newNode=factory.makeString( "a string"));
    assert_type( Handle::ntSTRING);

    handles.push_back( newNode=factory.makeSymbol( "string->number"));
    assert_type( Handle::ntSYMBOL);

    HandleVector::iterator pos;
    for (pos = handles.begin(); pos != handles.end(); ++pos) {
      factory.display( (*pos), &cout);
      cout << endl;
    }
    cout << "free memory: " << factory.freeCount() << endl;

    /*
     * Create a consed list from the nodes created before and print it.
     */
    cout << "*** trying to create list from nodes created before:" << endl;
    //Handle_ptr firstNode = stlvect2List( factory, handles);
    Handle_ptr firstNode = makeList( handles.begin(), handles.end(), 
				     interp.context());
    cout << "  first node == " << *firstNode << endl;
    printList( firstNode, cout);
    cout << endl;
  }
  catch( Exception lex) {
    cerr << "\nEXCEPTION: " << lex << endl;
  }
  catch( ...) {
    cerr << "\nUNCAUGHT EXCEPTION" << endl;
  }

  return 0;
}
