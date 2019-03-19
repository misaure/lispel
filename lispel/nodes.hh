/*
 * nodes.hh ---
 *
 *   Implementation of tagged memory cells on which the interpreter operates.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 *
 * RCSID: $Id: nodes.hh,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#ifndef _lispel_NODES_HH_
#define _lispel_NODES_HH_ 1

#include <string>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <iostream>
#include <lispel/defs.hh>
#include <lispel/context.hh>
#include <lispel/exception.hh>
#include <lispel/pointer.hh>
#include <lispel/iterator.hh>


/** @name Lisp Tagged Memory Cells
    The classes in this section implement the Lisp memory model which is
    based on the notion of 'tagged memory cells'. All memory cells are
    managed by a heap manager which keeps indirect references of the actual
    values stored in a memory cell.<p>

    Indirect references to the data of course impose certain overhead on
    memory accesses, but on the other hand they make life easier for the
    garbage collector because some garbage collector types depend on being
    able to transparently move the data stored in the interpreters memory.
 */
//@{

class NodeValue;
typedef NodeValue *NodeValue_ptr;

/**
   This is the base class for actually storing the data contained in a tagged
   memory cell. The base class makes provisions for reference counting as a
   generalization of boolean garbage collection tags.
 */
class NodeValue {
public:
  NodeValue();
  virtual ~NodeValue();

  /**
     This method will be called by the garbage collector when the memory cell
     referencing this data item will be disposed of. After calling this method
     the destructor of the memory cell <i>might</i> be called. Whether this
     happens or not depends on the actual gc implementation.
  */
  virtual void finalize();

  /**
     This method implements the low-level part of the evaluation mechanism.
     The default implementation simply returns the value object's address,
     and thus implements self-evaluating objects.
  */
  virtual NodeValue_ptr eval();

  /**
     Increments the reference count by one. This method can either be used to
     set a GC tag or for implementation automatic memory management based on
     reference counting.
  */
  virtual void incRef();

  /**
     Decrements the reference of deletes a GC tag, resp.
  */
  virtual void decRef();

  /**
     Checks if there is only one reference to this element. If this method
     returns true then the class (or code) keeping a reference on this element
     can assume itself the owner of the NodeValue instance.
  */
  virtual bool isShared();

  /**
     Print a standard representation of the current value to a given stream.
     @return The stream given as argument after the value has been printed.
  */
  virtual std::ostream &printToStream( std::ostream &os) const =0;

  /**
     Implements an STL compliant unary function for printing NodeValue
     objects. The function operates on pointers to NodeValues to support
     dynamic binding.
  */
  struct print1Func {
    std::ostream *m_stream;
    /// Initializes the instance with the stream to be used for printing.
    print1Func( std::ostream *os) { m_stream = os; }
    ~print1Func() { }
    void operator()( const NodeValue *val) { val->printToStream( *m_stream); }
  };

  /// Provides the incRef member function as an STL compliant unary function.
  static void incRef1Func( NodeValue *val);

  /// Provides the decRef member function as an STL compliant unary function
  static void decRef1Func( NodeValue *val);

  /**
    Check if the Lispel type implemented by a descendant class is a
    sequence type. If this function returns true there must be at least
    one iterator available for traversing the sequence.
    @return The default implementation always returns <code>false</code>.
   */
  virtual bool isSequence();

  /**
    Create an output iterator for writing to a sequence by appending new
    elements.
    @return The default implementation always return 0, which means that no
    iterator of that type is available.
   */
  virtual OutputIterator        *getOutputIterator();

  /**
    Create an input iterator for reading from a sequence by moving from one
    element to the next.
    @return The default implementation always return 0, which means that no
    iterator of that type is available.
   */
  virtual InputIterator         *getInputIterator();

  /**
    Create an iterator which allows to retrieve elements by moving from one
    element to the next in one direction.
    @return The default implementation always return 0, which means that no
    iterator of that type is available.
   */
  virtual ForwardIterator       *getForwardIterator();

