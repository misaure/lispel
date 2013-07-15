#ifndef lispel_DEFS_HH_
#define lispel_DEFS_HH_

#include <cassert>
//#warning "use miscclass/defs.hh instead"

#ifndef _
# define _( ASTRING) ASTRING
#endif

#define UNREACHABLE( DUMMYVAL) \
  throw InternalInconsistency( "unreachable code", __FILE__, __LINE__); \
  return (DUMMYVAL);

#define MCAssertValidInstance()
#define MCAssertNotReached(t)
#define MCAssert(cond,msg)

#endif /*lispel_DEFS_HH_*/

