/*
 * reader.hh ---
 *
 *   Declarations for the reader, the parser component of the interpreter.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: reader.hh,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#ifndef _lispel_READER_HH_
#define _lispel_READER_HH_ 1

//#include <stack>
#include <pair.h>
#include <iostream>
#include <lispel/nodes.hh>
#include <lispel/pointer.hh>
#include <lispel/nodefactory.hh>
#include <lispel/context.hh>

class Scanner;

class Reader {
public:

  class ReadException : public miscclass::RecoverableException {
  public:
    ReadException( std::string what, std::string where, int line);
    virtual ~ReadException() throw();
  };

  Reader();
  virtual ~Reader();

  virtual Handle_ptr read( std::istream *stream) =0;
  virtual Handle_ptr readList( std::istream *stream) =0;
  virtual Handle_ptr readVector( std::istream *stream) =0;
  virtual Handle_ptr readAtom( std::istream *stream) =0;
};


/**
   This class contains the code which reconstructs the programs syntax tree
   from a linear sequence of tokens.
   @version 0.1
*/
class SimpleReader : public Reader {
public:
  /**
     Initialize a reader to use a certain NodeFactory instance. A node factory
     objects is a kind of memory cell dispenser, in a classical lisp
     implementation this would be e.g the heap. The default implementation
     provided by this class implements a simple recursive descent parser. This
     can, of course, be changed to some more efficient implementation by
     subclassing the Reader class and passing the derived class to the
     interpreter.
     @param nodeFactory A NodeFactory instance which produces tagged memory
     cells from tokens.
  */
  SimpleReader( Context &ctx);
  virtual ~SimpleReader();

  /**
     Try to read the next expression from the parser given as an argument.
     @param scanner Scanner which provides the tokens for the parser.
  */
  Handle_ptr read( std::istream *stream);

  /**
     Reads (possibly nested) lists and returns the list's first element.
     @return Head of the list read.
  */
  Handle_ptr readList( std::istream *stream);
  Handle_ptr readVector( std::istream *stream);
  Handle_ptr readAtom( std::istream *stream);

protected:
  /**
     Append an element to the list which is on top of the reader's stack.
  */
  Handle_ptr append( Handle_ptr node);

  /**
     The NodeFactory instance which is used to allocate new tagged memory cells
  */
  NodeFactory_ptr m_nodeFactory;

  /**
     The token last returned by the scanner.
  */
  Token m_currentToken;

  /**
     A small debugging aid.
  */
  inline void dbgPrint( const char *methodname);

  int m_indent;
  Context &m_ctx;
};

typedef ptr<SimpleReader> SimpleReader_var;
typedef Reader *Reader_ptr;

/*
 * Some handy helper functions.
 */
extern std::ostream &printList( const Handle *node, std::ostream &os);

#endif /*_lispel_READER_HH_*/