  /**
    Create an iterator which allows to retrieve elements by moving from one
    element to the next in both directions.
    @return The default implementation always return 0, which means that no
    iterator of that type is available.
   */
  virtual BidirectionalIterator *getBidirectionalIterator();

  /**
    Create an iterator which allows random access to all elements of a
    sequence.
    @return The default implementation always return 0, which means that no
    iterator of that type is available.
   */
  virtual RandomAccessIterator  *getRandomAccessIterator();

protected:
  int m_refcount;
}; /*class NodeValue*/
typedef ptr<NodeValue> NodeValue_var;


/**
   A holder class for string values.
*/
class StringValue : public NodeValue {
public:
  explicit StringValue( const char *s, bool quote);
  explicit StringValue( const char *s);
  virtual ~StringValue();

  virtual void finalize();
  virtual const char *stringValue() const;
  virtual char *stringValue();
  virtual int length() const;
  virtual std::ostream &printToStream( std::ostream &os) const;

protected:
  char *m_string;
  unsigned int m_len;
  bool m_quote;
}; /*class StringValue*/

typedef ptr<StringValue> StringValue_var;
typedef StringValue *StringValue_ptr;


/**
   A holder class for vectors of NodeValue objects.
*/
class VectorValue : public NodeValue {
public:
  typedef std::vector<Handle*>::value_type value_type;
  typedef std::vector<Handle*>::size_type size_type;
  typedef std::vector<Handle*>::iterator iterator;
  typedef std::vector<Handle*>::const_iterator const_iterator;

  explicit VectorValue( const std::vector<Handle*> &vals);
  virtual ~VectorValue();

  iterator begin() { return m_values.begin(); }
  const_iterator begin() const { return m_values.begin(); }
  iterator end() { return m_values.end(); }
  const_iterator end() const { return m_values.end(); }

  virtual void finalize();
  virtual void incRef();
  virtual Handle *elementAt( int i);
  virtual const Handle *elementAt( int i) const;
  virtual size_type length() const;
  virtual std::ostream &printToStream( std::ostream &os) const;

protected:
  std::vector<Handle*> m_values;
}; /*class VectorValue*/

typedef ptr<VectorValue> VectorValue_var;
typedef VectorValue *VectorValue_ptr;


class Handle;
class Environment;
class Evaluator;
typedef Handle *Handle_ptr;


/**
   Base class for C++ implemented functions. Contains the functionality to
   enable commands of keeping track of the name they are registered under.
   @version 0.1
*/
class CommandImpl : public NodeValue {
public:
  CommandImpl();
  virtual ~CommandImpl();

  virtual void setName( const std::string &name);
  virtual const std::string &name() const;
  virtual std::string name();

  virtual std::ostream &printToStream( std::ostream &os) const;

protected:
  std::string m_name;
}; /*class CommandImpl*/

/**
   This class wraps functions implemented in C++. This class can't be used
   to implement special forms.
   @version 0.2
 */
class BuiltinValue : public CommandImpl {
public:
  BuiltinValue();
  virtual ~BuiltinValue();

  /**
     Execute the function in the given environment with the given arguments.
     The arguments will be evaluated, so this class can't be used to
     implement special forms.
   */
  virtual Handle_ptr execute( Context &ctx, Environment *env, std::vector<Handle_ptr> args) =0;
  virtual std::ostream &printToStream( std::ostream &os) const;
};

