#include <lispel/mathcmd.hh>
#include <lispel/lispel.hh>

Handle_ptr 
eqCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	   std::vector<Handle_ptr> args)
{
  // check number of arguments
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);

  if (args[0]->hasType( Handle::ntINTEGER) &&
      args[1]->hasType( Handle::ntINTEGER)) {
    return (args[0]->integerValue() == args[1]->integerValue())? ctx.TRUE : ctx.FALSE;
  } else {
    return (args[0]->doubleValue() == args[1]->doubleValue())? ctx.TRUE : ctx.FALSE;
  }
}

Handle_ptr
ltCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	   std::vector<Handle_ptr> args)
{
  // check number of arguments
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);

  if (args[0]->hasType( Handle::ntINTEGER) &&
      args[1]->hasType( Handle::ntINTEGER)) {
    return (args[0]->integerValue() < args[1]->integerValue())?
      ctx.TRUE : ctx.FALSE;

  } else {
    return (args[0]->doubleValue() < args[1]->doubleValue())?
      ctx.TRUE : ctx.FALSE;
  }
}

Handle_ptr
gtCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	   std::vector<Handle_ptr> args)
{
  // check number of arguments
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);

  if (args[0]->hasType( Handle::ntINTEGER) &&
      args[1]->hasType( Handle::ntINTEGER)) {
    return (args[0]->integerValue() > args[1]->integerValue())?
      ctx.TRUE : ctx.FALSE;
  } else {

    return (args[0]->doubleValue() > args[1]->doubleValue())?
      ctx.TRUE : ctx.FALSE;
  }
}

Handle_ptr
plusCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	     std::vector<Handle_ptr> args)
{
  if (0 == args.size()) { // return neutral element
    return ctx.factory->makeInteger( 0);

  } else if (1 == args.size()) { // return only actual parameter
    if (args[0]->isNumber())
      return args[0];

  } else { // sum up all arguments
    double result = 0.0;
    bool allInt = true;
    std::vector<Handle_ptr>::iterator pos;
    for (pos=args.begin(); pos != args.end(); ++pos) {
      result += (*pos)->doubleValue(); //doubleValue() checks type!
      if ((*pos)->hasType( Handle::ntDOUBLE))
	allInt = false;
    }
    return allInt? ctx.factory->makeInteger( static_cast<int>( result)) :
      ctx.factory->makeDouble( result);
  }
}

Handle_ptr
subtrCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	     std::vector<Handle_ptr> args)
{
  // check number of arguments
  if (0 == args.size())
    ArgumentCountException( 1, __FILE__, __LINE__);

  else if (1 == args.size()) { // return inverse argument
    if (args[0]->hasType( Handle::ntINTEGER))
      return ctx.factory->makeInteger( -args[0]->integerValue());
    else
      return ctx.factory->makeDouble( -args[0]->doubleValue());

  } else {
    double result = args[0]->doubleValue();
    bool allInt = true;
    std::vector<Handle_ptr>::size_type i;
    for (i=1; i<args.size(); ++i) {
      result -= args[i]->doubleValue(); //doubleValue() does type checking!
      if (args[i]->hasType( Handle::ntDOUBLE))
	allInt = false;
    }
    return allInt? ctx.factory->makeInteger( static_cast<int>( result)) :
      ctx.factory->makeDouble( result);
  }
}

extern Handle_ptr
mulCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	    std::vector<Handle_ptr> args)
{
  if (0 == args.size()) { // return neutral element
    return ctx.factory->makeInteger( 1);

  } else if (1 == args.size()) { // return only actual parameter
    if (args[0]->isNumber())
      return args[0];

  } else { // multiply all arguments
    bool allInt = true;
    std::vector<Handle_ptr>::iterator pos = args.begin();
    double result = (*pos)->doubleValue(); ++pos;
    for ( ; pos != args.end(); ++pos) {
      result *= (*pos)->doubleValue(); //doubleValue() checks type!
      if ((*pos)->hasType( Handle::ntDOUBLE))
	allInt = false;
    }
    return allInt? ctx.factory->makeInteger( static_cast<int>( result)) :
      ctx.factory->makeDouble( result);
  }
}

extern Handle_ptr
divCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	    std::vector<Handle_ptr> args)
{
  // check number of arguments
  if (0 == args.size()) {
    std::cerr << "error: " << owner->name() << "too few args" << std::endl;
    return ctx.NIL;

  } else if (1 == args.size()) { // return inverse argument
    if (args[0]->hasType( Handle::ntINTEGER))
      return ctx.factory->makeInteger( 1/args[0]->integerValue());
    else
      return ctx.factory->makeDouble( 1.0/-args[0]->doubleValue());

  } else {
    double result = args[0]->doubleValue();
    bool allInt = true;
    std::vector<Handle_ptr>::size_type i;
    for (i=1; i<args.size(); ++i) {
      result /= args[i]->doubleValue(); //doubleValue() does type checking!
      if (args[i]->hasType( Handle::ntDOUBLE))
	allInt = false;
    }
    return allInt? ctx.factory->makeInteger( static_cast<int>( result)) :
      ctx.factory->makeDouble( result);
  }
}

void addMathCommands( Interpreter &interp)
{
  interp.addBuiltin( "=", eqCommand);
  interp.addBuiltin( "<", ltCommand);
  interp.addBuiltin( ">", gtCommand);
  interp.addBuiltin( "+", plusCommand);
  interp.addBuiltin( "-", subtrCommand);
}
