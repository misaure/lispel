/*
 * interpreter.cpp ---
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: interpreter.cpp,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#include <fstream>
#include <miscclass/defs.hh>
#include <lispel/interpreter.hh>
#include <lispel/reader.hh>
#include <lispel/userinterface.hh>
#include <lispel/exception.hh>

Interpreter *Interpreter::M_globalInstance = 0;

Interpreter *Interpreter::instance()
{
  if (0 == M_globalInstance)
    M_globalInstance = new Interpreter();
  return M_globalInstance;
}

Interpreter::Interpreter()
{
  DefaultComponentFactory factory;

  m_ctx.toplevel = factory.createToplevelEnvironment();
  m_ctx.factory = factory.getGlobalNodeFactory( m_ctx, 65535UL);
  m_ctx.eval = factory.createEvaluator( m_ctx);
  m_ctx.interp = this;
  m_ctx.NIL = m_ctx.factory->makeNil();
  m_ctx.TRUE = m_ctx.factory->makeBoolean( true);
  m_ctx.FALSE = m_ctx.factory->makeBoolean( false);

  m_ctx.NIL->setFlag( Handle::GCSAFEFLAG);
  m_ctx.TRUE->setFlag( Handle::GCSAFEFLAG);
  m_ctx.FALSE->setFlag( Handle::GCSAFEFLAG);
  m_gc = factory.createGC( m_ctx);

  if (!M_lexer_initialized) {
    initLexerModule();
    M_lexer_initialized = true;
  }
  m_exitRequested = m_gcRequested = false;
}

Interpreter::Interpreter( Interpreter::ComponentFactory *factory)
{
  m_ctx.toplevel = factory->createToplevelEnvironment();
  m_ctx.factory = factory->getGlobalNodeFactory( m_ctx, 65535UL);
  m_ctx.eval = factory->createEvaluator( m_ctx);
  m_ctx.interp = this;
  m_ctx.NIL = m_ctx.factory->makeNil();
  m_ctx.TRUE = m_ctx.factory->makeBoolean( true);
  m_ctx.FALSE = m_ctx.factory->makeBoolean( false);
  m_gc = factory->createGC( m_ctx);

  m_ctx.NIL->setFlag( Handle::GCSAFEFLAG);
  m_ctx.TRUE->setFlag( Handle::GCSAFEFLAG);
  m_ctx.FALSE->setFlag( Handle::GCSAFEFLAG);

  if (!M_lexer_initialized) {
    initLexerModule();
    M_lexer_initialized = true;
  }
  m_exitRequested = m_gcRequested = false;
}

Interpreter::~Interpreter()
{
  MCAssertValidInstance();
  if (0 != m_ctx.eval)
    delete m_ctx.eval;

  if (0 != m_ctx.toplevel)
    delete m_ctx.toplevel;

  if (0 != m_ctx.factory)
    delete m_ctx.factory;
}

bool Interpreter::repl( std::istream *stream, UserInterface *ui)
{
  MCAssertValidInstance();
  assert( 0 != stream);
  SimpleReader reader( m_ctx);
  Handle_ptr retval;

  int iterationsSinceLastGC = 0;
  while (!m_exitRequested) {
    ui->displayPrompt();
    if (0 == (retval = reader.read( stream)))		// read expression
       break;
    Handle_ptr evaluated = 0;
    try {
       evaluated = m_ctx.eval->eval( retval); 		// evaluate expression
    }
    catch( miscclass::RecoverableException rex) {
       ui->warning( rex.what());
    }
    if (0 != evaluated)
      ui->displayExpression( evaluated);
    ++iterationsSinceLastGC;

    // perform garbage collection when less then half of the heap space is
    // left.
    //FIXME: this definitely needs some more intelligent heuristics!
    if (m_gcRequested ||
        (m_ctx.factory->size()/2 > m_ctx.factory->freeCount())) {
#if defined( DEBUG) && DEBUG > 1
       std::cout << "performing garbage collection..."; std::cout.flush();
       std::cout << m_gc->fullGC() << " cells freed" << std::endl;
#else
       m_gc->fullGC();
#endif
       iterationsSinceLastGC = 0;
       m_gcRequested = false;
    }
  }
  m_exitRequested = false;				// reset exit request

  return true;
}

bool Interpreter::source( const std::string &filename)
{
  MCAssertValidInstance();

  SimpleReader reader( m_ctx);
  std::ifstream infile( filename.c_str());
  if (!infile)
    return false;

  int iterationsSinceLastGC = 0;
  while (!m_exitRequested) {
    Handle_ptr retval;
    if (0 == (retval = reader.read( &infile)))		// read expression
       break;
    m_ctx.eval->eval( retval); 	// evaluate expression
    ++iterationsSinceLastGC;

    // perform garbage collection when less then half of the heap space is
    // left.
    if (m_gcRequested || (m_ctx.factory->size()/2 > m_ctx.factory->freeCount()))
    {
       m_gc->fullGC();
       iterationsSinceLastGC = 0;
       m_gcRequested = false;
    }
  }
  m_exitRequested = false;				// reset exit request

  return true;
}

void Interpreter::toplevelDefine( const std::string &key, Handle_ptr value)
{
  MCAssertValidInstance();
  assert( 0 != m_ctx.toplevel);
  m_ctx.toplevel->put( key, value);
}

Handle_ptr Interpreter::toplevelLookup( const std::string &key)
{
  MCAssertValidInstance();
  assert( 0 != m_ctx.toplevel);
  return m_ctx.toplevel->lookup( key);
}

Handle_ptr Interpreter::addBuiltin( const std::string &name, BuiltinValue *cmd)
{
  MCAssertValidInstance();
  Handle_ptr newBuiltin = m_ctx.factory->makeValue( Handle::ntCFUNC, cmd);
  toplevelDefine( name, newBuiltin);
  cmd->setName( name);

  return newBuiltin;
}

Handle_ptr Interpreter::addBuiltin( const std::string &name,
				    CBuiltinAdapter::cimpl cmd)
{
  MCAssertValidInstance();
  return addBuiltin( name, new CBuiltinAdapter( cmd));
}

Handle_ptr Interpreter::addSpecial( const std::string &name, SpecialValue *cmd)
{
  MCAssertValidInstance();
  Handle_ptr newSpecial = m_ctx.factory->makeValue( Handle::ntCFUNC, cmd);
  newSpecial->setFlag( Handle::SPECIALFLAG);
  toplevelDefine( name, newSpecial);
  cmd->setName( name);

  return newSpecial;
}

void Interpreter::gc()
{
  MCAssertValidInstance();
  m_gcRequested = true;
}

unsigned int Interpreter::cellCount()
{
  MCAssertValidInstance();
  return 0;
}


Interpreter::DefaultComponentFactory::DefaultComponentFactory()
{
}

Interpreter::DefaultComponentFactory::~DefaultComponentFactory()
{
}

Environment *
Interpreter::DefaultComponentFactory::createToplevelEnvironment()
{
  MCAssertValidInstance();
  return new Environment();
}

NodeFactory *
Interpreter::DefaultComponentFactory::GLOBAL_NODE_FACTORY = 0;

NodeFactory *
Interpreter::DefaultComponentFactory::getGlobalNodeFactory( Context &ctx, NodeFactory::size_type initSize)
{
  MCAssertValidInstance();
  if (0 == GLOBAL_NODE_FACTORY)
    GLOBAL_NODE_FACTORY = new NodeFactory( ctx, initSize);
  return GLOBAL_NODE_FACTORY;
}

Evaluator *
Interpreter::DefaultComponentFactory::createEvaluator( Context &ctx)
{
  MCAssertValidInstance();
  SimpleEvaluator *eval = new SimpleEvaluator( ctx);
  //eval->useToplevel( ctx.toplevel);

  return eval;
}

GarbageCollector *
Interpreter::DefaultComponentFactory::createGC( Context &ctx)
{
  MCAssertValidInstance();
  assert( 0 != ctx.factory);
  return new SimpleGC( ctx);
}

bool Interpreter::M_lexer_initialized = false;