/**
  Adapter to wrap a normal function to implement a builtin function.
*/
class CBuiltinAdapter : public BuiltinValue {
public:
  typedef Handle_ptr (*cimpl)(CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

  CBuiltinAdapter( cimpl cfun);
  virtual ~CBuiltinAdapter();

  /**
    The Adapter object itself will also be passed as an argument to the
    implementation function so that the function can request information
    about its name etc.
   */
  virtual Handle_ptr execute( Context &ctx, Environment *env, std::vector<Handle_ptr> args);
protected:
   cimpl m_func;
}; /*class CBuiltinAdapter*/

/**
   This adapter class can be used to make function of the type
   <code>Handle_ptr x Handle_ptr -> bool</code> available to the interpreter
   without explicitly implementing a wrapper.
   @version 0.1
*/
class BinaryPredicateAdapter : public BuiltinValue {
public:
  typedef bool (*binary_predicate)( const Handle*, const Handle*);

  /**
     Creates an adapter for the binary predicate passed as an argument.
  */
  BinaryPredicateAdapter( binary_predicate);
  ~BinaryPredicateAdapter();

  /**
     Checks the number of arguments and if exactly two arguments have been
     passed via args the wrapped binary predicate will be called.
     @return The result of executing the wrapped binary predicate after
     converting it to a Lispel value.
  */
  Handle_ptr execute( Context &ctx, Environment *env, std::vector<Handle_ptr> args);

protected:
  binary_predicate m_pred;
}; /*class BinaryPredicateAdapter*/


/**
  A base class for implementing special forms, i.e. builtins that require
  unevaluated arguments.
  @version 0.2
 */
class SpecialValue : public CommandImpl {
public:
   SpecialValue();
   virtual ~SpecialValue();

   /**
     Execute the function in the given environment with the given arguments.
     The arguments won't be evaluated, so you will have to apply eval()
     manually as needed.
     @param ctx Used to pass the components of an interpreter which a builtin
     implementation might want to access directly
     @param env The innermost binding environment that is active while the
     special form is being called.
     @param args The complete expression in an unevaluated form.
     @return The result of applying the special form in the current context.
    */
  virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr args) =0;
  virtual std::ostream &printToStream( std::ostream &os) const;
}; /*class SpecialValue*/


/**
  Implementation of Lispel's hash table data type.
  @version 0.1
  */
class Hashtable : public NodeValue {
public:
  typedef std::map<std::string,Handle_ptr> container_type;
  typedef container_type::iterator iterator;
  typedef container_type::const_iterator const_iterator;
  typedef container_type::value_type value_type;
  typedef container_type::size_type size_type;

  Hashtable();
  virtual ~Hashtable();

  iterator begin() { return m_table.begin(); }
  const_iterator begin() const { return m_table.begin(); }
  iterator end() { return m_table.end(); }
  const_iterator end() const { return m_table.end(); }

  virtual Handle_ptr lookup( const std::string &key);
  virtual void put( const std::string &key, Handle_ptr value);
  virtual int size();
  virtual void clear();
  virtual bool remove( const std::string &key);
  virtual bool exists( const std::string &key);

  virtual std::ostream &printToStream( std::ostream &os) const;

protected:
   container_type m_table;
}; /*class Hashtable*/


/**
   Implementation of Lispels set data type.
*/
class SetValue : public NodeValue {
public:
  typedef std::set<Handle_ptr> container_type;
  typedef container_type::iterator iterator;
  typedef container_type::const_iterator const_iterator;
  typedef container_type::value_type value_type;
  typedef container_type::size_type size_type;

  SetValue();
  virtual ~SetValue();

  iterator begin() { return m_set.begin(); }
  const_iterator begin() const { return m_set.begin(); }
  iterator end() { return m_set.end(); }
  const_iterator end() const { return m_set.end(); }

  virtual bool lookup( Handle_ptr value);
  virtual void put( Handle_ptr value);
  virtual bool remove( Handle_ptr value);
  virtual int size();
  virtual void clear();

  virtual std::ostream &printToStream( std::ostream &os) const;

protected:
  container_type m_set;
}; /*class SetValue*/

/**
  A first sketch of a possible implementation of object types supporting some
  introspection.
 */
class ObjectValue : public NodeValue {
public:
   ObjectValue();
   virtual ~ObjectValue();

