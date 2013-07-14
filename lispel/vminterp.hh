#ifndef _lispel_VMINTERP_HH_
#define _lispel_VMINTERP_HH_ 1

#include <vector>
#include <iostream>
#include <lispel/lexer.hh>

typedef std::vector<Token> VmCommand;
std::ostream &operator<<( std::ostream &os, const VmCommand &vc);

/**
   A class for reading commands understood by the textual VM code interpreter.
   The commands are given in a Scheme-like syntax, but in contrast to Scheme
   commands can't be nested.
   @author <a href="msaure@rz.uni-osnabrueck.de">Michael Saure</a>
   @version 0.1
*/
class VmCommandReader {
public:
  VmCommandReader( std::istream *stream);
  ~VmCommandReader();

  VmCommand readCommand();
  bool done() { return m_atEnd; }

protected:
  std::istream *m_stream;
  Token m_currentToken;
  bool m_atEnd;
};

#endif /*_lispel_VMINTERP_HH_*/

