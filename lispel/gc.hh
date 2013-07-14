/*
 * gc.hh ---
 *
 *    Public garbage collector interface.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: gc.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_GC_HH_
#define _lispel_GC_HH_

#include <list>
#include <lispel/context.hh>

class NodeFactory;

/**
   Public interface for garbage collectors. The requirements for a minimal
   interaction between the garbage collector and the other components of the
   interpreter haven't been worked out in full detail yet, so expect this
   interface to change in the future.<p>
   The current implementation provides two different methods for triggering
   garbage collection. One starts a complete garbage collection, the other
   requests a incremental garbage collection. This distinction was made to 
   allow optimal implementations of each of both modes.
   @version 0.0
*/
class GarbageCollector {
public:
  explicit GarbageCollector( Context&);
  virtual ~GarbageCollector();

  /**
    Request garbage collection for the complete heap.
    */
  virtual int fullGC() =0;
  
  /**
    Request incremental garbage collection for the given number of memory
    cells. The implementation should be able to figure out by itself in what
    range of the heap the garbage collection should take place to make sure
    that successive calls to this method collect garbage on all memory cells
    allocated on the heap.
   */
  virtual int incrementalGC( int amount) =0;

protected:
  NodeFactory *m_factory;
  Context &m_ctx;
};

/**
   Implements a very simple incremental garbage collector. It is tailored as
   a counterpart to the SimpleEvaluator because it doesn't look at any 
   activation stack. This especially means that the garbage collector only can
   do its job on safe points.
   @version 0.0
*/
class SimpleGC : public GarbageCollector {
public:
  explicit SimpleGC( Context&);
  virtual ~SimpleGC();

  /**
     Performs a full garbage collection cycle. This might take some time,
     depending on the memory size and memory organization. On the other hand
     a full gc cycle can be implemented more efficiently than subsequent
     incremental gc cycle.
     @return The number of memory cells returned to the pool of free nodes.
  */
  virtual int fullGC();

  /**
     Performs garbage collection on a subrange of the heap.
     @param amount The number of nodes that should be visited by the 
     garbage collector.
     @return The number of memory cells returned to the pool of free nodes.
  */
  virtual int incrementalGC( int amount);

  /**
     When incremental garbage collection has been performed on parts of the
     heap, this method will complete the gc cycle started by visiting all
     memory cells not checked by previous calls to incrementalGC.
  */
  virtual void finishCycle();

public:
  bool m_incrementalMode;
  std::list<int> m_releasePool;
};

#endif /*_lispel_GC_HH_*/
