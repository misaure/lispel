/*
 * lispelvm.hh ---
 *
 *   Bytecode interpreting virtual machine for Lispel.
 *
 * (c) 1999-2016 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: lispelvm.hh,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#ifndef lispel_LISPELVM_HH_
#define lispel_LISPELVM_HH_ 1

#include <lispel/nodes.hh>
#include <lispel/exception.hh>
#include <lispel/exception.hh>
#include <lispel/environment.hh>
#include <lispel/evaluator.hh>
#include <lispel/interpreter.hh>

/**
   @name Lispel Virtual Machine
   The classes in this section can be used to easily build up an Interpreter
   instance which operates on an intermediate byte code representation of
   Scheme expressions.
   MISSING: Threads, performance opts for stack handling
*/
//@{


/**
   This is the set of registers the Lispel Virtual Machine operates on.
   @version 0.1
*/
struct Registers {

  /**
     ACC is the accumulator register which is used to store the result of the
     eval operation performed last.
  */
  Handle_ptr ACC;

  /**
     PC is similar to the program counter found in normal microprocessors.
     Here, it more generally contains the rest of the expressions to be
     executed.
  */
  Handle_ptr PC;

  /**
     CE contains a pointer to the current environment.
  */
  Environment *CE;

  /**
     When function calls are being assembled this register is used to store
     the arguments evaluated so far.
  */
  std::vector<Handle_ptr> VAL;
  int stackBase;
  int stackOffset;
}; //Registers


/**
   A single thread of execution with its own associated state (register set and
   argument stack).
   @version 0.0
*/
struct Thread {
  Registers regs;
  Handle_ptr *argStack;
  int maxStackSize;
  int stackPos;
}; //Thread

/**
   This is the basic type for the activation (or control) stack.
   @version 0.0
*/
struct CallFrame {
  /**
     This constructor simply initializes all member to 0.
  */
  CallFrame();

  /**
     Initialize a CallFrame from a set of registers.
  */
  CallFrame( Registers &regs);
  ~CallFrame();

  void load( Registers &regs) {
    PC = regs.PC; VAL = regs.VAL; CE = regs.CE;
  }

  void restore( Registers &regs) {
    regs.PC = PC; regs.VAL = VAL; regs.CE = CE;
  }

  Handle_ptr PC;
  std::vector<Handle_ptr> VAL;
  Environment *CE;
}; //CallFrame


/**
  A stack for saving activation records upon applications.
  */
class ActivationStack {
public:
  typedef unsigned int size_type;

  ActivationStack( size_type);
  ~ActivationStack();

  void push( Registers &r);
  void pop( Registers &r);

  bool resize( size_type s);
  size_type size() { return m_size; }

protected:
  CallFrame *m_stack;
  size_type m_pos;
  size_type m_size;
}; //ActivationStack


/**
   This class implements the basic functions of a simple heap-based virtual
   machine suitable for executing simple Lispel programs. To avoid any
   restrictions in the possible uses of this VM, this class isn't tied to any
   external representations like byte codes. It simply provides one method for
   each instruction, so that it can be used as the basis of a byte code
   interpreter or an interpreter performing compiled functions, ...
   @version 0.1
*/
class LispelVM {
public:
  LispelVM( int stacksize);
  virtual ~LispelVM();

  virtual Handle_ptr halt();
  virtual void resolveBinding( Handle_ptr var);
  virtual void constant( Handle_ptr obj);
  virtual void createClosure( Handle_ptr vars, Handle_ptr body);
  virtual void ifThen( Handle_ptr thenCode, Handle_ptr elseCode);
  virtual void localAssign( Handle_ptr var);
  virtual void globalAssign( Handle_ptr var);
  virtual void createCont();
  virtual void nuate( Handle_ptr currStack, Handle_ptr var);
  virtual void pushFrame( Handle_ptr ret);
  virtual void appendArgument();
  virtual void apply();

  Handle_ptr getResult();
  void resetRegs();

protected:
  CallFrame *m_activationStack;
  int m_stackSize;
  int m_stackPos;
  Registers m_regs;
};


/**
   <p>CompilingEvaluator transforms each newly allocated closure to an
   intermediate byte code representation which is based on the descriptions
   in Chapter 2 of 'Three Implementation Models for Scheme' by R. Kent Dybvig.
   This implementation differs in that it doesn't rely on the heap as strongly
   as the VM  described in the document. The machine's internal data will
   only be moved to heap when it is really needed, that is, when closures and
   continuations are created.</p>
   @version 0.0
 */
class CompilingEvaluator : public Evaluator {
public:
  CompilingEvaluator( Context &ctx);
  virtual ~CompilingEvaluator();

  virtual Handle_ptr eval( Handle_ptr);
  virtual Handle_ptr evalExpression( Handle_ptr)
     throw( RecoverableException);
  virtual Handle_ptr evalVariable( Handle_ptr)
     throw( InternalInconsistency, UndefinedValue);

protected:
  Registers m_regs;
};


/**
  A (new and experimental) evaluator implementation which is similar to a VM based evaluator.
  @version 0.1
*/
class IterativeEvaluator : public Evaluator {
public:
  IterativeEvaluator( Context &ctx, ActivationStack::size_type ssz);
  ~IterativeEvaluator();

  virtual Handle_ptr eval( Handle_ptr);
  virtual Handle_ptr evalExpression( Handle_ptr)
     throw( RecoverableException);
  virtual Handle_ptr evalVariable( Handle_ptr)
     throw( InternalInconsistency, UndefinedValue);

protected:
  Registers m_regs;
  ActivationStack m_stack;
};

//@}

#endif /*lispel_LISPELVM_HH_*/
