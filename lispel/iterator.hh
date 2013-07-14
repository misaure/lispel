/*
 * iterator.hh ---
 *
 *   Defines a hierarchy of iterators which can be used to implement 
 *   algorithms operating on sequence types.
 *
 * (c) 1999 by Michael Saure
 * RCSID: $Id: iterator.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_ITERATOR_HH_
#define _lispel_ITERATOR_HH_

//FIXME: 
class Handle;
typedef Handle *Handle_ptr;

struct OutputIterator {
  virtual void put( Handle_ptr) =0;
};

struct InputIterator {
  virtual Handle_ptr getNext() =0;
  virtual bool isLast() =0;
};

struct ForwardIterator {
  virtual bool next() =0;
  virtual void insert( Handle_ptr) =0;
  virtual bool isLast() =0;
  virtual Handle_ptr get() =0;
  virtual void reset() =0;
};

struct BidirectionalIterator : public ForwardIterator {
  virtual bool back() =0;
  virtual bool isFirst() =0;
};

struct RandomAccessIterator : public BidirectionalIterator {
  virtual Handle_ptr elementAt( int i) =0;
  virtual bool setElementAt( int i, Handle_ptr val);
};

#endif /*_lispel_ITERATOR_HH_*/