   virtual bool haveAttribute( const std::string &attrname) =0;
   virtual bool setAttribute( const std::string &attrname, Handle_ptr val) =0;
   virtual Handle_ptr getAttribute( const std::string &attrname) =0;
   //FIXME: this should be replaced by an iterator oriented interface
   virtual std::vector<std::string> enumAttributes() =0;

   virtual bool haveMethod( const std::string &methname) =0;
   virtual std::vector<std::string> enumMethods() =0;
}; /*class ObjectValue*/


/**
   Wrapper for Lispel defined closures.
 */
class ClosureValue : public NodeValue {
public:
  ClosureValue( std::list<Handle_ptr>, Environment*, Handle_ptr);
  ~ClosureValue();

  virtual std::ostream &printToStream( std::ostream &os) const;
  virtual void incRef();

  /**
     Retrieves the number of formal parameters of a closure.
  */
  int arity() const;

  /**
     Get a closure body, i.e. the expression that will be executed upon
     application of the closure.
  */
  Handle_ptr body();
  const Handle_ptr body() const;

  /**
     Create a new environment local to a closure application that contains
     binding of the closure's formal parameters to the actual parameters.
   */
  Environment *bindArguments( std::vector<Handle_ptr> &argValues);

  /**
     Follows the chain of binding environments from the closure's private
     environment up to the toplevel binding environment and enters them into
     the set passed by reference. This function is used to garbage collect
     unreferenced (dropped) environments.
  */
  void collectReferencedEnvironments( std::set<Environment*>&);

protected:
  /// The names of the formal parameters of a closure.
  std::vector<std::string> m_argumentNames;
  /// The local binding environment active on the closure's definition.
  Environment *m_env;
  /// The expression making up the closure's body.
  Handle_ptr m_body;
}; /*class ClosureValue*/


/**
   An interface for ports, and endpoint for input and output operations.
*/
class PortValue : public NodeValue {
public:
  PortValue();
  virtual ~PortValue();

  virtual Handle_ptr peek() =0;
  virtual Handle_ptr get() =0;
  virtual Handle_ptr eof() =0;
  virtual Handle_ptr putBack( Handle_ptr c) =0;
  virtual std::ostream &printToStream( std::ostream &os) const;
}; /*class PortValue*/

typedef ptr<PortValue> PortValue_var;
typedef PortValue *PortValue_ptr;

/**
   Handle for NodeValue objects which also contains tag definitions.
   @version 0.3
*/
class Handle {
public:

  typedef unsigned long flagstype;

  /*
   * node types
   */
  typedef enum {
    ntEMPTY,
    ntCHARACTER, ntBOOLEAN, ntINTEGER, ntDOUBLE,
    ntSTRING, ntSYMBOL, ntKEYWORD, ntAVECTOR,
    ntARRAY, ntOBJECT, ntPORT, ntCONS, ntCFUNC, ntCLOSURE, ntHASHTABLE, ntSET
    /*, ntERROR*/
  } NodeType;

  /// Marks a cell as non-writable.
  static const flagstype CONSTFLAG;

  /**
    When this flag is set for a Handle and the referenced value represents
    immediately executable code (closures and primitives) then none of the
    arguments passed to the function during evaluation will itself be
    evaluated. This allows adding special forms as needed.
   */
  static const flagstype SPECIALFLAG;

  static const flagstype WATCHFLAG;

  /**
    Using this flag prevents a node from being recycled by the garbage
    collector.
   */
  static const flagstype GCSAFEFLAG;

  ~Handle();

  /**
    Retrieve the type flag.
    */
  NodeType type() const {
    MCAssertValidInstance();
    return m_nodetype;
  }

  bool checkFlag( flagstype f) const {
    MCAssertValidInstance();
    return (m_flags & f);
  }

  // Set a flag.
  void setFlag( flagstype f) {
    MCAssertValidInstance();
    m_flags |= f;
  }

