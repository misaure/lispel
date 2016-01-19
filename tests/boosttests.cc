#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LispelTests

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <lispel/lexer.hh>
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

BOOST_AUTO_TEST_CASE(integerTokenTest)
{
    Lexer::initLexerModule();
    
    std::istringstream is("100");
    Token t;
    is >> t;
    
    BOOST_CHECK(t.hasType(Token::ttINT));
    BOOST_CHECK(t.lexval() == std::string("100"));
}

BOOST_AUTO_TEST_CASE(parenthesisTokenTest)
{
    Lexer::initLexerModule();
    
    std::istringstream is("( )");
    Token t1, t2;
    
    is >> t1;
    
    BOOST_CHECK(t1.hasType(Token::ttLPAREN));
    BOOST_CHECK(t1.lexval() == std::string("("));
    
    is >> t2;
    
    BOOST_CHECK(t2.hasType(Token::ttRPAREN));
    BOOST_CHECK(t2.lexval() == std::string(")"));
}

BOOST_AUTO_TEST_CASE(simpleStringTokenTest)
{
    Lexer::initLexerModule();
    
    std::istringstream is("\"this is a simple test\"");
    Token t;
    
    is >> t;
    
    BOOST_CHECK(t.hasType(Token::ttSTRING));
    BOOST_CHECK(std::string("this is a simple test") == t.lexval());
}

BOOST_AUTO_TEST_CASE(keywordTokenTest)
{
    Lexer::initLexerModule();
    
    std::istringstream is("string->symbol");
    Token t;
    
    is >> t;
    
    BOOST_CHECK(t.hasType(Token::ttID));
    BOOST_CHECK(t.lexval() == std::string("string->symbol"));
}

BOOST_AUTO_TEST_CASE(nilTest0)
{
   Interpreter interp;
   const Context &ctx = interp.context();

   BOOST_CHECK(0 != ctx.NIL);
   BOOST_CHECK(ctx.NIL->hasType(Handle::ntCONS));
   BOOST_CHECK(ctx.NIL->isNilRep());
}
