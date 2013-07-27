#include <lispel/nodes.hh>

using namespace std;

int main( void)
{
  NodeValue *val = new StringValue( "a little test");

  cout << "val == " << val
       << ", eval() == " << val->eval() << endl;
  if (val != val->eval())
    cout << "FAILURE" << endl;
  else
    cout << "OK" << endl;

  return 0;
}
