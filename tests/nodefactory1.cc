/*
 * nodefactory1.cc ---
 *
 *  Checks NodeFactory's make* and display functions and stllist2List and 
 *  printList from reader (nodefactory0 uses stlvec2List instead of
 *  stllist2List).
 *
 * RCSID: $Id: nodefactory1.cc,v 1.1.1.1 2001/10/31 15:44:43 michael Exp $
 */

#include <lispel/lispel.hh> //printList, stllist2List

using namespace std;

typedef list<Handle_ptr> HandleList;


int main( void)
{
  Interpreter interp;
  NodeFactory &factory = *(interp.context().factory);
  HandleList handles;

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
  handles.push_back( factory.makeCharacter( 'a'));
  handles.push_back( factory.makeBoolean( true));
  handles.push_back( factory.makeBoolean( false));
  handles.push_back( factory.makeInteger( 1001));
  handles.push_back( factory.makeDouble( 1001.0));
  handles.push_back( factory.makeString( "a string"));
  handles.push_back( factory.makeSymbol( "string->number"));

  HandleList::iterator pos;
  for (pos = handles.begin(); pos != handles.end(); ++pos) {
    factory.display( (*pos), &cout);
    cout << endl;
  }
  cout << "free memory: " << factory.freeCount() << endl;

  /*
   * Create a consed list from the nodes created before and print it.
   */
  cout << "*** trying to create list from nodes created before:" << endl;
  Handle_ptr firstNode = makeList( handles.begin(), handles.end(), interp.context());
  cout << "  first node == " << *firstNode << endl;
  printList( firstNode, cout);
  cout << endl;

  return 0;
}
