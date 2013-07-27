#include <cstdlib>
#include <fstream>
#include <lispel/lexer.hh>

using namespace std;

int main( void)
{
  initLexerModule();

  for (;;) {
    if (!cin)
      break;
    Token t;
    cin >> t;
    cout << t << endl;
    if (t.tokennum() == Token::ttERROR || t.tokennum() == Token::ttEOF)
      break;
  }

  return 0;
}

