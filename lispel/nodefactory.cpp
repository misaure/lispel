#include <miscclass/defs.hh>
#include <lispel/nodes.hh>
#include <lispel/nodefactory.hh>
#include <lispel/lexer.hh>
#include <lispel/environment.hh>

#ifndef _
# define _( ASTRING) ASTRING
#endif

NodeFactory::NodeFactory( Context &ctx, NodeFactory::size_type s)
  : m_ctx( ctx)
{
  m_cells = new Handle[m_size=s];
  int i;
  for (i=0; i<static_cast<int>(m_size); ++i)
    m_free.push_back( i);
#ifdef DEBUG
  std::cerr << sizeof( Handle)*m_size << " bytes allocated for heap" << std::endl;
#endif
}

NodeFactory::~NodeFactory()
{
  MCAssertValidInstance();
  //FIXME: really needs some more intelligent handling
  //delete [] m_cells;
}

Handle_ptr NodeFactory::makeNil()
{
  MCAssertValidInstance();
  Handle_ptr nilNode = allocNode( Handle::ntCONS);
  nilNode->val.m_cons.car = nilNode->val.m_cons.cdr = 0;

  return nilNode;
}

Handle_ptr NodeFactory::makeValue( Handle::NodeType type, NodeValue *val)
{
   MCAssertValidInstance();
   Handle_ptr newHandle = allocNode( type);
   newHandle->val.m_value = val;

   return newHandle;
}

Handle_ptr NodeFactory::makeCharacter( int c)
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntCHARACTER);
  newHandle->val.m_intValue = c;

  return newHandle;
}

Handle_ptr NodeFactory::makeBoolean( bool b)
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntBOOLEAN);
  newHandle->val.m_boolValue = b;

  return newHandle;
}

Handle_ptr NodeFactory::makeInteger( int v)
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntINTEGER);
  newHandle->val.m_intValue = v;

  return newHandle;
}

Handle_ptr NodeFactory::makeDouble( double d)
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntDOUBLE);
  newHandle->val.m_dblValue = d;

  return newHandle;
}

Handle_ptr NodeFactory::makeString( const char *s)
{
  MCAssertValidInstance();
  assert( 0 != s);

  Handle_ptr newHandle = allocNode( Handle::ntSTRING);
  newHandle->val.m_value = new StringValue( s);
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}

Handle_ptr NodeFactory::makeSymbol( const char *s)
{
  MCAssertValidInstance();
  assert( 0 != s);

  Handle_ptr newHandle = allocNode( Handle::ntSYMBOL);
  newHandle->val.m_value = new StringValue( s, false);
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}

Handle_ptr NodeFactory::makeKeyword( const char *s)
{
  MCAssertValidInstance();
  assert( 0 != s);

  Handle_ptr newHandle = allocNode( Handle::ntKEYWORD);
  newHandle->val.m_value = new StringValue( s, false);
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}


Handle_ptr NodeFactory::makeVector( std::vector<Handle*> &v)
{
  MCAssertValidInstance();

  Handle_ptr newHandle = allocNode( Handle::ntARRAY);
  newHandle->val.m_value = new VectorValue( v);
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}

Handle_ptr NodeFactory::makePort( PortValue *pv)
{
  MCAssertValidInstance();
  assert( 0 != pv);
  Handle_ptr newHandle = allocNode( Handle::ntPORT);
  newHandle->val.m_value = pv;
  
  return newHandle;
}

Handle_ptr NodeFactory::makeHashtable()
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntHASHTABLE);
  newHandle->val.m_value = new Hashtable();
  assert( 0 != newHandle->val.m_value);
  
  return newHandle;
}

Handle_ptr NodeFactory::makeSet()
{
  MCAssertValidInstance();
  Handle_ptr newHandle = allocNode( Handle::ntSET);
  newHandle->val.m_value = new SetValue();
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}

Handle_ptr NodeFactory::makeCons()
{
  MCAssertValidInstance();
  return makeCons( m_ctx.NIL, m_ctx.NIL);
}

Handle_ptr NodeFactory::makeCons( Handle_ptr car)
{
  MCAssertValidInstance();
  return makeCons( car, m_ctx.NIL);
}

Handle_ptr NodeFactory::makeCons( Handle_ptr car, Handle_ptr cdr)
{
  MCAssertValidInstance();
  assert( 0 != car && 0 != cdr);
  Handle_ptr newHandle = allocNode( Handle::ntCONS);
  newHandle->setCar( car);
  newHandle->setCdr( cdr);

  return newHandle;
}

Handle_ptr NodeFactory::makeClosure( std::list<Handle_ptr> args, Environment *env,
				     Handle_ptr functionBody)
{
  MCAssertValidInstance();
  assert( 0 != env && 0 != functionBody);
  Handle_ptr newHandle = allocNode( Handle::ntCLOSURE);
  
  newHandle->val.m_value = new ClosureValue( args, env, functionBody);
  assert( 0 != newHandle->val.m_value);

  return newHandle;
}

/*
 * NodeFactory protected methods
 */


