#include <strstream>
#include <lispel/utilities.hh>
#include <lispel/lexer.hh>
#include <lispel/defs.hh>

const char *typetagToString( Handle::NodeType t)
{
  switch( t) {
  case Handle::ntEMPTY:		return "empty";
  case Handle::ntBOOLEAN:	return "boolean";
  case Handle::ntCHARACTER:	return "character";
  case Handle::ntINTEGER:	return "integer";
  case Handle::ntDOUBLE:	return "double";
  case Handle::ntSTRING:	return "string";
  case Handle::ntSYMBOL:	return "symbol";
  case Handle::ntARRAY:
  case Handle::ntAVECTOR:	return "vector";
  case Handle::ntCONS:		return "pair";
  case Handle::ntCFUNC:		return "builtin";
  case Handle::ntCLOSURE:	return "closure";
  default:
    return "unknown type";
  }
}

int listlength( const Handle *n)
{
   int i;
   for (i=0; !n->isNilRep(); ++i)
      n = n->cdr();
   return i;
}

bool eqv( const Handle *n1, const Handle *n2)
{
  if (eq( n1, n2))
     return true;
  if (n1->type() != n2->type())
     return false;
  if (n1->hasType( Handle::ntINTEGER))
     return n1->integerValue() == n2->integerValue();
  if (n1->hasType( Handle::ntDOUBLE))
     return n1->doubleValue() == n2->doubleValue();
  if (n1->hasType( Handle::ntCHARACTER))
     return n1->integerValue() == n2->integerValue();

  return false;
}

bool
isCompleteExpression( std::string &expr)
{
   std::istrstream is( expr.c_str(), expr.size());
   int nestLevel = 0;
   while (!is.eof()) {
      Token tok;
      if (Token::ttVECSTART == tok.tokennum() ||
          Token::ttLPAREN == tok.tokennum())
	 ++nestLevel;
      else if (Token::ttRPAREN == tok.tokennum())
         --nestLevel;
      else if (Token::ttEOF == tok.tokennum())
         break;
   }
   return (nestLevel==0);
}

