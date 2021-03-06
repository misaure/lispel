/*
 * exception.hh ---
 *
 *   This defines a common base class for the exceptions thrown by the various
 *   Lispel classes so they can be caught separately from exceptions thrown
 *   by the standard library.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: exception.hh,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#ifndef _lscript_EXCEPTION_HH_
#define _lscript_EXCEPTION_HH_ 1

#include <iostream>
#include <string>
//#include <miscclass/exception.hh>

/**
   @name Standard Exceptions
   This section contains a small hierarchy of Lispel standard exceptions.
   The most important branches are the RecoverableExceptions and the
   FatalExceptions. The latter will stop the sample shell to immediately
   stop execution while the first lead to a runtime error message. When you
   build an application with a Lispel exception, you should at least ensure
   that your application continues after a RecoverableException occurred.
*/
//@{

class Exception {
public:
	static const int LINE_UNDEFINED = -1;

    Exception();
    Exception(std::string what);
    Exception(std::string what, std::string where, int line);
    virtual ~Exception() throw();

    const std::string &what() const { return m_what; }
    const std::string &where() const { return m_where; }
    int line() const { return m_line; }

protected:
    std::string m_what;
    std::string m_where;
    int m_line;
};

extern std::ostream &operator<<( std::ostream &os, const Exception e);

class RecoverableException : public Exception {
public:
	RecoverableException(std::string message);
	RecoverableException(std::string message, std::string where, int line);
	virtual ~RecoverableException() throw();
};

class InternalInconsistency : public Exception {
public:
    //InternalInconsistency();
    InternalInconsistency(std::string message);
    InternalInconsistency(std::string message, std::string where, int line);
    virtual ~InternalInconsistency() throw();
};

/**
   Thrown when a function has been applied to an incompatible type.
*/
class TypeException : public RecoverableException {
public:
  explicit TypeException( std::string expected);
  /**
     Create a exception with full error and debugging information.
     @param excpected A string describing what parameter type(s) were expected.
  */
  TypeException( std::string expected, std::string where, int line);
  TypeException();
  virtual ~TypeException() throw();
};


/**
   Thrown when lookup of a symbols value fails.
*/
class UndefinedValue : public RecoverableException {
public:
  /**
     Create a exception instance with full error and debuggin information.
     @param symname Name of the symbol that couldn't be resolved.
  */
  UndefinedValue( std::string symname, std::string where, int line);
  UndefinedValue( std::string symname);
  virtual ~UndefinedValue() throw();
};


/**
   Signals that the number of actual arguments didn't match a functions
   arity during application.
*/
class ArgumentCountException : public RecoverableException {
public:
  /**
     Create an ArgumentCountException with full debugging information.
     The exact error message produced by this exception depends on the
     signedness of the <code>expected</code> parameter.
     @param expected This paramter basically gives the number of parameters
     that were expected. If <code>expected</code is a negative value then the
     error message produced by throwing this exception will tell that
     <i>at least</i> <code>-expected</code> paramters have been expected.
   */
  ArgumentCountException( int expected, std::string where, int line);
  ArgumentCountException();
  virtual ~ArgumentCountException() throw();
};

//@}

#endif /*_lscript_EXCEPTION_HH_*/

