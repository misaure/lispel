	/*
 * interpreter.hh ---
 *
 *   The interpreter class acts as a facade to the various components making
 *   up a complete interpreter. The API implemented by this class is what
 *   the user will normally have to deal with.
 *
 * (c) 1999 by Michael Saure
 * RCSID: $Id: interpreter.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_INTERPRETER_HH_
#define _lispel_INTERPRETER_HH_

#include <string>
#include <iostream>
#include <lispel/nodefactory.hh>
#include <lispel/nodes.hh>
#include <lispel/evaluator.hh>
#include <lispel/gc.hh>


class Environment;
class UserInterface;


/**
   This class acts as facade for the various components making up a complete
   interpreter. All commonly used methods of the various components are 
   exported as methods of the Interpreter class, so this class acts as the
   interpreters high-level API. To get deeper inside the interpreter's 
   internal, the components actually making up the interpreter can be accessed
   directly, thus giving access to the low-level APIs.<p>

   All of the interpreter's components are exchangeable with alternative
   implementations. To determine what component types will actually be used,
   you can pass a ComponentFactory to the interpreter's constructor. All
   components used by the interpreter will be created using that factory
   object. If you don't specify any factory object, an instance of the
   DefaultComponentFactory will be used.
   @version 0.1
*/
class Interpreter {
public:

  /**
     This class defines the interface to factory objects which are used by 
     the Interpreter to create it's subcomponents. The ComponentFactory can
     be viewed as a kind of meta factory object, as most object created by
     this factory are factory objects by themselves.
     @version 0.2
  */
  class ComponentFactory {
  public:
    /// Creates a new global binding environment.
    virtual Environment *createToplevelEnvironment() =0;

    /// Returns a shared instance of a node factory object (singleton pattern).
    virtual NodeFactory *getGlobalNodeFactory( Context &ctx,
					       NodeFactory::size_type) =0;

    virtual Evaluator   *createEvaluator( Context &ctx) =0;
    virtual GarbageCollector *createGC( Context &ctx) =0;
  };
  
  /**
     This is the ComponentFactory used by default when no other factory is
     explicitly specified upon creation of a new interpreter instance.
     @version 0.1
  */
  class DefaultComponentFactory : public ComponentFactory {
  public:
    DefaultComponentFactory();
    virtual ~DefaultComponentFactory();

    virtual Environment *createToplevelEnvironment();
    virtual NodeFactory *getGlobalNodeFactory( Context &ctx,
					       NodeFactory::size_type initsz);
    virtual Evaluator   *createEvaluator( Context &ctx);
    virtual GarbageCollector *createGC( Context &ctx);

  protected:
    static NodeFactory *GLOBAL_NODE_FACTORY;
  };

  Interpreter();
  Interpreter( ComponentFactory *factory);
  ~Interpreter();

  /**
    repl is an acronym for 'Read-Eval-Print loop'. This method reads and
    executes a Lispel program read from the given stream until either an
    end-of-file occurs or the program is explicitly halted.
    @param stream The stream from which the program should be read.
    @return false if an error occured, true else.
    */
  bool repl( std::istream *stream, UserInterface *ui);

  /**
    Load and execute a Lispel source file. If any exception occurs during 
    program execution it won't be caught by this method so that the user
    code can more exactly determine the type of error.
    @return true when loading and executing the program succeeded or false if
    opening the input file failed.
   */
  bool source( const std::string &filename);

  /**
     Establish a variable binding in the global binding environment.
     @param key Name under which the value will be available
     @param value Can be any kind of valid memory cell.
  */
  void toplevelDefine( const std::string &key, Handle_ptr value);

  /**
     Look up a value in the global binding environment.
     @param key Name of the value to look up.
     @return A handle to the value stored with key or zero if no such value
     exists.
  */
  Handle_ptr toplevelLookup( const std::string &key);

  /**
     Add a new builtin to the interpreter. All builtins are defined in the
     toplevel binding environment and, in contrast to closures, don't keep
     an environment of their own as part of their data structure.
     @param name Specifies the name for the new builtin. Any data already
     existing under the same name will be overwritten. So you might want
     to do a toplevelLookup first, before you add your new command.
     @param cmdimpl A subclass of BuiltinValue implementing the new command.
     @return A pointer to the memory cell allocated for the new command or
     0 if the operation failed.
    */
  Handle_ptr addBuiltin( const std::string &name, BuiltinValue *cmdimpl);
  Handle_ptr addBuiltin( const std::string &name, CBuiltinAdapter::cimpl);

  /**
     This method is in operation very similar to addBuiltin except that it
     adds a special form, i.e. a builtin which receives unevaluated
     arguments.
   */
  Handle_ptr addSpecial( const std::string &name, SpecialValue *cmdimpl);

  /**
    Returns a globally unique instance of the class Interpreter. This 
    instance will be created using default interpreter components.
    */
  static Interpreter *instance();

  /// Perform garbage collection, if available.
  void gc();

  /// Get the number of memory cells currently in use.
  unsigned int cellCount();
  
  void exit() { m_exitRequested = true; }

  Environment *toplevel() { return m_ctx.toplevel; }
  NodeFactory *nodeFactory() { return m_ctx.factory; }
  Evaluator   *evaluator() { return m_ctx.eval; }
  const Context &context() const { return m_ctx; }
  Context &context() { return m_ctx; }

protected:
  Context m_ctx;
  static bool M_lexer_initialized;
  bool m_exitRequested;
  bool m_gcRequested;
  GarbageCollector *m_gc;
  static Interpreter *M_globalInstance;
};

#endif /*_lispel_INTERPRETER_HH_*/