  /**
    Checks if the current handle is of a given type.
    @param t Type tag to check for.
    @return true if the current handle is of type t.
   */
  bool hasType( NodeType t) const {
    MCAssertValidInstance();
    return m_nodetype == t;
  }

  /**
    Checks if the current handle represents a (primitive) numeric type.
   */
  bool isNumber() {
    MCAssertValidInstance();
    return (ntDOUBLE == m_nodetype || ntINTEGER == m_nodetype);
  }

  bool isNilRep() const {
    return (m_nodetype == ntCONS && 0 == val.m_cons.car && 0 == val.m_cons.cdr);
  }

  Handle_ptr car();
  const Handle *car() const;
  Handle_ptr cdr();
  const Handle *cdr() const;
  void setCar( Handle_ptr car);
  void setCdr( Handle_ptr car);

  /**
     Get the literal string value of a node.
     @exception lispel_exception Thrown if the node doesn't contain a string
     value.
  */
  const char *stringValue() const;
  char  *stringValue();

  /**
     Get the literal double value of a node.
     @exception lispel_exception Thrown if the node doesn't contain a double
     value.
  */
  double doubleValue() const;

  /**
     Get the integer value of a memory cell. Applicable only to ntINTEGER type
     cells.
     @exception lispel_exception Thrown if the cell isn't of type ntINTEGER.
  */
  int    integerValue() const;
  bool   booleanValue() const;

  Handle_ptr body();

  /**
    Get the NodeValue object referenced by this handle instance.
    Once immediate values (e.g. for storing double, boolean and integer
    values) will be used, the result of this operation might be
    <i>undefined</i>.
    */
  NodeValue_ptr typeImpl() {
    MCAssertValidInstance();
    return val.m_value;
  }

  void incRef();
  bool tagged() { return 0 != m_refcount; }

  /**
     Creates a new binding environment which contains the bindings of the
     actual parameters for a lambda application. The new environment will be
     a direct descendant of the environment stored upon creation of the
     closure (to implement lexical binding).
     @param argvalues Contains the actual parameter (values) for a application
     of a closure. The vector length must equal the closure's arity. Each of
     the values stored in this parameter will be entered into the new
     environment under the name of the corresponding formal parameter. This
     officially happens in an unspecified order (and, in reality, in the
     from left to right).
     @return A new binding environment in which the formal parameters of a
     closure are bound to the actual parameters computed for a specific
     application.
     @exception lispel_exception Will be thrown if the number of actual
     parameters doesn't match the number of formal parameters.
  */
  Environment *bindArguments( std::vector<Handle_ptr> &argvalues);

  /// <b>NOTE:</b> stub only!
  static NodeType registerType( NodeValue *td, const char *name);

  /**
     This function implements printing the contents of a memory cell in the
     style supported by the R4RS 'display' function.
  */
  friend std::ostream &operator<<( std::ostream &os, const Handle &h);
  friend class NodeFactory;
  friend class GarbageCollector;

protected:
  Handle();

  /// Type identifier for a node.
  NodeType m_nodetype;
  flagstype m_flags;

  union {
    /// types implemented through C++ classes
    NodeValue *m_value;
    /// immediately stored integer values
    int m_intValue;
    /// immediately stored double values
    double m_dblValue;
    /// immediately stored boolean values
    bool m_boolValue;

    /// immediatly stored arithmetic vectors, i.e. arrays of double values
    struct {
       double *data;
       int length;
    } m_avector;

    /// A cons cell consists of a head element and a pointer to the rest (list)
    struct {
      Handle_ptr car;
      Handle_ptr cdr;
    } m_cons;
  } val;

  /// Placeholder for later implementation of reference counting or GC tags
  int m_refcount;
}; /*class Handle*/

typedef ptr<Handle> Handle_var;

//@}

#endif /*_lispel_NODES_HH_*/
