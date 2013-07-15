#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <climits>
#include <cassert>
#include <lispel/lexer.hh>
#include <lispel/exception.hh>
#include <lispel/defs.hh>


typedef unsigned long chartype;

static chartype CHARTYPE_ILLEGAL = 0L;
static chartype CHARTYPE_IDBEGIN = (1L << 2);
static chartype CHARTYPE_ID      = (1L << 3);
static chartype CHARTYPE_DECIMAL = (1L << 4);
static chartype CHARTYPE_LSTART  = (1L << 5);
static chartype CHARTYPE_LEND    = (1L << 6);
static chartype CHARTYPE_QUOTE   = (1L << 7);
static chartype CHARTYPE_WS      = (1L << 8);
static chartype CHARTYPE_COMMENT = (1L << 9);
static chartype CHARTYPE_NONID   = (1L << 10);
static chartype CHARTYPE_STRQUOT = (1L << 11);
static chartype CHARTYPE_OPCHAR  = (1L << 12);
static chartype CHARTYPE_RELOP   = (1L << 13);

static chartype characterType[256];

static const char *NON_ID_CHARACTERS = "()'\";";
static const char *OPERATOR_CHARACTERS = "+-*/";
static const char *RELOP_CHARACTERS = "<>=";

inline static bool is_chartype( int c, chartype t)
{
  return (characterType[c] & t);
}

static struct {
   const char *name;
   char character;
} defaultCharacterNames[] = {
  { "space",	' ' },
  { "newline",	'\n' },
  { "nl",	'\n' },
  { "tab",	'\t' },
  { "ht",	'\t' },
  { "alarm",	'\a' },
  { "return",	'\r' },
  { "cr",	'\r' },
  { "backspace",'\b' },
  { "bs",	'\b' },
  { "backslash",'\\' },
  { "vtab",	'\v' },
  { "vt",	'\v' },
  { "escape",	'\033' },
  { "esc",	'\033' },
  { 0,          0 },
};

static std::map<std::string,char> charnameMap; 

void initLexerModule()
{
  int i;
  for (i=0; i<256; ++i) {
    characterType[i] = CHARTYPE_ILLEGAL;
    // alphabetic characters
    if (isalpha( i)) {
      characterType[i] |= CHARTYPE_IDBEGIN;
      characterType[i] |= CHARTYPE_ID;
      // whitespace
    } else if (isspace( i))
      characterType[i] |= CHARTYPE_WS;
  }

  // digits
  for (i='0'; i<='9'; ++i) {
    characterType[i] |= CHARTYPE_ID;
    characterType[i] |= CHARTYPE_DECIMAL;
  }

  // characters which mark the end of an identifier
  for (i=0; NON_ID_CHARACTERS[i] != 0; ++i)
    characterType[NON_ID_CHARACTERS[i]] |= CHARTYPE_NONID;

  // single character operators (shortcuts to arithmetic functions)
  for (i=0; OPERATOR_CHARACTERS[i] != 0; ++i) {
    characterType[OPERATOR_CHARACTERS[i]] |= CHARTYPE_ID;
    characterType[OPERATOR_CHARACTERS[i]] |= CHARTYPE_OPCHAR;
  }

  // character making up relational operators
  for (i=0; RELOP_CHARACTERS[i] != 0; ++i) {
    characterType[RELOP_CHARACTERS[i]] |= CHARTYPE_ID;
    characterType[RELOP_CHARACTERS[i]] |= CHARTYPE_RELOP;
  }

  characterType['('] |= CHARTYPE_LSTART;
  characterType[')'] |= CHARTYPE_LEND;
  characterType['\''] |= CHARTYPE_QUOTE;
  characterType[';'] |= CHARTYPE_COMMENT;
  characterType['"'] |= CHARTYPE_STRQUOT;

  // build a map for faster lookup of character constants
  for (i=0; 0 != defaultCharacterNames[i].name; ++i) {
    charnameMap[defaultCharacterNames[i].name] = defaultCharacterNames[i].character;
  }
}

