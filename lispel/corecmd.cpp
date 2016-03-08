/*
 * corecmd.cpp ---
 *
 *   Provides the most basic commands and special forms.
 *
 * (c) 2000-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: corecmd.cpp,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#include <list>
#include <ctime>
#include <cassert>
#include <lispel/corecmd.hh>
#include <lispel/lispel.hh>
#include "version.h"

/*
 * SPECIAL FORMS
 */

Handle_ptr
LambdaCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  MCAssertValidInstance();
  assert( 0 != env && 0 != ctx.eval && 0 != expr);

  Handle_ptr args = expr->cdr();
  if (!isList( args->car())) {
    throw TypeException( "list", __FILE__, __LINE__);
  }

  // collect argument names and check if argument specification is valid
  std::list<Handle_ptr> argumentList;
  if (!eq( args->car(), ctx.NIL)) {
     std::back_insert_iterator<std::list<Handle_ptr> > iiter( argumentList);
     copyList( args->car(), iiter);
     std::list<Handle_ptr>::iterator pos;
     for (pos=argumentList.begin(); pos!=argumentList.end(); ++pos) {
       if (!(*pos)->hasType( Handle::ntSYMBOL)) {
         throw TypeException( "symbol", __FILE__, __LINE__);
       }
     }
  } else {
     // empty argument list
     std::cerr << "lambda: empty argument list" << std::endl;
  }

#if defined( DEBUG) && DEBUG > 3
  std::cerr << "create closure with " << argumentList.size() << " arguments."
       << std::endl;
  std::cerr << "  body: ";
  printList( args->cdr(), std::cerr); std::cerr << std::endl;
  std::cerr << "  args: ";
  std::list<Handle_ptr>::iterator pos;
  for (pos=argumentList.begin(); pos!=argumentList.end(); ++pos)
    std::cerr << *(*pos) << ' ';
  std::cerr << std::endl;
#endif

  return ctx.factory->makeClosure( argumentList, env, args->cdr());
}


Handle_ptr
CondCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
   Handle_ptr args = expr->cdr();

   Handle_ptr branch;   // iterate over all clauses/branches
   for (branch=args; !eq( branch, ctx.NIL); branch=branch->cdr()) {
      Handle_ptr exprList = branch->car();
      Handle_ptr evaluated = exprList->car();

      if (!eq( ctx.eval->eval( evaluated), ctx.FALSE)) {
        // expression evaluates to true, so evaluate remaining expressions in
	// the current branch and return result of last evaluation
	for ( exprList=exprList->cdr(); !eq( exprList, ctx.NIL); exprList=exprList->cdr()) {
	    evaluated = ctx.eval->eval( exprList->car());
        }
	return evaluated;
      }
   }
   return ctx.NIL; //this is left unspecified by R[45]RS
}


Handle_ptr
QuoteCommand::execute( Context &ctx, Environment *env, Handle_ptr args)
{
  MCAssertValidInstance();
  assert( 0 != args);

  if (eq( ctx.NIL, args)) {
    throw TypeException( "non-NIL", __FILE__, __LINE__);
  }
  // MISSING: check number of arguments (list_length)
  if (eq( args->cdr(), ctx.NIL))
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  return args->cdr()->car();
}

Handle_ptr
DefineCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  MCAssertValidInstance();

  if (3 != listlength( expr))
    throw ArgumentCountException( 3, __FILE__, __LINE__);

  Handle_ptr args = expr->cdr();
  if (args->car()->hasType( Handle::ntSYMBOL)) {
    // Variablenbindung
    ctx.toplevel->put( args->car()->stringValue(),
		       ctx.eval->eval( args->cdr()->car()));
    return args->car();

  } else {
    if (args->car()->hasType( Handle::ntCONS) && !args->car()->isNilRep()) {
      // Funktionsdefinition
      std::cerr << "error: function definitions aren't supported right now" << std::endl;
      return ctx.NIL;

    } else
      throw TypeException( "symbol or list", __FILE__, __LINE__);
  }

  MCAssertNotReached( 0);

  return 0;
}



