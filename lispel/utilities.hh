/*
 * utilities.hh ---
 *
 *   Unsorted helper routines and classes.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: utilities.hh,v 1.1.1.1 2001/10/31 15:44:43 michael Exp $
 */

#ifndef _lispel_UTILITIES_HH_
#define _lispel_UTILITIES_HH_

#include <lispel/nodes.hh>

static inline bool eq( const Handle *n1, const Handle *n2) {
  return n1==n2;
}

extern bool eqv( const Handle *n1, const Handle *n2);

extern const char *typetagToString( Handle::NodeType t);

/**
  Count the number of elements of a list without recursing into sublists.
  NIL aka () is regarded as a list of zero length.
 */
extern int listlength( const Handle *n);

//FIXME: this possibly needs to check the whole list once dotted pairs are
//implemented
static inline bool
isList( const Handle *n) {
  return (n->hasType( Handle::ntCONS) && n->cdr()->hasType( Handle::ntCONS));
}

/**
   Convert a a given range of an STL container to a consed list.
   @param begin Iterator pointing to the element where to start from (inclusive)
   @param end   Past-the-end iterator (exclusive)
   @return A consed list if begin != end initially, nil else.
*/
template<class ForwardIterator>
Handle_ptr makeList( ForwardIterator begin, ForwardIterator end, Context &ctx)
{
  if (begin != end) {
#if defined( DEBUG) && DEBUG > 2
    int i=1;
#endif
    ForwardIterator pos = begin;
    Handle_ptr listHead, current;
    listHead = current = ctx.factory->makeCons( *pos);
    ++pos;
    for (; pos!=end; ++pos) {
#if defined( DEBUG) && DEBUG > 2
      ++i;
#endif
      current->setCdr( ctx.factory->makeCons( *pos));
      current = current->cdr();
    }
#if defined( DEBUG) && DEBUG > 2
    std::cerr << "makeList: " << i << " elements in new list" << std::endl;
#endif
    return listHead;
  }
  return ctx.NIL;
}

/**
   Copy a Lispel consed list into a STL container by using some kind of insert
   iterator. This function does not recurse into sublists, so only the top level of
   a list will be copied.
   @param llist The consed lispel list which is to be copied.
   @param iiter Some kind of insert iterator for an STL container object.
*/
template<class InsertIterator>
InsertIterator copyList( Handle *llist, InsertIterator iiter)
{
  while (llist->hasType( Handle::ntCONS)) {
    if (llist->isNilRep())
      break;
    *iiter++ = llist->car();
    llist = llist->cdr();
  }
  return iiter;
}

/**
   A function object for printing (pointer to) handles using STL algorithms.
*/
class printNodeFunc {
  std::ostream &m_os;
public:
  printNodeFunc( std::ostream &os) : m_os( os) {}
  ~printNodeFunc() {}
  void operator()( const Handle *val) { m_os << (*val); }
};

/**
   This function can be used to check if a Lispel expression provided as a
   string representation is complete. Complete here doesn't mean that it is
   a <i>correct</i> Lispel expression, the function mainly checks if the 
   number of opening and closing parenthesis match.
   @param expr A string containing a Lispel expression.
 */
bool isCompleteExpression( std::string &expr);

#endif /*_lispel_UTILITIES_HH_*/
