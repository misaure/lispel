#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LispelMathCommandTests

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <lispel/lexer.hh>
#include <boost/test/unit_test.hpp>
#include "lispel/nodes.hh"
#include "lispel/lispel.hh"

BOOST_AUTO_TEST_CASE(checkEval0)
{
  NodeValue *val = new StringValue( "a little test");

  BOOST_CHECK(val == val->eval());
}