int translateCharacter( const std::string &charname)
{
  if (1 == charname.size())
    return static_cast<int>( charname[0]);
  else if (0 == charname.size())
    return -1;
  else {
    std::map<std::string,char>::iterator pos = charnameMap.find( charname);
    if (charnameMap.end() != pos)
      return static_cast<int>( (*pos).second);
  }
  return -1;
}

void printChartypes()
{
  int i;
  for (i=0; i<256; ++i) {
    if (characterType[i] == CHARTYPE_ILLEGAL)
      continue;

    std::cout << i << ":\t";
    if (characterType[i] & CHARTYPE_IDBEGIN)
      // whitespace
      std::cout << "idbegin ";
    if (characterType[i] & CHARTYPE_ID)
      std::cout << "id ";
    if (characterType[i] & CHARTYPE_DECIMAL)
      std::cout << "decimal ";
    if (characterType[i] & CHARTYPE_LSTART)
      std::cout << "liststart ";
    if (characterType[i] & CHARTYPE_LEND)
      std::cout << "listend ";
    if (characterType[i] & CHARTYPE_QUOTE)
      std::cout << "quote ";
    if (characterType[i] & CHARTYPE_WS)
      std::cout << "whitespace ";
    if (characterType[i] & CHARTYPE_COMMENT)
      std::cout << "comment-char ";
    if (characterType[i] & CHARTYPE_NONID)
      std::cout << "non-id ";
    if (characterType[i] & CHARTYPE_OPCHAR)
      std::cout << "operator ";
    if (characterType[i] & CHARTYPE_RELOP)
      std::cout << "relop ";
    if (std::isprint( i))
      std::cout << '[' << static_cast<char>( i) << ']';
    std::cout << std::endl;
  }
}


LexBuffer::size_type Token::M_LEXBUFFERSIZE = 1024;

StringBucket::StringBucket()
{
}

StringBucket::~StringBucket()
{
}

const char *StringBucket::internString( const char *s)
{
  bucket_iterator pos;
  if (m_bucket.end() == (pos = m_bucket.find( s))) {
    m_bucket[strdup( s)] = 1;     //... and insert it with recount == 1
    return (*pos).first;
  } else {
    (*pos).second += 1;
    return (*pos).first;
  }
}

void StringBucket::releaseString( const char *s)
{
  bucket_iterator pos;
  if (m_bucket.end() != (pos = m_bucket.find( s))) {
    (*pos).second -= 1;
    if (0 == (*pos).second) // delete string if refcount == 0
      m_bucket.erase( pos);
  }
}

Token::Token() : m_tokennum( ttERROR), m_lexval( "")
{
}

Token::Token( Token::TokenType tokennum, const std::string &lexval)
  : m_tokennum( tokennum), m_lexval( lexval)
{
}

Token::~Token()
{
  MCAssertValidInstance();
}

std::string Token::convertSymbol( std::string s)
{
   return s;
}

std::ostream &operator<<( std::ostream &os, const Token &t)
{
  MCAssert( 0 != &t, "output operator called for null Token inst");
  return os << tokennumToString( t.m_tokennum) 
	    << " \"" << t.m_lexval << '"';
}

inline std::istream &skiptoeol( std::istream &strm)
{
  MCAssert( 0 != &strm, "skiptoeol applied to null input stream");
  strm.ignore( INT_MAX, '\n');
  return strm;
}

typedef enum {
  lsINITIAL, lsSKIPWS, lsSPECIAL, lsID, lsNUMBER, lsSTRING, lsFLOAT,
  lsKEYWORD
} LexerState;

