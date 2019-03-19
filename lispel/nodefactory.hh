/*
 * nodefactory.hh ---
 *
 *   Declarations for the default node factory. A node factory can be seen
 *   as an allocation interface based on the interpreters heap.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: nodefactory.hh,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#ifndef _lispel_NODEFACTORY_HH_
#define _lispel_NODEFACTORY_HH_ 1

#include <cstdlib> //size_t
#include <list>
#include <stack>
#include <set>
#include <iostream>
#include <lispel/pointer.hh>
#include <lispel/lexer.hh>
#include <lispel/nodes.hh>
#include <lispel/exception.hh>
#include <lispel/context.hh>

class PortValue;

/**
   This class should be used to create any memory cell which should be visible
   inside the interpreter. In a classical lisp implementation it would simply
   serve as a facade to the heap and the garbage collector. The default
   implementation provides a very basic heap implementation completely inside
   one class.
   @version 0.2
 */
class NodeFactory {
  //FIXME: nearly all of the methods throw MemoryExhausted
public:
  typedef size_t size_type;
  typedef Handle value_type;
  typedef Handle *iterator;
  typedef const Handle *const_iterator;

  /**
     Creates a pool a preallocated cells which will be used to serve
     allocation requests. Only handles to the concrete values will be
     allocated, not the value holder classes itself.
     @param s Number of memory cells to preallocate.
  */
  NodeFactory( Context &ctx, size_type initsize);
  virtual ~NodeFactory();

  iterator begin() { return m_cells; }
  const_iterator begin() const { return m_cells; }
  iterator end() { return m_cells+m_size; }
  const_iterator end() const { return m_cells+m_size; }
  size_type size() const { return m_size; }

  /**
     Exception thrown when memory is exhausted. This exception is assumed
     to signal a recoverable error because the garbage collector might be
     able to free space.
  */
  struct MemoryExhausted : public RecoverableException {
    MemoryExhausted( std::string where, int line)
      : RecoverableException( "memory exhausted", where, line) {}
    virtual ~MemoryExhausted() throw() {}
  };

  virtual Handle_ptr makeValue( Handle::NodeType type, NodeValue *val);
  /// Creates a new character node.
  virtual Handle_ptr makeCharacter( int c);
  /// Creates a new boolean node. Instances of true and false may be unique.
  virtual Handle_ptr makeBoolean( bool b);
  virtual Handle_ptr makeInteger( int v);
  virtual Handle_ptr makeDouble( double d);
  virtual Handle_ptr makeString( const char *s);
  virtual Handle_ptr makeSymbol( const char *s);
  virtual Handle_ptr makeKeyword( const char *s);
  virtual Handle_ptr makeVector( std::vector<Handle*> &v);
  virtual Handle_ptr makePort( PortValue *pv);
  virtual Handle_ptr makeHashtable();
  virtual Handle_ptr makeSet();

  /// Creates an empty cons cell.
  virtual Handle_ptr makeCons();
  /// Creates a new cons cell with the given content for its CAR field.
  virtual Handle_ptr makeCons( Handle_ptr car);
  virtual Handle_ptr makeCons( Handle_ptr car, Handle_ptr cdr);
  virtual Handle_ptr makeClosure( std::list<Handle_ptr> args, Environment *env,
				  Handle_ptr functionBody);
  /**
     Allocate a new memory cell and assign it a value. The node's type will
     be derived from the type of the token given as an argument. The nodes
     value, if any, will be derived from the token's lexical value.
     @param tok A token which is to be translated to a memory cell.
  */
  virtual Handle_ptr nodeFromToken( const Token_ptr tok);

  /**
     Return the number of preallocated memory cells still available. This
     number isn't very informative as the available memory could be 
     increased automatically or some amount of memory could be released by
     the next run of the garbage collector.
  */
  virtual size_type freeCount();

  /**
     Prints a memory cells content to a given stream.
     @param node Memory cell which is to be printed.
     @param os An output stream to which the memory cell is to be printed.
  */
  virtual void display( Handle_ptr node, std::ostream *os);
  virtual void displayAll( std::ostream *os);

  /**
     This method should be called after each complete garbage collection cycle
     to reset the tags set by the garbage collector.
     FIXME: maybe this method shouldn't be public
  */
  virtual void untagNonEmpty();
  virtual int recycleUntagged();

  /**
     Collect all binding environments referenced by the closures stored on
     the heap.
   */
  virtual std::set<Environment*> activeEnvironments();

  friend class Interpreter;

protected:
  /// Returns a node instance representing NIL (which normally is unique)
  virtual Handle_ptr makeNil();

  /// Allocates a new node on the heap.
  inline Handle_ptr allocNode( Handle::NodeType);

  /// Return a node to the pool of free nodes.
  inline void recycleNode( Handle_ptr);

  typedef std::list<int> handlelist_type;

  Handle *m_cells;
  size_type m_size;
  handlelist_type m_free;

  Context &m_ctx;
};

typedef ptr<NodeFactory> NodeFactory_var;
typedef NodeFactory *NodeFactory_ptr;

#endif /*_lispel_NODEFACTORY_HH_*/