// HINT: this should be turned into a global function which takes a token and
// a NodeFactory as an argument and returns a new Handle
Handle_ptr NodeFactory::nodeFromToken( const Token_ptr tok)
  throw( miscclass::InternalInconsistency)
{
  MCAssertValidInstance();
  assert( 0 != tok);

  double doubleVal = 0.0; //initializers are here to make some compilers happy
  int intVal = 0;

  switch (tok->tokennum()) {
  case Token::ttSTRING:
    return makeString( tok->lexval().c_str());

  case Token::ttID:
  case Token::ttOPERATOR:
    return makeSymbol( tok->lexval().c_str());

  case Token::ttKEYWORD:
    return makeKeyword( tok->lexval().c_str());

  case Token::ttINT:
    intVal = atoi( tok->lexval().c_str());
    return makeInteger( intVal);

  case Token::ttDOUBLE:
    doubleVal = strtod( tok->lexval().c_str(), 0);
    return makeDouble( doubleVal);

  case Token::ttCHARACTER:
    if (-1 != (intVal = translateCharacter( tok->lexval())))
      return makeCharacter( intVal);
      //FIXME: need new exception for this case
    std::cerr << "nodeFromToken( Token::ttCHARACTER) failed!" << std::endl;
    throw miscclass::RecoverableException( "undefined character", __FILE__, __LINE__);

  case Token::ttFALSE:
    return m_ctx.FALSE;

  case Token::ttTRUE:
    return m_ctx.TRUE;

    //MISSING: add more cases here
  default:
    throw miscclass::InternalInconsistency( "NodeFactory::nodeFromToken called for unknown tag", __FILE__, __LINE__);
  }
}

NodeFactory::size_type NodeFactory::freeCount()
{
  MCAssertValidInstance();
  return m_free.size();
}

void NodeFactory::display( Handle_ptr handle, std::ostream *os)
{
  MCAssertValidInstance();
  assert( 0 != handle && 0 != os);
  (*os) << (*handle);
}

void NodeFactory::displayAll( std::ostream *os)
{
  MCAssertValidInstance();
  int i;
  for (i=0; i<static_cast<int>( m_size); ++i)
    if (Handle::ntEMPTY != m_cells[i].type())
      (*os) << i << '\t' << m_cells[i] << std::endl;
}

void NodeFactory::untagNonEmpty()
{
  size_type i;
  for (i=0; i<m_size; ++i)
    if (!Handle::ntEMPTY == m_cells[i].type())
      m_cells[i].m_refcount = 0;
#if defined( DEBUG) && DEBUG > 5
  for (i=0; i<m_size; ++i)
    if (!m_cells[i].hasType( Handle::ntEMPTY))
      std::cerr << "m_cells[" << i << "] = " << m_cells[i].m_refcount << std::endl;
#endif
}

int NodeFactory::recycleUntagged()
{
  int recycled = 0;
  size_type i;
  for (i=0; i<m_size; ++i) {
#if defined(DEBUG) && DEBUG > 5
    if (!(m_cells[i].hasType( Handle::ntEMPTY)))
      std::cerr << "m_cells[" << i <<"] == " << m_cells[i].m_refcount << std::endl;
#endif
    //non-empty untagged nodes can be recycled
    if (!(m_cells[i].hasType( Handle::ntEMPTY)) && (0 == m_cells[i].m_refcount)) {
      recycleNode( &m_cells[i]);
      ++recycled;
    }
  }

  return recycled;
}

std::set<Environment*> NodeFactory::activeEnvironments()
{
  std::set<Environment*> activeEnvs;
  size_type i;
  for (i=0; i<m_size; ++i)
    if (m_cells[i].hasType( Handle::ntCLOSURE)) {
      ClosureValue *closure = dynamic_cast<ClosureValue*>( m_cells[i].val.m_value);
      closure->collectReferencedEnvironments( activeEnvs);
    }

  return activeEnvs;
}

Handle_ptr NodeFactory::allocNode( Handle::NodeType nt)
  throw( MemoryExhausted)
{
  MCAssertValidInstance();
  if (!m_free.empty()) {
    int freeInd = m_free.front();
    m_free.pop_front();
    assert( (size_type)freeInd >= 0 && (size_type)freeInd < m_size);
    assert( m_cells[freeInd].hasType( Handle::ntEMPTY));
    m_cells[freeInd].m_nodetype = nt;
    return &m_cells[freeInd];
  } else
    throw MemoryExhausted( __FILE__, __LINE__);
}

void NodeFactory::recycleNode( Handle_ptr node)
{
  MCAssertValidInstance();
  if (0 == node)
    throw miscclass::InternalInconsistency( "NodeFactory::recyclyNode: cannot free node (null)", __FILE__, __LINE__);

  if (node->checkFlag( Handle::GCSAFEFLAG))
    return;

  Handle::NodeType nodetype = node->type();
  switch( nodetype) {
  case Handle::ntEMPTY:
    return;

  case Handle::ntAVECTOR:
    delete [] node->val.m_avector.data;
    break;

  case Handle::ntCHARACTER:
  case Handle::ntBOOLEAN:
  case Handle::ntINTEGER:
  case Handle::ntDOUBLE:
  case Handle::ntCONS: //FIXME: dec refcount of car and cdr, possibly free
    break;

  case Handle::ntPORT:   //FIXME: make m_value base type impl. default
  case Handle::ntSTRING: // after initial development phase
  case Handle::ntSYMBOL:
  case Handle::ntKEYWORD:
  case Handle::ntARRAY:
  case Handle::ntOBJECT:
  case Handle::ntCLOSURE:
  case Handle::ntHASHTABLE:
  case Handle::ntSET:
    assert( 0 != node->val.m_value);
    node->val.m_value->finalize();
    break;

  case Handle::ntCFUNC:
    break;
  }

  node->m_nodetype = Handle::ntEMPTY;
  m_free.push_front( node-m_cells);
}
