/*
 * setcmd.cpp ---
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: setcmd.cpp,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#include <cassert>
#include <lispel/setcmd.hh>
#include <lispel/lispel.hh>

SetValue *asSet( Handle_ptr handle)
{
  assert( 0 != handle);
  if (!handle->hasType( Handle::ntSET))
    throw TypeException( "set", __FILE__, __LINE__);

  SetValue *setval = dynamic_cast<SetValue*>( handle->typeImpl());
  MCAssert( 0 != setval, "failed to access handle as set");

  return setval;
}

Handle_ptr
makesetCommand( CBuiltinAdapter *adapter, Context &ctx,
		Environment *env, std::vector<Handle_ptr> args)
{
  if (0 != args.size())
    std::cerr << "warning: " << adapter->name() << " doesn't require any "
	 << "arguments" << std::endl;

  return ctx.factory->makeSet();
}

Handle_ptr
setputCommand( CBuiltinAdapter *adapter, Context &ctx,
	       Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);

  SetValue *setval = asSet( args[0]);
  setval->put( args[1]);

  return args[1];
}

Handle_ptr
setexistsCommand( CBuiltinAdapter *adapter, Context &ctx,
		  Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);
  SetValue *setval = asSet( args[0]);
  return (setval->lookup( args[1]))? ctx.TRUE : ctx.FALSE;
}

Handle_ptr
setremoveCommand( CBuiltinAdapter *adapter, Context &ctx,
		  Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);
  SetValue *setval = asSet( args[0]);
  setval->remove( args[1]);

  return args[0];
}

Handle_ptr
setclearCommand( CBuiltinAdapter *adapter, Context &ctx,
		 Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  SetValue *setval = asSet( args[0]);
  setval->clear();

  return args[0];
}

Handle_ptr
setsizeCommand( CBuiltinAdapter *adapter, Context &ctx,
		Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  SetValue *setval = asSet( args[0]);

  return ctx.factory->makeInteger( setval->size());
}

void addSetCommands( Interpreter &interp)
{
  interp.addBuiltin( "make-set", makesetCommand);
  interp.addBuiltin( "set-put", setputCommand);
  interp.addBuiltin( "set-exists", setexistsCommand);
  interp.addBuiltin( "set-remove", setremoveCommand);
  interp.addBuiltin( "set-clear", setclearCommand);
  interp.addBuiltin( "set-size", setsizeCommand);
}