Handle_ptr
LetCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  MCAssertValidInstance();
  MCAssert( 0 != expr && !eq( expr, ctx.NIL), "illegal expression");

  if (!eq( expr->cdr(), ctx.NIL) && !eq( expr->cdr()->cdr(), ctx.NIL)) {
     // create new binding environment
     Environment *newEnv = env->makeChildEnvironment();
     Handle_ptr bindList = expr->cdr()->car();

#if defined( DEBUG) && DEBUG > 2
     std::cerr << "let bindings == ";
     printList( bindList, std::cerr);
     std::cerr << std::endl;
#endif

     for ( ; !eq( bindList, ctx.NIL); bindList = bindList->cdr()) {
        Handle_ptr bindings = bindList->car();
#if defined( DEBUG) && DEBUG > 3
        std::cerr << " evaluate binding ";
	printList( bindings, std::cerr);
	std::cerr << std::endl;
#endif
	std::string key = bindings->car()->stringValue();
	Handle_ptr value = ctx.eval->eval( bindings->cdr()->car());
	newEnv->put( key, value);
     }

     // evaluate expressions in new environment
     ctx.eval->pushEnvironment( newEnv);
     Handle_ptr pos, evaluated = 0;

#if defined( DEBUG) && DEBUG > 2
     std::cerr << "let expressions == ";
     printList( expr->cdr()->cdr(), std::cerr);
     std::cerr << std::endl;
#endif

     for (pos=expr->cdr()->cdr(); !eq( pos, ctx.NIL); pos=pos->cdr())
        evaluated = ctx.eval->eval( pos->car());
     ctx.eval->popEnvironment();

     MCAssert( 0 != evaluated, "let failed to evaluate body");
     return evaluated;

  } else {
     throw ArgumentCountException( -2, __FILE__, __LINE__);
  }

  MCAssertNotReached( 0);

  return 0;
}


Handle_ptr
LetStarCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  MCAssertValidInstance();
  MCAssert( 0 != expr && !eq( expr, ctx.NIL), "illegal expression");

  if (!eq( expr->cdr(), ctx.NIL) && !eq( expr->cdr()->cdr(), ctx.NIL)) {
     // create new binding environment which will be used as current env
     ctx.eval->pushEnvironment();
     Handle_ptr bindList = expr->cdr()->car();

#if defined( DEBUG) && DEBUG > 2
     std::cerr << "let bindings == ";
     printList( bindList, std::cerr);
     std::cerr << std::endl;
#endif

     for ( ; !eq( bindList, ctx.NIL); bindList = bindList->cdr()) {
        Handle_ptr bindings = bindList->car();
#if defined( DEBUG) && DEBUG > 3
        std::cerr << " evaluate binding ";
	printList( bindings, std::cerr);
	std::cerr << std::endl;
#endif
	std::string key = bindings->car()->stringValue();
	Handle_ptr value = ctx.eval->eval( bindings->cdr()->car());
	ctx.eval->currentEnvironment()->put( key, value);
     }

     // evaluate expressions in new environment
     Handle_ptr pos, evaluated = 0;

#if defined( DEBUG) && DEBUG > 2
     std::cerr << "let expressions == ";
     printList( expr->cdr()->cdr(), std::cerr);
     std::cerr << std::endl;
#endif

     for (pos=expr->cdr()->cdr(); !eq( pos, ctx.NIL); pos=pos->cdr())
        evaluated = ctx.eval->eval( pos->car());
     ctx.eval->popEnvironment();

     MCAssert( 0 != evaluated, "let failed to evaluate body");
     return evaluated;

  } else {
     throw ArgumentCountException( -2, __FILE__, __LINE__);
  }

  MCAssertNotReached( 0);

  return 0;
}


Handle_ptr
IfCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  Handle_ptr args = expr->cdr();
  if (3 != listlength( args))
    throw ArgumentCountException( 3, __FILE__, __LINE__);

  if (ctx.eval->eval( args->car()) != ctx.FALSE)
    return ctx.eval->eval( args->cdr()->car());
  else
    return ctx.eval->eval( args->cdr()->cdr()->car());
}


