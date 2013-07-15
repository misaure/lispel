#include <lispel/vminterp.hh>
#include <lispel/exception.hh>

std::ostream &operator<<( std::ostream &os, const VmCommand &vc)
{
  VmCommand::const_iterator pos;
  for (pos=vc.begin(); pos!=vc.end(); ++pos)
    os << (*pos).lexval() << ' ';
  return os;
}


VmCommandReader::VmCommandReader( std::istream *stream)
{
  m_stream = stream;
  (*m_stream) >> m_currentToken;
  m_atEnd = (m_currentToken.hasType( Token::ttEOF))? true : false;
}

VmCommandReader::~VmCommandReader()
{
}

VmCommand VmCommandReader::readCommand()
{
  VmCommand cmd;
  if (!m_currentToken.hasType( Token::ttLPAREN)) {
    if (m_currentToken.hasType( Token::ttEOF)) {
      m_atEnd = true;
      return cmd;
    }
    std::cerr << "expected '('" << std::endl;//FIXME: exception
  } else {

    for (;;) {
      (*m_stream) >> m_currentToken;
      if (m_currentToken.hasType( Token::ttRPAREN)) {
	(*m_stream) >> m_currentToken;
	return cmd;
      }
      if (m_currentToken.hasType( Token::ttEOF))
	//FIXME: throw specialized exception
	throw Exception( "end-of-file inside command", __FILE__, __LINE__);
      switch( m_currentToken.tokennum()) {
      case Token::ttID:
      case Token::ttKEYWORD:
      case Token::ttINT:
      case Token::ttDOUBLE:
      case Token::ttSTRING:
      case Token::ttOPERATOR:
      case Token::ttFALSE:
      case Token::ttTRUE:
      case Token::ttCHARACTER:
	cmd.push_back( m_currentToken);
	std::cerr << '\'' << m_currentToken.lexval() << '\'' << " - " 
	     << tokennumToString( m_currentToken.tokennum()) 
	     << std::endl;
	break;
	
      default:
	std::cerr << "unexpected token type " 
	     << tokennumToString( m_currentToken.tokennum())
	     << std::endl;
      }
    }
  }
  if (m_currentToken.hasType( Token::ttEOF))
    m_atEnd = true;

  return cmd;
}

