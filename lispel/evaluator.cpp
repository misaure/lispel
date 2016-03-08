#include <cassert>
#include <lispel/defs.hh>
#include <lispel/exception.hh>
#include <lispel/evaluator.hh>
#include <lispel/nodes.hh>
#include <lispel/utilities.hh>
#include <lispel/reader.hh> //FIXME: included for printList prototype

Evaluator::Evaluator( Context &ctx) : m_ctx( ctx)
{
  assert( 0 != ctx.factory);
  assert( 0 != ctx.toplevel);
  m_envStack.push( ctx.toplevel);
}

Evaluator::~Evaluator()
{
}

Environment *Evaluator::pushEnvironment()
{
  MCAssertValidInstance();
  pushEnvironment( m_envStack.top()->makeChildEnvironment());
  assert( 0 != m_envStack.top());

  return m_envStack.top();
}

void Evaluator::pushEnvironment( Environment *env)
{
  MCAssertValidInstance();
  assert( 0 != m_envStack.size() && 0 != m_envStack.top() && 0 != env);
  m_envStack.push( env);
}

void Evaluator::popEnvironment()
{
  MCAssertValidInstance();
  assert( 0 != m_envStack.size());
  m_envStack.pop();
}

Environment *Evaluator::currentEnvironment()
{
  MCAssertValidInstance();
  assert( 0 != m_envStack.size() && 0 != m_envStack.top());
  return m_envStack.top();
}


SimpleEvaluator::SimpleEvaluator( Context &ctx)
  : Evaluator( ctx)
{
}

SimpleEvaluator::~SimpleEvaluator()
{
  MCAssertValidInstance();
}

Handle_ptr SimpleEvaluator::eval( Handle_ptr node)
{
  MCAssertValidInstance();
  assert( 0 != node);

#if defined( DEBUG) && DEBUG > 2
  std::cerr << "SimpleEvaluator::eval( " << node << ")" << std::endl;
#endif

  if (node->hasType( Handle::ntCONS))
    //FIXME: why handle application of NIL?
    return (eq( node, m_ctx.NIL))? m_ctx.NIL : evalExpression( node);
  else if (node->hasType( Handle::ntSYMBOL))
    return evalVariable( node);     // lookup value bound to symbol
  else
    return node;                    // self-evaluating nodes
}

Handle_ptr
SimpleEvaluator::evalExpression( Handle_ptr node)
   throw( RecoverableException)
{
  MCAssertValidInstance();
  assert( 0 != node);

#if defined( DEBUG) && DEBUG > 2
  std::cerr << "SimpleEvaluator::evalExpression: ";
  printList( node, std::cerr);
  std::cerr << std::endl;
#endif

  // The car cell of the first node either contains a lambda
  // expression or a symbol which refers to some executable node type.
  // In both cases the first node needs to be evaluated before application.
  Handle_ptr expr = eval( node->car());
  if (0 == expr)
    throw "SimpleEvaluator::evalExpression: eval returned (null)";

  if (expr->checkFlag( Handle::SPECIALFLAG)) {
    // Handle specials (don't evaluate arguments before call)

    if (expr->hasType( Handle::ntCFUNC)) {
      // --- apply builtin special form
      SpecialValue *cmd = dynamic_cast<SpecialValue*>( expr->typeImpl());
      assert( 0 != cmd);
      return cmd->execute( m_ctx, m_envStack.top(), node);
    }

  } else {
    // Handle standard s-expression: evaluate arguments before calling

    // step 1: collect arguments
    std::vector<Handle_ptr> args;
    Handle_ptr arg;
    if (eq( node->cdr(), m_ctx.NIL))
      std::cerr << "nil arglist" << std::endl;
    for (arg = node->cdr(); arg != m_ctx.NIL; arg = arg->cdr()) {
#if defined( DEBUG) && DEBUG > 3
      std::cerr << "eval arg: " << *(node->car()) << std::endl;
#endif /*DEBUG*/
      args.push_back( eval( arg->car()));
    }
#if defined( DEBUG) && DEBUG > 2
    std::cerr << "collected " << args.size() << " arguments" << std::endl;
#endif

    // step 2: execute function with the arguments collected before
    if (expr->hasType( Handle::ntCFUNC)) {
      // --- apply builtin
      BuiltinValue *cmd = dynamic_cast<BuiltinValue*>( expr->typeImpl());
      assert( 0 != cmd);
      return cmd->execute( m_ctx, m_envStack.top(), args);

    } else if (expr->hasType( Handle::ntCLOSURE)) {
      // --- apply closure
      // a: push new binding environment and add arguments to it
      pushEnvironment( expr->bindArguments( args));
#if defined( DEBUG) && DEBUG > 2
      std::cerr << "closure body: ";
      printList( expr->body(), std::cerr);
      std::cerr << std::endl;
#endif
      // b: call eval on the closure's body
      Handle_ptr retval = 0;
      Handle_ptr pos;
      for (pos=expr->body(); !eq( pos, m_ctx.NIL); pos=pos->cdr())
         retval = eval( pos->car());
      popEnvironment();
      MCAssert( 0 != retval, "invalid lambda application");
      return retval;

    } else if (expr->hasType( Handle::ntOBJECT)) {
      // --- message sending
      //MISSING: object implementation, need to create a good design first
      return m_ctx.NIL;

    } else
      throw RecoverableException( "invalid expression type", __FILE__, __LINE__);
  }

  MCAssertNotReached( 0);

  return 0;
}

Handle_ptr
SimpleEvaluator::evalVariable( Handle_ptr node)
   throw( InternalInconsistency, UndefinedValue)
{
  MCAssertValidInstance();
  assert( 0 != node);

#if defined( DEBUG) && DEBUG > 2
  std::cerr << "SimpleEvaluator::evalVariable( \"" << node->stringValue()
       << "\")" << std::endl;
#endif

  //FIXME: better error handling!
  if (0 == m_envStack.size() || 0 == m_envStack.top()) {
    throw InternalInconsistency( "eval stack corrupted", __FILE__, __LINE__);
  }

  Handle_ptr valueNode = m_envStack.top()->lookup( node->stringValue());

  if (0 == valueNode) {
    throw UndefinedValue( node->stringValue(), __FILE__, __LINE__);
  }

  return valueNode;
}