std::istream &operator>>( std::istream &is, Token &t)
{
  assert( 0 != &t);
  LexBuffer buffer( Token::M_LEXBUFFERSIZE);

  LexerState state = lsINITIAL;
  for (;;) {
    //FIXME: many unget() can be saved when this is done only once outside
    // the loop, but I don't want to break the scanner again, right now...
    int c = is.peek();

    switch( state) {
    case lsINITIAL:
      if (is.eof() || -1 == c) {
	t.m_tokennum = Token::ttEOF;
	t.m_lexval = "";
	return is;

      } else if (is_chartype( c, CHARTYPE_COMMENT)) {
      	is >> skiptoeol;
	break;

      } else if (isspace( c)) {
	state = lsSKIPWS;
	break;

      } else if ('(' == c) {
	is.get();
	t.m_tokennum = Token::ttLPAREN;
	t.m_lexval = "(";
	return is;

      } else if (')' == c) {
	is.get();
	t.m_tokennum = Token::ttRPAREN;
	t.m_lexval = ")";
	return is;

      } else if ('#' == c) {
	is.get();			// consume leading '#'
	state = lsSPECIAL;
	break;

      } else if (isalpha( c)) {
	state = lsID;
	break;

      } else if (':' == c) {
	is.get();			// consume leading ':'
        state = lsKEYWORD;
	break;

      } else if (isdigit( c) || '-' == c || '+' == c) {
	state = lsNUMBER;
	break;

      } else if ('\'' == c) {
	c = is.get();
	t.m_tokennum = Token::ttQUOTECHAR;
	t.m_lexval = "'";
	return is;

      } else if ('"' == c) {
	c = is.get();			// consume leading '"'
	state = lsSTRING;		// read string
	break;

      } else if (is_chartype( c, CHARTYPE_OPCHAR)) {
	t.m_tokennum = Token::ttOPERATOR;
	t.m_lexval = buffer.toString();
	return is;

      } else if (is_chartype( c, CHARTYPE_RELOP)) {
	t.m_tokennum = Token::ttID;
	t.m_lexval = buffer.toString();

      } else {
	t.m_tokennum = Token::ttERROR;
	t.m_lexval = _( "illegal character");
#if defined( DEBUG) && DEBUG > 2
	std::cout << "character number == " << c << std::endl;
#endif
      }
      return is;

    case lsSKIPWS:
      while (!is.eof() && isspace( c))
         c = is.get();
      if (is.eof()) {
	t.m_tokennum = Token::ttEOF;
	return is;
      }
      is.unget(), buffer.reset();
      state = lsINITIAL;
      break;

    case lsSPECIAL: // read special introduced by '#'
      if (is.eof()) {
	t.m_tokennum = Token::ttERROR;
	t.m_lexval = _( "end-of-file after '#'");
	return is;
      }
      if ('(' == c) {
	t.m_tokennum = Token::ttVECSTART;
	t.m_lexval = buffer.toString();

      } else if ('f' == c) {
	t.m_tokennum = Token::ttFALSE;
	t.m_lexval = buffer.toString();

      } else if ('t' == c) {
	t.m_tokennum = Token::ttTRUE;
	t.m_lexval = buffer.toString();

      } else if ('!' == c) {
	is >> skiptoeol;
	state = lsINITIAL;
	break;

      } else if ('\\' == c) {
        c = is.get();
	for (;;) {
	   c = is.get();
	   if (isspace( c) || is_chartype( c, CHARTYPE_NONID))
	      break;
	   buffer.append( c);
	}
	is.unget();
	t.m_tokennum = Token::ttCHARACTER;
	t.m_lexval = buffer.toString();
	return is;

      } else {
	t.m_tokennum = Token::ttERROR;
	t.m_lexval = buffer.toString();
      }
      return is;
      
    case lsID:
      for (;;) {
        if (is_chartype( c, CHARTYPE_NONID) || isspace( c) || is.eof()) {
	  is.unget(); --buffer;
	  t.m_tokennum = Token::ttID;
	  t.m_lexval = buffer.toString();
	  return is;
	}
	buffer.append( c=is.get());
      }
      break;

    case lsNUMBER:
      is.get();
      if ('+' == c || '-' == c) {
	buffer.append( c);
	c = is.get();
      }
      if (!isdigit( c)) {
	t.m_tokennum = Token::ttOPERATOR;
	t.m_lexval = buffer.toString();
	return is;
      }
      while (isdigit( c)) {
	buffer.append( c);
	c = is.get();
      }
      if ('.' == c) {
	state = lsFLOAT;
	break;
      }
      is.unget();
      t.m_tokennum = Token::ttINT;
      t.m_lexval = buffer.toString();
      return is;

    case lsSTRING:
      c = is.get();				//skip leading quote character
      while ('"' != c) {
        if (is.eof()) {			// signal error on EOF
	  t.m_tokennum = Token::ttERROR;
	  t.m_lexval = _( "end-of-file inside string");
	  return is;
        }
	if ('\\' == c) {		// save characters, performing
	   c = is.get();		// backslash substitution
	   switch( c) {
	   case 'n': c = '\n'; break;
	   case 'r': c = '\r'; break;
	   case 'b': c = '\b'; break;
	   case 't': c = '\t'; break;
	   case 'a': c = '\a'; break;
	   case 'v': c = '\v'; break;
	   case 'f': c = '\f'; break;
	   case '"': c = '"'; break;
	   }
	}
	buffer.append( c);
	c = is.get();
      }
      t.m_tokennum = Token::ttSTRING;
      t.m_lexval = buffer.toString();
      return is;

    case lsFLOAT:
      if (is.eof() || !isdigit( c)) {
	is.unget(); buffer.unget();
	t.m_tokennum = Token::ttDOUBLE;
	t.m_lexval = buffer.toString();
	return is;
      }
      break;

    case lsKEYWORD:
      c = is.get();
      while (std::isalnum( c)) {
        buffer.append( c);
	c = is.get();
      }
      is.unget();
      t.m_tokennum = Token::ttKEYWORD;
      t.m_lexval = buffer.toString();
      return is;

    default:
      t.m_tokennum = Token::ttERROR;
      t.m_lexval = _( "illegal scanner state");
      return is;
    }
  }

  MCAssertNotReached( is);
}


