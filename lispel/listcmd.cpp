#include <lispel/listcmd.hh>
#include <lispel/lispel.hh>

Handle_ptr
lengthCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	       std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  if (isList( args[0]))
    return ctx.factory->makeInteger( listlength( args[0]));
  
  throw TypeException("expected list");
}

Handle_ptr
listCommand( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	     std::vector<Handle_ptr> args)
{
  return makeList( args.begin(), args.end(), ctx);
}

Handle_ptr
nullPredicate( CBuiltinAdapter *owner, Context &ctx, Environment *env,
	       std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  return (eq( ctx.NIL, args[0]))? ctx.TRUE : ctx.FALSE;
}

Handle_ptr
setCdrFCommand( CBuiltinAdapter *adapter, Context &ctx,
	        Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
     throw ArgumentCountException( 2, __FILE__, __LINE__);
  if (args[0]->hasType( Handle::ntCONS) && args[1]->hasType( Handle::ntCONS)) {
     args[0]->setCdr( args[1]);
     return args[0];
  }
  throw TypeException( "pair", __FILE__, __LINE__);
}

Handle_ptr
setCarFCommand( CBuiltinAdapter *adapter, Context &ctx,
                Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
     throw ArgumentCountException( 2, __FILE__, __LINE__);
  args[0]->setCar( args[1]);

  return args[0];
}

void addListCommands( Interpreter &interp)
{
  interp.addBuiltin( "list",  listCommand);
  interp.addBuiltin( "length",  lengthCommand);
  interp.addBuiltin( "null?",  nullPredicate);
  interp.addBuiltin( "set-car!", setCarFCommand);
  interp.addBuiltin( "set-cdr!", setCdrFCommand);
  
  // TODO (list? x)
  // TODO (pair? x)
}

