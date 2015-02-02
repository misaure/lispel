#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include "lispel/nodes.hh"
#include "lispel/lispel.hh"

int add(int i, int j)
{
	return i + j;
}

BOOST_AUTO_TEST_CASE(checkEval0)
{
  NodeValue *val = new StringValue( "a little test");

  BOOST_CHECK(val == val->eval());
}

BOOST_AUTO_TEST_CASE(universeInOrder)
{
	BOOST_CHECK(add(2,2) == 4);
}

BOOST_AUTO_TEST_CASE(nilTest0)
{
   Interpreter interp;
   const Context &ctx = interp.context();

   BOOST_CHECK(0 != ctx.NIL);
   BOOST_CHECK(ctx.NIL->hasType(Handle::ntCONS));
   BOOST_CHECK(ctx.NIL->isNilRep());
}