const char *tokennumToString( Token::TokenType tokennum)
{
  switch( tokennum) {
  case Token::ttCHARACTER: return "character";
  case Token::ttDOUBLE: return "double";
  case Token::ttEOF: return "EOF";
  case Token::ttERROR: return "ERROR";
  case Token::ttFALSE: return "false";
  case Token::ttID: return "symbol";
  case Token::ttINT: return "integer";
  case Token::ttKEYWORD: return "keyword";
  case Token::ttLPAREN: return "lparen";
  case Token::ttOPERATOR: return "operator";
  case Token::ttQUOTECHAR: return "quotechar";
  case Token::ttRPAREN: return "rparen";
  case Token::ttSTRING: return "string";
  case Token::ttTRUE: return "true";
  case Token::ttVECSTART: return "vecstart";
  default:
    return "???";
  }
}


LexBuffer::LexBuffer( LexBuffer::size_type s)
{
  m_pos = 0;
  m_buffer = new char[m_maxsize=s];
}

LexBuffer::~LexBuffer()
{
  MCAssertValidInstance();
  delete [] m_buffer;
}

void
LexBuffer::append( char c)
{
  MCAssertValidInstance();
  //if (m_pos<m_maxsize-1) {
  assert( 0 != m_buffer);
  assert( static_cast<LexBuffer::size_type>( m_pos) < m_maxsize-1);
  m_buffer[m_pos++] = c;
  //m_buffer[m_pos] = '\0';
  //else throw some_exception;
}

void
LexBuffer::unget()
{
  MCAssertValidInstance();
  if (m_pos > 0) {
    assert( 0 != m_buffer);
    --m_pos;
    m_buffer[m_pos] = '\0';
  }
}

std::string
LexBuffer::toString() const
{
  MCAssertValidInstance();
  m_buffer[m_pos] = '\0';
  return std::string( m_buffer);
}

std::ostream &
operator<<( std::ostream &os, const LexBuffer &lb)
{
  assert( 0 != &lb);
  return os << lb.m_buffer;
}