Handle_ptr
AndCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  Handle_ptr args = expr->cdr();
  int argc = listlength( args);
  if (0 == argc)
    //FIXME: needs *at least* one argument
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  int i;
  Handle_ptr evaluated = 0;
  for (i=0; i<argc; ++i, args=args->cdr()) {
    evaluated = ctx.eval->eval( args->car());
    if (eq( evaluated, ctx.FALSE))
      return ctx.FALSE;
  }
  assert( 0 != evaluated);
  return evaluated;
}

Handle_ptr
OrCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  Handle_ptr args = expr->cdr();
  int argc = listlength( args);
  if (0 == argc)
    //FIXME: needs *at least* one argument
    throw ArgumentCountException( 1, __FILE__, __LINE__);

  int i;
  Handle_ptr evaluated = 0;
  for (i=0; i<argc; ++i, args=args->cdr()) {
    evaluated = ctx.eval->eval( args->car());
    if (!eq( evaluated, ctx.FALSE)) {
      assert( 0 != evaluated);
      return evaluated;
    }
  }

  return ctx.FALSE;
}

Handle_ptr
TimeCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
   clock_t stopwatch = clock();
   Handle_ptr pos;
   for (pos=expr->cdr(); !eq( pos, ctx.NIL); pos=pos->cdr())
      ctx.eval->eval( pos->car());
   stopwatch = clock()-stopwatch;

   return ctx.factory->makeDouble( static_cast<double>( stopwatch) /
   				   static_cast<double>( CLOCKS_PER_SEC));
}


/*
 * FUNCTIONS
 */
Handle_ptr displayCommand( CBuiltinAdapter *adapter, Context &ctx,
			   Environment *env, std::vector<Handle_ptr> args)
{
  std::vector<Handle_ptr>::iterator pos;
  for (pos = args.begin(); pos != args.end(); ++pos) {
     std::cout << *(*pos) << std::endl;
  }

  return ctx.TRUE; //FIXME: what should be returned to be standards compliant?
}


Handle_ptr carCommand( CBuiltinAdapter *adapter, Context &ctx,
		       Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  return args[0]->car();
}

Handle_ptr cdrCommand( CBuiltinAdapter *adapter, Context &ctx,
		       Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  return args[0]->cdr();
}

Handle_ptr memstatCommand( CBuiltinAdapter *adapter, Context &ctx,
			   Environment *env, std::vector<Handle_ptr> args)
{
   if (args.size() > 0) {
     std::cerr << "warning: " << adapter->name()
	  << " doesn't require any arguments." << std::endl;
   }
   Handle_ptr freeCount = ctx.factory->makeCons( ctx.factory->makeInteger( ctx.factory->freeCount()));
   return ctx.factory->makeCons( ctx.factory->makeInteger( ctx.factory->size()), freeCount);
}


Handle_ptr gcCommand( CBuiltinAdapter *adapter, Context &ctx,
		      Environment *env, std::vector<Handle_ptr> args)
{
  //FIXME:
  if (args.size() > 0) {
    std::cerr << "warning: " << adapter->name()
	 << " doesn't require any arguments." << std::endl;
  }
  ctx.interp->gc();
  return ctx.TRUE;
}


Handle_ptr beginCommand( CBuiltinAdapter *adapter, Context &ctx,
			 Environment *env, std::vector<Handle_ptr> args)
{
  if (0 == args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  return args[args.size()-1];
}


Handle_ptr consCommand( CBuiltinAdapter *adapter, Context &ctx,
			Environment *env, std::vector<Handle_ptr> args)
{
  if (2 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);
  return ctx.factory->makeCons( args[0], args[1]);
}

TypePredicate::TypePredicate( Handle::NodeType t)
{
  m_type = t;
}

TypePredicate::~TypePredicate()
{
}

Handle_ptr
TypePredicate::execute( Context &ctx, Environment *env, std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 2, __FILE__, __LINE__);
  return (args[0]->hasType( m_type))? ctx.TRUE : ctx.FALSE;
}

