/*
 * exception.cpp ---
 *
 *   Lispel specific hierarchy of exception classes.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: exception.cpp,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#include <cstring>
#include <cstdlib>
#include <strstream>
#include <lispel/defs.hh>
#include <lispel/exception.hh>

Exception::Exception()
{
}

Exception::Exception(std::string _what) : m_what(_what)
{
}

Exception::Exception(std::string what, std::string where, int line) : m_what(what)
{
}

Exception::~Exception() throw()
{
}

std::ostream &operator<<( std::ostream &os, const Exception e)
{
    return os << e.what();
}

InternalInconsistency::InternalInconsistency(std::string message)
{
}

InternalInconsistency::InternalInconsistency(std::string message, std::string where, int line)
{
}
 
InternalInconsistency::~InternalInconsistency() throw()
{
}

RecoverableException::RecoverableException(std::string message)
{
}

RecoverableException::RecoverableException(std::string message, std::string where, int line)
{
}

RecoverableException::~RecoverableException() throw()
{
}


TypeException::TypeException( std::string expected, std::string where, int line)
  : RecoverableException( "type error: expected "+expected, where, line)
{
}

TypeException::TypeException()
  : RecoverableException( "type error") 
{
}

TypeException::~TypeException() throw()
{
}


UndefinedValue::UndefinedValue( std::string symname,
std::string where, int line)
  : RecoverableException( "undefined value '"+symname+"'", where, line)
{
}

UndefinedValue::UndefinedValue( std::string symname)
  : RecoverableException( "undefined symbol '"+symname+"'")
{
}

UndefinedValue::~UndefinedValue() throw()
{
}


ArgumentCountException::ArgumentCountException( int expected, std::string where,
						int line)
  : RecoverableException( "", where, line)
{
  std::ostrstream oss;
  oss << "invalid number of arguments: ";
  if (expected > 0)
     oss << "expected " << expected << " arguments" << std::ends;
  else if (0 == expected)
     oss << "no arguments required" << std::ends;
  else
     oss << "expected at least " << -expected << " arguments" << std::ends;
  m_what = oss.str();
}

ArgumentCountException::ArgumentCountException()
  : RecoverableException( "invalid number of arguments")
{
}

ArgumentCountException::~ArgumentCountException() throw()
{
}

