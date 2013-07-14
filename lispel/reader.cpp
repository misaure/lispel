#include <cassert> //REMOVEME:
#include <lispel/defs.hh>
#include <lispel/reader.hh>
#include <lispel/lexer.hh>
#include <lispel/nodefactory.hh>
#include <lispel/utilities.hh>

static void printListHelper( const Handle *node, std::ostream &os)
{
  assert( node->hasType( Handle::ntCONS));

  if (!node->isNilRep()) {
    if (node->car()->hasType( Handle::ntCONS))
      printList( node->car(), os);
    else {
        os << *(node->car());
        if (!node->cdr()->isNilRep())
	  os << ' ';
    }
    printListHelper( node->cdr(), os);
  }
}

//FIXME: move to utilities
std::ostream &printList( const Handle *node, std::ostream &os)
{
  assert( 0 != node && 0 != &os);
  if (!node->hasType( Handle::ntCONS))
    throw TypeException( "pair", "printList", __LINE__);
  
  os << '(';
  printListHelper( node, os);
  return os << ')';
}

Reader::Reader()
{
}

Reader::~Reader()
{
}

Reader::ReadException::ReadException( std::string what, std::string where, int line)
  : RecoverableException( what, where, line)
{
}

Reader::ReadException::~ReadException() throw()
{
}

SimpleReader::SimpleReader( Context &ctx) : m_currentToken( Token::ttEOF, ""), m_ctx( ctx)
{
  assert( 0 != ctx.factory);
  m_nodeFactory = m_ctx.factory;
  m_indent = 1;
}

SimpleReader::~SimpleReader()
{
  MCAssertValidInstance();
}

Handle_ptr SimpleReader::read( std::istream *stream)
{
  MCAssertValidInstance();
  assert( 0 != stream);

  (*stream) >> m_currentToken;
  dbgPrint( "SimpleReader::read");
#if defined( DEBUG) && DEBUG > 2
  std::cerr << "SimpleReader::read: m_currentToken == "
       << tokennumToString( m_currentToken.tokennum()) << std::endl;
#endif


  if (!m_currentToken.hasType( Token::ttEOF)) {

    // read list
    if (m_currentToken.hasType( Token::ttLPAREN))
      return readList( stream);

    // read vector
    else if (m_currentToken.hasType( Token::ttVECSTART))
      return readVector( stream);

    // read quoted expressionm converting 'EXPR to (quote EXPR)
    else if (m_currentToken.hasType( Token::ttQUOTECHAR)) {
#if defined( DEBUG) && DEBUG > 3
      std::cerr << "SimpleReader::reader( " << stream << ")" << std::endl;
#endif
      std::list<Handle_ptr> quoteExpr;
      quoteExpr.push_back( m_nodeFactory->makeSymbol( "quote"));
      Handle_ptr nextNode = this->read( stream);
//        if (Token::ttEOF != m_currentToken.tokennum())
//  	throw Reader::ReadException( "end-of-file after quote",
//  				     __FILE__, __LINE__);
//        if (Token::ttERROR != m_currentToken.tokennum())
//  	throw Reader::ReadException( "invalid input", __FILE__, __LINE__);
      quoteExpr.push_back( nextNode);
      return makeList( quoteExpr.begin(), quoteExpr.end(), m_ctx);
    } else

      return readAtom( stream);

  } else 
     return 0;
}

Handle_ptr SimpleReader::readList( std::istream *stream)
{
  MCAssertValidInstance();
  assert( 0 != stream);

  dbgPrint( "SimpleReader::readList");  ++m_indent;

  // create storage for the list elements being read
  Handle_ptr exprRead;
  std::list<Handle_ptr> buffList;
  while (0 != (exprRead = read( stream)))
    buffList.push_back( exprRead);

  // read list elements
  if (m_currentToken.tokennum() == Token::ttRPAREN) {
     --m_indent;
     if (buffList.size() == 0)
       // read empty list
       return m_ctx.NIL;
     return makeList( buffList.begin(), buffList.end(), m_ctx);
  }

  // read returns (Handle_ptr)0 either when end-of-file occured or a right
  // parenthesis has been read.
  if (m_currentToken.tokennum() == Token::ttEOF)
     throw "SimpleReader::readList: end-of-file inside list";

  throw "SimpleReader::readList: internal error";
  MCAssertNotReached( 0);
}

Handle_ptr SimpleReader::readVector( std::istream *stream)
{
  MCAssertValidInstance();
  assert( 0 != stream);

  dbgPrint( "SimpleReader::readVector"); ++m_indent;

  // temporarily collect all expressions in a list
  std::vector<Handle_ptr> newVector;
  Handle_ptr exprRead;

  // numbersOnly is used to implement a little trick with vectors: when all
  // elements of a vector being read are numbers, the vector will be stored
  // as an immediate value (array of double values), thus allowing 
  // significantly faster vector operations.
  bool numbersOnly = true;
  int i;
  for (i = 0;  (exprRead = read( stream)); ++i) {
    assert( 0 != exprRead);
    if (!exprRead->isNumber())
       numbersOnly = false;
    newVector.push_back( exprRead);
  }
  --m_indent;

  return m_ctx.factory->makeVector( newVector);
}

Handle_ptr SimpleReader::readAtom( std::istream *stream)
{
  MCAssertValidInstance();
  assert( 0 != stream);
  dbgPrint( "SimpleReader::readAtom");

  switch( m_currentToken.tokennum()) {
  case Token::ttRPAREN:
    return 0;

  case Token::ttVECSTART:
    return readVector( stream);

  default:
    return m_nodeFactory->nodeFromToken( &m_currentToken);
  }

  MCAssertNotReached( 0);
}

void SimpleReader::dbgPrint( const char *methodname)
{
#if defined( DEBUG) && DEBUG > 3
  MCAssertValidInstance();
  assert( 0 != methodname);

  int i;
  for (i=0; i<m_indent; ++i)
    std::cerr << ' ';
  std::cerr << methodname << '\t' << m_currentToken << std::endl;
#endif /*DEBUG*/
}

