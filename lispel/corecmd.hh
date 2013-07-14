/*
 * corecmd.hh ---
 *
 *   Declarations for the implementations of the core interpreter commands.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: corecmd.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_CORECMD_HH_
#define _lispel_CORECMD_HH_ 1

#include <lispel/nodes.hh>
#include <lispel/interpreter.hh>
#include <lispel/context.hh>

class Environment;
class Evaluator;

/**
   @name Basic Scheme Primitives

   The classes and functions in this section implement the core primitives of
   the Lispel language. All commands declared here can be added to an
   interpreter by calling the addCoreCommands function. You can, of course,
   register the implementations with your interpreter under different names
   if you don't fell the need to build a Scheme compatible interpreter
   instance.<p>

   As far as I can see all functionality implemented here is fully compatible
   to the equivalent commands defined in the R4RS. The descriptions following
   assume that the primitive have been registered using the 
   addCoreCommands function. If you use a different interpreter configuration,
   the names of the functions and special forms available may vary.
*/

//@{

class LambdaCommand : public SpecialValue {
public:
   LambdaCommand() {}
   virtual ~LambdaCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};


class QuoteCommand : public SpecialValue {
public:
   QuoteCommand() {}
   virtual ~QuoteCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class IfCommand : public SpecialValue {
public:
   IfCommand() {}
   virtual ~IfCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

//MISSING
/// Implements set! like functionality
class AssignCommand : public SpecialValue {
public:
   AssignCommand();
   virtual ~AssignCommand();
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class CondCommand : public SpecialValue {
public:
   CondCommand() {}
   virtual ~CondCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class LetCommand : public SpecialValue {
public:
   LetCommand() {}
   virtual ~LetCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class LetStarCommand : public SpecialValue {
public:
   LetStarCommand() {}
   virtual ~LetStarCommand() {}
   virtual Handle_ptr execute( Context&, Environment*, Handle_ptr);
};

//MISSING
class CaseCommand : public SpecialValue {
public:
   CaseCommand();
   virtual ~CaseCommand();
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class AndCommand : public SpecialValue {
public:
   AndCommand() {}
   virtual ~AndCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class OrCommand : public SpecialValue {
public:
   OrCommand() {}
   virtual ~OrCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

class DefineCommand : public SpecialValue {
public:
   DefineCommand() {}
   virtual ~DefineCommand() {}
   virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr expr);
};

/**
  Implements a builtin that executes a sequence of expressions from left to
  right and returns the time that has been consumed by the evaluation 
  process.
 */
class TimeCommand : public SpecialValue {
public:
   TimeCommand() {}
   virtual ~TimeCommand() {}
   virtual Handle_ptr execute( Context&, Environment*, Handle_ptr);
};

class DlLoadCommand : public BuiltinValue {
public:
  DlLoadCommand();
  virtual ~DlLoadCommand();

  virtual Handle_ptr execute( Context &c, Environment *e, std::vector<Handle_ptr> a);
};

/**
   Implements the R4RS 'begin' function which provides clean code blocks.
   It is assumed clean because begin behaves like a normal function as it
   returns the result of the last expression computed. This behaviour is
   equivalent to performing the expressions within nested lambda expressions.
*/
extern Handle_ptr
beginCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/// Implements the R4RS 'display' function.
extern Handle_ptr
displayCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/// Implements the R4RS 'car' function.
extern Handle_ptr
carCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/// Implements the R4RS 'cdr' function
extern Handle_ptr
cdrCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
consCommand( CBuiltinAdapter*, Context &, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
notCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
listCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/**
   Returns Lispel's full version number as a string.
 */
extern Handle_ptr
versionCommand( CBuiltinAdapter*, Context &, Environment*, std::vector<Handle_ptr>);

/**
   Load and execute (evaluate) a Lispel source file.
  */
extern Handle_ptr
loadCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/**
   Exits from the read-eval-print loop.
 */
extern Handle_ptr
exitCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/**
  The 'memstat' command returns the current heap size and the number of
  unused memory cells as a two element consed list. The command doesn't
  require any arguments.
 */
extern Handle_ptr
memstatCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/**
  Explicitly request full garbage collection. The function will return 
  immediately but request will be performed when the next safe point has been
  reached.  'gc' always returns true because of the delayed execution. 
 */
extern Handle_ptr
gcCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

/**
   This class allows the creation of a Lispel type predicate on the fly.
   Just pass the type you want to check as an argument to the constructor and
   register that object with your interpreter.
   @version 0.1
*/
class TypePredicate : public BuiltinValue {
public:
  TypePredicate( Handle::NodeType t);
  virtual ~TypePredicate();
  virtual Handle_ptr execute( Context &ctx, Environment *env,
			      std::vector<Handle_ptr> args);
protected:
  Handle::NodeType m_type;
};

  
void addCoreCommands( Interpreter &interp);

//@}

#endif /*_lispel_CORECMD_HH_*/

