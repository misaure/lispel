#include <gtest/gtest.h>
#include <sstream>
#include "lispel/lexer.hh"
#include "lispel/nodes.hh"
#include "lispel/lispel.hh"

namespace {

class LexerTest : public ::testing::Test {
public:
	LexerTest() {
	}

	virtual ~LexerTest() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

TEST_F(LexerTest, checkEval0) {
  NodeValue *val = new StringValue( "a little test");

  ASSERT_EQ(val, val->eval());
}

TEST_F(LexerTest, integerTokenTest) {
    Lexer::initLexerModule();
    
    std::istringstream is("100");
    Token t;
    is >> t;
    
    ASSERT_TRUE(t.hasType(Token::ttINT));
    ASSERT_EQ(t.lexval(), std::string("100"));
}

TEST_F(LexerTest, parenthesisTokenTest) {
    Lexer::initLexerModule();
    
    std::istringstream is("( )");
    Token t1, t2;
    
    is >> t1;
    
    ASSERT_TRUE(t1.hasType(Token::ttLPAREN));
    ASSERT_EQ(t1.lexval(), std::string("("));
    
    is >> t2;
    
    ASSERT_TRUE(t2.hasType(Token::ttRPAREN));
    ASSERT_EQ(t2.lexval(), std::string(")"));
}

TEST_F(LexerTest, simpleStringTokenTest) {
    Lexer::initLexerModule();
    
    std::istringstream is("\"this is a simple test\"");
    Token t;
    
    is >> t;
    
    ASSERT_TRUE(t.hasType(Token::ttSTRING));
    ASSERT_EQ(std::string("this is a simple test"), t.lexval());
}

TEST_F(LexerTest, keywordTokenTest) {
    Lexer::initLexerModule();
    
    std::istringstream is("string->symbol");
    Token t;
    
    is >> t;
    
    ASSERT_TRUE(t.hasType(Token::ttID));
    ASSERT_EQ(t.lexval(), std::string("string->symbol"));
}

TEST_F(LexerTest, nilTest0) {
   Interpreter interp;
   const Context &ctx = interp.context();

   ASSERT_NE(0, ctx.NIL);
   ASSERT_TRUE(ctx.NIL->hasType(Handle::ntCONS));
   ASSERT_TRUE(ctx.NIL->isNilRep());
}
} //namespace

