/*
 * hashcmd.cpp ---
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: hashcmd.cpp,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#include <cassert>
#include <lispel/hashcmd.hh>
#include <lispel/lispel.hh>

Hashtable *asHashtable( Handle_ptr handle)
{
   assert( 0 != handle);
   if (!handle->hasType( Handle::ntHASHTABLE)) {
     throw TypeException( "hashtable", __FILE__, __LINE__);
   }

   Hashtable *hashtable = dynamic_cast<Hashtable*>( handle->typeImpl());
   MCAssert( 0 != hashtable, "failed to access handle as hashtable");

   return hashtable;
}

Handle_ptr makehashCommand( CBuiltinAdapter *adapter, Context &ctx,
		 Environment *env, std::vector<Handle_ptr> args)
{
  if (0 != args.size()) {
    std::cerr << "warning: " << adapter->name() << " doesn't require any "
	 << "arguments" << std::endl;
  }

  return ctx.factory->makeHashtable();
}

Handle_ptr hashsetCommand( CBuiltinAdapter *adapter, Context &ctx,
		Environment *env, std::vector<Handle_ptr> args)
{
  if (3 != args.size()) {
    throw ArgumentCountException( 3, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  std::string key = args[1]->stringValue();
  hashtable->put( key, args[2]);

  return args[1];
}

Handle_ptr hashgetCommand( CBuiltinAdapter *adapter, Context &ctx,
		Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size()) {
    throw ArgumentCountException( 2, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  assert( 0 != hashtable);
  std::string key = args[1]->stringValue();

  return hashtable->lookup( key);
}

Handle_ptr hashsizeCommand( CBuiltinAdapter *adapter, Context &ctx,
		 Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size()) {
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  assert( 0 != hashtable);

  return ctx.factory->makeInteger( hashtable->size());
}

Handle_ptr hashclearCommand( CBuiltinAdapter *adapter, Context &ctx,
		  Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size()) {
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  }

  asHashtable( args[0])->clear();

  return args[0];
}

Handle_ptr hashremoveCommand( CBuiltinAdapter *adapter, Context &ctx,
		   Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size()) {
     throw ArgumentCountException( 2, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  std::string key = args[1]->stringValue();
  hashtable->remove( key);

  return args[0];
}

Handle_ptr hashexistsCommand( CBuiltinAdapter *adapter, Context &ctx,
		   Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size()) {
     throw ArgumentCountException( 2, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  std::string key = args[1]->stringValue();
  return (hashtable->exists( key))? ctx.TRUE : ctx.FALSE;
}


Handle_ptr hashkeysCommand( CBuiltinAdapter *adapter, Context &ctx,
                 Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  Hashtable *hashtable = asHashtable( args[0]);
  Hashtable::iterator pos = hashtable->begin();
  if (pos != hashtable->end()) {
    Handle_ptr listHead, current;
    listHead = current = ctx.factory->makeCons( ctx.factory->makeString( (*pos).first.c_str()));
    ++pos;
    for ( ; pos!=hashtable->end(); ++pos) {
      current->setCdr( ctx.factory->makeCons( ctx.factory->makeString( (*pos).first.c_str())));
      current = current->cdr();
    }
    return listHead;
  } else
    return ctx.NIL;
}

Handle_ptr hashvalsCommand( CBuiltinAdapter *adapter, Context &ctx,
                 Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size()) {
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  }

  Hashtable *hashtable = asHashtable( args[0]);
  Hashtable::iterator pos = hashtable->begin();
  if (pos != hashtable->end()) {
    Handle_ptr listHead, current;
    listHead = current = ctx.factory->makeCons( (*pos).second);
    ++pos;
    for ( ; pos!=hashtable->end(); ++pos) {
      current->setCdr( ctx.factory->makeCons( (*pos).second));
      current = current->cdr();
    }
    return listHead;
  } else
    return ctx.NIL;
}


void addHashCommands( Interpreter &interp)
{
  interp.addBuiltin( "make-hash", makehashCommand);
  interp.addBuiltin( "hash-put", hashsetCommand);
  interp.addBuiltin( "hash-get", hashgetCommand);
  interp.addBuiltin( "hash-clear", hashclearCommand);
  interp.addBuiltin( "hash-size", hashsizeCommand);
  interp.addBuiltin( "hash-remove", hashremoveCommand);
  interp.addBuiltin( "hash-exists", hashexistsCommand);
  interp.addBuiltin( "hash-keys", hashkeysCommand);
  interp.addBuiltin( "hash-values", hashvalsCommand);
}
