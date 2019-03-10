/*
 * nodes.cc ---
 *
 *   Implementation of tagged memory cells on which the interpreter operates.
 *
 * (c) 1999 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: nodes.cpp,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#include <cstdlib> //free
#include <cstring> //strlen, strdup
#include <algorithm>
#include <cassert>
#include <lispel/defs.hh>
#include <lispel/exception.hh>
#include <lispel/nodes.hh>
#include <lispel/exception.hh>
#include <lispel/utilities.hh>
#include <lispel/reader.hh> //FIXME: contains decl of printList
#include <lispel/environment.hh>

const Handle::flagstype Handle::CONSTFLAG    = 0x00000001;
const Handle::flagstype Handle::SPECIALFLAG  = 0x00000002;
const Handle::flagstype Handle::WATCHFLAG    = 0x00000004;
const Handle::flagstype Handle::GCSAFEFLAG   = 0x00000008;

Handle::Handle( )
{
  m_nodetype = ntEMPTY;
  m_flags = 0UL;
  val.m_value = 0;
  m_refcount = 0;
}

Handle::~Handle()
{
  MCAssertValidInstance();
}

Handle_ptr Handle::car() throw( TypeException)
{
  MCAssertValidInstance();

  if (hasType( ntCONS))
    return val.m_cons.car;

  throw TypeException( "pair", __FILE__, __LINE__);
  MCAssertNotReached( 0);
}

const Handle *Handle::car() const throw( TypeException)
{
  MCAssertValidInstance();

  if (hasType( ntCONS))
    return val.m_cons.car;

  throw TypeException( "pair", __FILE__, __LINE__);

  MCAssertNotReached( 0);
}

Handle_ptr Handle::cdr() throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntCONS))
    return val.m_cons.cdr;

  throw TypeException( "pair", __FILE__, __LINE__);
  MCAssertNotReached( 0);
}

const Handle *Handle::cdr() const throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntCONS))
    return val.m_cons.cdr;

  throw TypeException( "pair", __FILE__, __LINE__);
  MCAssertNotReached( 0);
}

void Handle::setCar( Handle_ptr car) throw( TypeException)
{
  MCAssertValidInstance();
  assert( 0 != car);

  if (hasType( ntCONS)) { //FIXME: rc stuff
    val.m_cons.car = car;
  } else
    throw TypeException( "pair", __FILE__, __LINE__);
}

void Handle::setCdr( Handle_ptr cdr) throw( TypeException)
{
  MCAssertValidInstance();
  assert( 0 != cdr);

  if (hasType( ntCONS)) { //FIXME: rc stuff
    val.m_cons.cdr = cdr;
  } else
    throw TypeException( "pair", __FILE__, __LINE__);
}

const char *Handle::stringValue() const
  throw( TypeException, InternalInconsistency)
{
  MCAssertValidInstance();
  if (hasType( ntSTRING) || hasType( ntSYMBOL)) {
    StringValue *strval = dynamic_cast<StringValue*>( val.m_value);
    if (0 == strval)
      throw InternalInconsistency( "null string", __FILE__, __LINE__);
    return strval->stringValue();
  }
  throw TypeException( "string or symbol", __FILE__, __LINE__);

  MCAssertNotReached( 0);
}

char *Handle::stringValue()
  throw( TypeException, InternalInconsistency)
{
  MCAssertValidInstance();
  if (hasType( ntSTRING) || hasType( ntSYMBOL) || hasType( ntKEYWORD)) {
    StringValue *strval = dynamic_cast<StringValue*>( val.m_value);
    if (0 == strval)
      throw InternalInconsistency( "null string", __FILE__, __LINE__);
    return strval->stringValue();
  }
  throw TypeException( "string or symbol", __FILE__, __LINE__);

  MCAssertNotReached( 0);
}

double Handle::doubleValue() const throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntDOUBLE)) {
    return val.m_dblValue;
  } else if (hasType( ntINTEGER)) {
    return val.m_intValue;
  } else
     throw TypeException( "double", __FILE__, __LINE__);
}

Handle *Handle::body() throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntCLOSURE)) {
    ClosureValue *closure = dynamic_cast<ClosureValue*>( val.m_value);
    assert( 0 != closure);
    return closure->body();
  } else
    throw TypeException( "closure", __FILE__, __LINE__);
}

Environment *Handle::bindArguments( std::vector<Handle_ptr> &argValues)
  throw( TypeException)
{
   MCAssertValidInstance();
   if (!hasType( ntCLOSURE))
      throw TypeException( "closure", __FILE__, __LINE__);

   ClosureValue *closure = dynamic_cast<ClosureValue*>( val.m_value);
   assert( 0 != closure);
   return closure->bindArguments( argValues);
}

int Handle::integerValue() const throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntINTEGER) || hasType( ntCHARACTER)) {
    return val.m_intValue;
  }
  throw TypeException( "integer", __FILE__, __LINE__);
}

bool Handle::booleanValue() const throw( TypeException)
{
  MCAssertValidInstance();
  if (hasType( ntBOOLEAN)) {
    return val.m_boolValue;
  }
  throw TypeException( "boolean",  __FILE__, __LINE__);
}

void Handle::incRef()
{
  MCAssertValidInstance();
  ++m_refcount;
  if (hasType( ntCONS)) {
    if (0 != val.m_cons.car)
      val.m_cons.car->incRef();
    if (0 != val.m_cons.cdr)
      val.m_cons.cdr->incRef();

  } else if (hasType( ntARRAY)) {
    val.m_value->incRef();
  }
}

std::ostream &
operator<<( std::ostream &os, const Handle &h)
  throw( InternalInconsistency)
{
  MCAssert( 0 != &h && 0 != &os, "trying to print (null) handle");
  MCAssert( 0 != &os, "trying to print on (null) stream");

#if defined( DEBUG) && DEBUG > 4
  std::cerr << "operator<<( ostream &, const Handle &): "
       << typetagToString( h.m_nodetype) << endl;
#endif

  int i;
  switch( h.m_nodetype) {
  case Handle::ntEMPTY:
    throw InternalInconsistency( "access to empty memory cell",
					    __FILE__, __LINE__);

  case Handle::ntINTEGER:
    return os << h.val.m_intValue;

  case Handle::ntDOUBLE:
    return os << h.val.m_dblValue;

  case Handle::ntBOOLEAN:
    if (h.val.m_boolValue)
       return os << "#t";
    else
       return os << "#f";

  case Handle::ntCHARACTER:
    return os << static_cast<char>( h.val.m_intValue);

  case Handle::ntCONS:
    if (h.isNilRep())
      return os << "nil";
    else {
      return printList( &h, os);
    }

  case Handle::ntAVECTOR:
    os << "#(";
    for (i=0; i<h.val.m_avector.length; ++i)
      os << ' ' << h.val.m_avector.data[i];
    return os << ')';

  case Handle::ntSTRING:
  case Handle::ntSYMBOL:
  case Handle::ntKEYWORD:
  case Handle::ntARRAY:
  case Handle::ntOBJECT:
  case Handle::ntPORT:
  case Handle::ntCLOSURE:
  case Handle::ntHASHTABLE:
  case Handle::ntSET:
    h.val.m_value->printToStream( os);
    return os;

  case Handle::ntCFUNC:
    return os << "#<builtin@" << &h << '>';
  }

  MCAssertNotReached( os);
  return os;
}


/*
 * NodeValue ---
 */
