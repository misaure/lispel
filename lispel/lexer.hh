/*
 * lexer.hh ---
 *
 *   Lexical scanner for Scheme.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: lexer.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_LEXER_HH_
#define _lispel_LEXER_HH_ 1

#include <string>
#include <iostream>
#include <cstdlib>
#include <stack>
#include <map>
#include <lispel/pointer.hh>

/**
   @name Lexical Scanner Routines
   The classes and functions in this sections make up the lexical analysis part
   of the LispScript interpreter. They should be general enough to be used in
   other contexts, for example for reading configuration files. The only thing
   you should have to change is the Scanner class itself and possibly the
   character class specification.<p>

   The default character class specification is created with a call to the
   (global) initLexerModule function. There is currently no standard
   interface to the character class table, so you would have to change the
   code in lexer.cc to make the definitions suit your needs. This will
   hopefully change in one of the next releases.
*/
//@{


/**
   LexBuffer objects are dynamically allocated character buffers which
   can be used for recording input character by character.
   @memo Recording scanner input.
   @version 0.1
*/
class LexBuffer {
public:
  typedef size_t size_type;

  /**
     Create a read buffer capable of holding the given number of characters.
  */
  explicit LexBuffer( size_type size);
  ~LexBuffer();

  /**
     Append a character to the read buffer. This method also appends a zero
     character so that the buffer always holds a valid C string.
  */
  void append( char c);

  void unget();

  void operator--() { unget(); }

  /**
     A shortcut for the append operation.
  */
  void operator+=( char c) { append( c); }

  /**
     Clears the buffer so it can be reused for storing a new value.
  */
  void reset() { m_pos = 0; }

  /**
    Get number of characters added so far, i.e. the length of the string.
    */
  size_type size() const { return m_pos; }

  /**
     Return the buffers content as a STL string.
  */
  std::string toString() const;

  /**
     Print the buffer's contents.
  */
  friend std::ostream &operator<<( std::ostream &os, const LexBuffer &lb);

protected:
  int m_pos;
  char *m_buffer;
  size_type m_maxsize;
};

typedef ptr<LexBuffer> LexBuffer_var;
typedef LexBuffer *LexBuffer_ptr;

/**
   A string bucket can be used to store unique instances of equal strings.
   The advantage of doing this will be lower memory usage and that testing
   strings for equality boils down to a pointer comparison. The drawback of
   using this class is, of course, a slight runtime overhead when strings
   are inserted. The implementation uses explicit reference counting to 
   determine when strings can be deleted.
   @version 0.1
 */
class StringBucket {
public:
  StringBucket();
  ~StringBucket();

  /**
     Inserts a string into the bucket if it doesn't already exist and returns
     the unique instances address. If the string already exists, its 
     reference count will be increased by one.
  */
  const char *internString( const char *s);

  /**
     Decrease the strings reference count by one and delete it once the
     reference count drops to zero.
   */
  void releaseString( const char *s);

protected:
  typedef std::map<const char*,int> bucket_type;
  typedef std::map<const char*,int>::iterator bucket_iterator;
  bucket_type m_bucket;
};

/**
   <b>FIXME:</b> add pos in source code
   @memo General token and scanner class.
   @version 0.2
*/
class Token {
public:
  typedef enum {
    ttEOF, ttLPAREN, ttRPAREN, ttID, ttKEYWORD,
    ttINT, ttDOUBLE, ttSTRING, ttVECSTART,
    ttOPERATOR, ttERROR, ttFALSE, ttTRUE, ttCHARACTER, 
    ttQUOTECHAR, ttMAX
  } TokenType;

  /**
     Create a token instance which is marked invalid (token number -1).
  */
  Token();

  /**
     Initializes a token instance to a certain type.
     @param tokennum A numeric tag specifying the token's type.
     @param lexval The input read by the scanner.
  */
  Token( TokenType tokennum, const std::string &lexval);
  virtual ~Token();

  /**
     Get the numeric tag specifying the token's type.
     @return The numeric tag specifying the token's type.
  */
  inline TokenType tokennum() const { return m_tokennum; }
  inline bool hasType( TokenType t) const { return m_tokennum == t; }

  /**
     Get the lexical value of the token, which normally is the input read
     by the scanner. For some tokens, especially single character tokens,
     the lexical value will be an empty string.
  */
  inline std::string lexval() const { return m_lexval; }

  /**
    This method is a hook for manipulating a symbol's lexical value before
    after it has been read and before it is passed back to the caller. 
    Possible applications of this method include building an atom table to
    save memory space and speed up symbol comparisons or simply convert all
    symbol to lower case to build a case insensitive interpreter.<p>
    The current implementation is a no-op.
    @param s Symbol's lexical value
    @return Converted value, which in the current implementation is the 
    identical to the argument's value.
   */
  virtual std::string convertSymbol( std::string s);

  /**
     Write the lexical value and type of a token to an output stream.
     This function is mainly provided for debugging purposes.
  */
  friend std::ostream &operator<<( std::ostream &os, const Token &t);

  /**
     This is a wrapper for the complete scanner.
  */
  friend std::istream &operator>>( std::istream &is, Token &t);

protected:
  static LexBuffer::size_type M_LEXBUFFERSIZE;
  TokenType m_tokennum;
  std::string m_lexval;
};

typedef ptr<Token> Token_var;
typedef Token *Token_ptr;


extern const char *tokennumToString( Token::TokenType tokennum);

/**
   Translate the character name part of a string constant to a single
   character. All character names of length 1 represent the character itself.
   @return Returns -1 if the character name can't be translated to a
   character.
*/
int translateCharacter( const std::string &charname);

/**
   This function must be called once before any scanner functionalities
   are used because it initializes the character classification table used
   by the scanner.
   @memo Initialize lexical scanner subsytem.
*/
void initLexerModule();

/**
   Print all character which are regarded as valid input characters, appended
   by their types.
*/
void printChartypes();

//@}

#endif /*_lispel_LEXER_HH_*/
