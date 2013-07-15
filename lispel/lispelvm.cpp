/*
 * lispelvm.cpp ---
 *
 *  A simple bytecode-interpreting virtual machine implementation for Lispel.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: lispelvm.cpp,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#include <algorithm>
#include <lispel/defs.hh>
#include <lispel/exception.hh>
#include <lispel/lispelvm.hh>
#include <lispel/utilities.hh>


/*
 * CallFrame ---
 */

CallFrame::CallFrame()
{
  PC = 0;
  CE = 0;
}

CallFrame::CallFrame( Registers &regs)
  : PC( regs.PC), VAL( regs.VAL), CE( regs.CE)
{
}

CallFrame::~CallFrame()
{
}


/*
 * ActivationStack ---
 */

ActivationStack::ActivationStack( ActivationStack::size_type s)
{
  m_stack = new CallFrame[m_size=s];
  m_pos = 0;
}

ActivationStack::~ActivationStack()
{
  delete [] m_stack;
}

void ActivationStack::push( Registers &r)
{
  m_stack[m_pos].load( r);
  ++m_pos;
}

void ActivationStack::pop( Registers &r)
{
  m_stack[m_pos].restore( r);
  --m_pos;
}

bool ActivationStack::resize( ActivationStack::size_type newSize)
{
  CallFrame *newStack = new CallFrame[newSize];
  delete [] m_stack;
  m_stack = newStack;
  m_size = newSize;
  m_pos = 0;

  return true; //MISSING: fixed retval
}


/*
 * LispelVM ---
 */

LispelVM::LispelVM( int stacksize)
{
    m_activationStack = new CallFrame[m_stackSize=stacksize];
    m_stackPos = 0;
}

LispelVM::~LispelVM()
{
    MCAssertValidInstance();
    delete [] m_activationStack;
}

Handle_ptr LispelVM::halt()
{
  MCAssertValidInstance();
  return m_regs.ACC;
}

void LispelVM::resolveBinding( Handle_ptr var)
{
  MCAssertValidInstance();
  assert( 0 != var);
  m_regs.ACC = m_regs.CE->lookup( var->stringValue()); //FIXME:
}

void LispelVM::constant( Handle_ptr obj)
{
  MCAssertValidInstance();
  assert( 0 != obj);
  m_regs.ACC = obj;
}

void LispelVM::createClosure( Handle_ptr vars, Handle_ptr body)
{
  MCAssertValidInstance();
  assert( 0 != vars);
}

void LispelVM::ifThen( Handle_ptr thenBody, Handle_ptr elseBody)
{
  MCAssertValidInstance();
  assert( 0 != thenBody && 0 != elseBody);
  m_regs.PC = (m_regs.ACC->isNilRep())? elseBody : thenBody;
  //FIXME: needs optimization which in turn needs a Context
}

void LispelVM::localAssign( Handle_ptr var)
{
  MCAssertValidInstance();
  assert( 0 != var && 0 != m_regs.ACC);
  std::string varname = var->stringValue();
  m_regs.CE->put( varname, m_regs.ACC);
}

void LispelVM::globalAssign( Handle_ptr var)
{
    MCAssertValidInstance();
    assert( 0 != var);
    //MISSING:
}

void LispelVM::createCont()
{
  MCAssertValidInstance();
  //MISSING:
}

void LispelVM::nuate( Handle_ptr currStack, Handle_ptr var)
{
  MCAssertValidInstance();
  assert( 0 != currStack && 0 != var);
  //MISSING:
}

void LispelVM::pushFrame( Handle_ptr ret)
{
  MCAssertValidInstance();
  assert( 0 != ret);
  m_activationStack[m_stackPos].load( m_regs); //FIXME: PC->next()
  ++m_stackPos;
  std::vector<Handle_ptr> newVec;
  m_regs.VAL = newVec;
}

void LispelVM::appendArgument()
{
  MCAssertValidInstance();
  assert( 0 != m_regs.ACC);
  m_regs.VAL.push_back( m_regs.ACC);
}

void LispelVM::apply()
{
  MCAssertValidInstance();
  //MISSING:
}


Handle_ptr LispelVM::getResult()
{
  MCAssertValidInstance();
  return m_regs.ACC;
}

void LispelVM::resetRegs()
{
    MCAssertValidInstance();
    //MISSING:
}


/*
 * CompilingEvaluator ---
 */
CompilingEvaluator::CompilingEvaluator( Context &ctx) : Evaluator( ctx)
{
}

CompilingEvaluator::~CompilingEvaluator()
{
   MCAssertValidInstance();
}

Handle_ptr CompilingEvaluator::eval( Handle_ptr node)
{
  MCAssertValidInstance();
  MCAssert( 0 != node, "CompilingEvaluator::eval received null node");

  if (node->hasType( Handle::ntCONS))
    return evalExpression( node);
  else if (node->hasType( Handle::ntSYMBOL))
    return evalVariable( node);
  else
    return 0; //MISSING:
}


/*
 * IterativeEvaluator ---
 */

IterativeEvaluator::IterativeEvaluator( Context &ctx, 
					ActivationStack::size_type ssz)
  : Evaluator( ctx), m_stack( ssz)
{
}

IterativeEvaluator::~IterativeEvaluator()
{
   MCAssertValidInstance();
}

Handle_ptr IterativeEvaluator::eval( Handle_ptr n)
{
   MCAssertValidInstance();

   if (n->hasType( Handle::ntCONS))
      return (eq( n, m_ctx.NIL))? m_ctx.NIL : evalExpression( n);
   else if (n->hasType( Handle::ntSYMBOL))
      return evalVariable( n);
   else
      return n;
}

Handle_ptr IterativeEvaluator::evalExpression( Handle_ptr n)
   throw( RecoverableException)
{
   MCAssertValidInstance();
   MCAssertNotReached( 0); //FIXME:
}

Handle_ptr IterativeEvaluator::evalVariable( Handle_ptr n)
   throw( InternalInconsistency, UndefinedValue)
{
   MCAssertValidInstance();
   MCAssertNotReached( 0); //FIXME:
}