NodeValue::NodeValue() : m_refcount( 0)
{
}

NodeValue::~NodeValue()
{
  //FIXME: should explicit deallocation be allowed for shared instances?
  MCAssert( 1 >= m_refcount, "destroying NodeValue instance with refcount > 0");
}


void
NodeValue::finalize()
{
  MCAssertValidInstance();
}

NodeValue *
NodeValue::eval()
{
  MCAssertValidInstance();
  return this;
}

void
NodeValue::incRef()
{
  MCAssertValidInstance();
  ++m_refcount;
}

void
NodeValue::decRef()
{
  MCAssertValidInstance();
  --m_refcount;
}

bool
NodeValue::isShared()
{
  MCAssertValidInstance();
  return (1 != m_refcount);
}

//  std::ostream &NodeValue::printToStream( std::ostream &os) const
//  {
//    MCAssertValidInstance();
//    return os << "#<empty@" << this << '>';
//  }

void
NodeValue::incRef1Func( NodeValue *val)
{
  val->incRef();
}

void
NodeValue::decRef1Func( NodeValue *val)
{
  val->decRef();
}


bool
NodeValue::isSequence()
{
  return false;
}

OutputIterator *
NodeValue::getOutputIterator()
{
  return 0;
}

InputIterator *
NodeValue::getInputIterator()
{
  return 0;
}

ForwardIterator *
NodeValue::getForwardIterator()
{
  return 0;
}

BidirectionalIterator *
NodeValue::getBidirectionalIterator()
{
  return 0;
}

RandomAccessIterator *
NodeValue::getRandomAccessIterator()
{
  return 0;
}


/*
 * StringValue ---
 */
StringValue::StringValue( const char *s, bool quote)
{
  m_string = strdup( s);
  m_len = strlen( s);
  m_quote = quote;
}

StringValue::StringValue( const char *s)
{
  m_string = strdup( s);
  m_len = strlen( s);
  m_quote = true;
}

StringValue::~StringValue()
{
  finalize();
}

void StringValue::finalize()
{
  MCAssertValidInstance();
  if (0 != m_string) {
    free( m_string);
    m_string = 0;
    m_len = 0;
  }
}