Handle_ptr
notCommand( CBuiltinAdapter *adapter, Context &ctx, Environment *env,
	    std::vector<Handle_ptr> args)
{
  if (1 != args.size())
    throw ArgumentCountException( 1, __FILE__, __LINE__);
  if (eq( args[0], ctx.FALSE))
    return ctx.TRUE;
  return ctx.FALSE;
}

Handle_ptr
versionCommand( CBuiltinAdapter *adapter, Context &ctx, Environment *env,
	        std::vector<Handle_ptr> args)
{
   if (0 != args.size()) {
      std::cerr << "warning: ignoring argument(s)" << std::endl;
   }
   return ctx.factory->makeString( LISPEL_VERSION);
}

Handle_ptr
exitCommand( CBuiltinAdapter *adapter, Context &ctx, Environment *env,
	     std::vector<Handle_ptr> args)
{
   if (0 != args.size()) {
      std::cerr << "warning: " << adapter->name()
	   << " doesn't require any arguments" << std::endl;
   }
   ctx.interp->exit();
   return ctx.TRUE;
}

Handle_ptr
loadCommand( CBuiltinAdapter *adapter, Context &ctx, Environment *env,
	     std::vector<Handle_ptr> args)
{
   if (1 != args.size())
      throw ArgumentCountException( 1, __FILE__, __LINE__);
   std::string filename = args[0]->stringValue();
   return (ctx.interp->source( filename))? ctx.TRUE : ctx.FALSE;
}


void addCoreCommands( Interpreter &interp)
{
  // special forms
  interp.addSpecial( "and", new AndCommand());
  interp.addSpecial( "cond", new CondCommand());
  interp.addSpecial( "define", new DefineCommand());
  interp.addSpecial( "if", new IfCommand());
  interp.addSpecial( "lambda", new LambdaCommand());
  interp.addSpecial( "let", new LetCommand());
  interp.addSpecial( "let*", new LetStarCommand());
  interp.addSpecial( "or", new OrCommand());
  interp.addSpecial( "quote", new QuoteCommand());
  interp.addSpecial( "time", new TimeCommand());

  // some basic functions
  interp.addBuiltin( "begin", beginCommand);
  interp.addBuiltin( "car", carCommand);
  interp.addBuiltin( "cdr", cdrCommand);
  interp.addBuiltin( "cons", consCommand);
  interp.addBuiltin( "display", displayCommand);
  interp.addBuiltin( "eq?", new BinaryPredicateAdapter( eq));
  interp.addBuiltin( "eqv?", new BinaryPredicateAdapter( eqv));
  interp.addBuiltin( "exit", exitCommand);
  interp.addBuiltin( "gc", gcCommand);
  interp.addBuiltin( "load", loadCommand);
  interp.addBuiltin( "memstat", memstatCommand);
  interp.addBuiltin( "not", notCommand);
  interp.addBuiltin( "version",  versionCommand);

  // type predicates
  interp.addBuiltin( "boolean?", new TypePredicate( Handle::ntBOOLEAN));
  interp.addBuiltin( "char?", new TypePredicate( Handle::ntCHARACTER));
  interp.addBuiltin( "double?", new TypePredicate( Handle::ntDOUBLE));
  interp.addBuiltin( "integer?", new TypePredicate( Handle::ntINTEGER));
  interp.addBuiltin( "pair?", new TypePredicate( Handle::ntCONS));
  interp.addBuiltin( "string?", new TypePredicate( Handle::ntSTRING));
  interp.addBuiltin( "symbol?", new TypePredicate( Handle::ntSYMBOL));
  interp.addBuiltin( "hashtable?", new TypePredicate( Handle::ntHASHTABLE));
  interp.addBuiltin( "keyword?", new TypePredicate( Handle::ntKEYWORD));
  interp.addBuiltin( "set?", new TypePredicate( Handle::ntSET));
}