const char *
StringValue::stringValue() const
{
  MCAssertValidInstance();
  return m_string;
}

char *
StringValue::stringValue()
{
  MCAssertValidInstance();
  return m_string;
}

int
StringValue::length() const
{
  MCAssertValidInstance();
  return m_len;
}

std::ostream &
StringValue::printToStream( std::ostream &os) const
{
  if (m_quote)
    return os << '"' << m_string << '"';
  else
    return os << m_string;
}


/*
 * VectorValue ---
 */
VectorValue::VectorValue( const std::vector<Handle*> &vals)
{
  m_values = vals;
}

VectorValue::~VectorValue()
{
  MCAssertValidInstance();
  finalize();
}

void
VectorValue::finalize()
{
  MCAssertValidInstance();
  //array_type::iterator pos;
  //for (pos = m_values.begin(); pos != m_values.end(); ++pos)
  //  (*pos)->finalize();
}

void
VectorValue::incRef()
{
  VectorValue::iterator pos;
  for (pos=m_values.begin(); pos!=m_values.end(); ++pos)
    (*pos)->incRef();
}

const Handle *
VectorValue::elementAt( int i) const
  throw( RecoverableException)
{
  MCAssertValidInstance();
  if (i < static_cast<int>( m_values.size()))
    return m_values[i];
  else
    //FIXME: create proper hierarchy of exceptions
    throw RecoverableException( "index out of bounds",
					   __FILE__, __LINE__);
}

Handle *
VectorValue::elementAt( int i) throw( RecoverableException)
{
  MCAssertValidInstance();
  if (i < static_cast<int>( m_values.size()))
    return m_values[i];
  else
    //FIXME: create proper hierarchy of exceptions
    throw RecoverableException( "index out of bounds",
					   __FILE__, __LINE__);
}

VectorValue::size_type VectorValue::length() const
{
  MCAssertValidInstance();
  return m_values.size();
}

std::ostream &
VectorValue::printToStream( std::ostream &os) const
{
  VectorValue::const_iterator pos;
  os << "#(";
  for (pos=begin(); pos!=end(); ++pos)
    os << ' ' << *(*pos);
  return os << ')';
}


/*
 * PortValue ---
 */
PortValue::PortValue()
{
}

PortValue::~PortValue()
{
  MCAssertValidInstance();
}

std::ostream &
PortValue::printToStream( std::ostream &os) const
{
  MCAssertValidInstance();
  return os << "#<port@" << this << '>';
}


/*
 * CommandImpl ---
 */
CommandImpl::CommandImpl() : m_name( "")
{
}

CommandImpl::~CommandImpl()
{
}

void
CommandImpl::setName( const std::string &name)
{
  m_name = name;
}

const std::string &
CommandImpl::name() const
{
  return m_name;
}

std::string
CommandImpl::name()
{
  return m_name;
}

std::ostream &
CommandImpl::printToStream( std::ostream &os) const
{
  throw InternalInconsistency( "printing CommandImpl base class",
					  __FILE__, __LINE__);
}


/*
 * BuiltinValue ---
 */
BuiltinValue::BuiltinValue()
{
}


BuiltinValue::~BuiltinValue()
{
  MCAssertValidInstance();
}

std::ostream &
BuiltinValue::printToStream( std::ostream &os) const
{
  MCAssertValidInstance();
  return os << "#<builtin@" << this << " \"" << name() << "\">"
	    << std::endl;
}


/*
 * CBuiltin ---
 */
CBuiltinAdapter::CBuiltinAdapter( CBuiltinAdapter::cimpl cfunc)
{
   m_func = cfunc;
}

CBuiltinAdapter::~CBuiltinAdapter()
{
}

Handle_ptr
CBuiltinAdapter::execute( Context &ctx, Environment *env,
         std::vector<Handle_ptr> args)
{
   return (*m_func)(this, ctx, env, args);
}


/*
 * BinaryPredicateAdapter ---
 */

BinaryPredicateAdapter::BinaryPredicateAdapter( BinaryPredicateAdapter::binary_predicate pred)
{
  m_pred = pred;
}

BinaryPredicateAdapter::~BinaryPredicateAdapter()
{
   MCAssertValidInstance();
}

Handle_ptr
BinaryPredicateAdapter::execute( Context &ctx, Environment *env,
				 std::vector<Handle_ptr> args)
{
  if (2 != args.size()) {
    std::cerr << "error: " << name() << " needs exactly 2 arguments"
              << std::endl;
    return ctx.NIL;
  }

  return ((*m_pred)( args[0], args[1]))? ctx.TRUE : ctx.FALSE;
}

/*
 * SpecialValue ---
 */
SpecialValue::SpecialValue()
{
}

SpecialValue::~SpecialValue()
{
  MCAssertValidInstance();
}

std::ostream &
SpecialValue::printToStream( std::ostream &os) const
{
  MCAssertValidInstance();
  return os << "#<special@" << this << " \"" << name() << "\">";
}


/*
 * ObjectValue ---
 */

ObjectValue::ObjectValue()
{
}

ObjectValue::~ObjectValue()
{
  MCAssertValidInstance();
}


/*
 * ClosureValue ---
 */

ClosureValue::ClosureValue( std::list<Handle_ptr> argNames, Environment *env,
                            Handle_ptr body)
{
  assert( 0 != env && 0 != body);
  std::list<Handle_ptr>::iterator pos;
  for (pos=argNames.begin(); pos!=argNames.end(); ++pos)
    m_argumentNames.push_back( std::string( (*pos)->stringValue()));
  m_body = body;
  m_env = env;
}

ClosureValue::~ClosureValue()
{
  MCAssertValidInstance();
}

std::ostream &
ClosureValue::printToStream( std::ostream &os) const
{
  MCAssertValidInstance();
  return os;
}

int
ClosureValue::arity() const
{
  MCAssertValidInstance();
  return m_argumentNames.size();
}

Handle_ptr
ClosureValue::body()
{
  MCAssertValidInstance();
  return m_body;
}

const
Handle_ptr ClosureValue::body() const
{
  MCAssertValidInstance();
  return m_body;
}

void
ClosureValue::incRef()
{
  MCAssertValidInstance();
  ++m_refcount;
  assert( 0 != m_body);
  m_body->incRef();
}

Environment *
ClosureValue::bindArguments( std::vector<Handle_ptr> &argValues)
{
  MCAssertValidInstance();
  if (argValues.size() != m_argumentNames.size())
    throw ArgumentCountException( m_argumentNames.size(), __FILE__, __LINE__);

  else {
    // number of actual parameters matches closure's arity
    Environment *newEnv = m_env->makeChildEnvironment();
    std::vector<std::string>::iterator namePos = m_argumentNames.begin();
    std::vector<Handle_ptr>::iterator valPos = argValues.begin();
    while ((namePos != m_argumentNames.end()) && (valPos != argValues.end())) {
      newEnv->put( *namePos, *valPos);
      ++namePos, ++valPos;
    }
    return newEnv;
  }

  MCAssertNotReached( 0);
}

void
ClosureValue::collectReferencedEnvironments( std::set<Environment*> &active)
{
  MCAssertValidInstance();
  Environment *currentEnv = m_env;
  while (0 != currentEnv) {
    active.insert( currentEnv);
    currentEnv = currentEnv->getParent();
  }
}


/*
 * Hashtable ---
 */
Hashtable::Hashtable()
{
}

Hashtable::~Hashtable()
{
   MCAssertValidInstance();
}

Handle_ptr
Hashtable::lookup( const std::string &key)
{
  iterator pos = m_table.find( key);
  if (m_table.end() == pos)
    return 0;
  return (*pos).second;
}

void
Hashtable::put( const std::string &key, Handle_ptr value)
{
  m_table[key] = value;
}

int
Hashtable::size()
{
  return m_table.size();
}

void
Hashtable::clear()
{
  m_table.erase( m_table.begin(), m_table.end());
}

bool
Hashtable::remove( const std::string &key)
{
  if (exists( key)) {
    m_table.erase( m_table.find( key));
    return true;
  }
  return false;
}

bool
Hashtable::exists( const std::string &key)
{
  return (m_table.end() != m_table.find( key));
}

std::ostream &
Hashtable::printToStream( std::ostream &os) const
{
  return os << "#<hashtable@" << this << ">";
}


/*
 * SetValue ---
 */

SetValue::SetValue()
{
}

SetValue::~SetValue()
{
   MCAssertValidInstance();
}

bool
SetValue::lookup( Handle_ptr value)
{
  MCAssertValidInstance();
  iterator pos = m_set.find( value);
  if (m_set.end() == pos)
    return false;
  return (*pos);
}

void
SetValue::put( Handle_ptr value)
{
  MCAssertValidInstance();
  m_set.insert( value);
}

bool
SetValue::remove( Handle_ptr value)
{
  MCAssertValidInstance();
  iterator pos = m_set.find( value);
  if (m_set.end() == pos)
    return false;
  m_set.erase( pos);
  return true;
}

int
SetValue::size()
{
  MCAssertValidInstance();
  return m_set.size();
}

void
SetValue::clear()
{
  MCAssertValidInstance();
  m_set.erase( m_set.begin(), m_set.end());
}

std::ostream &
SetValue::printToStream( std::ostream &os) const
{
  MCAssertValidInstance();
  //FIXME: might be printed as list
  return os << "#<set@" << this << ">";
}

