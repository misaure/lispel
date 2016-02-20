#include <gtest/gtest.h>
#include "lispel/environment.hh"
#include "lispel/nodefactory.hh"
#include "lispel/interpreter.hh"

namespace {

class FactoryTest : public ::testing::Test {
public:
	FactoryTest() : toplevel(0), current(0) { }
	virtual ~FactoryTest();
	virtual void SetUp();
	virtual void TearDown();

	Interpreter interp;
	Environment *toplevel, *current;

	virtual NodeFactory &factory();
};

FactoryTest::~FactoryTest() {

}

void FactoryTest::SetUp() {
	toplevel = current = new Environment();
}

void FactoryTest::TearDown() {
	current = toplevel = 0;
}

NodeFactory &FactoryTest::factory() {
	return *(interp.context().factory);
}

TEST_F(FactoryTest,thatNilIsGeneratedCorrecly) {
    Handle_ptr nil = interp.context().NIL;
    ASSERT_FALSE(0 == nil);
    ASSERT_TRUE(nil->hasType(Handle::ntCONS));
}

TEST_F(FactoryTest,thatCharactersAreGeneratedCorrectly) {
	Handle_ptr c = factory().makeCharacter('a');
	ASSERT_FALSE(0 == c);
	ASSERT_TRUE(c->hasType(Handle::ntCHARACTER));
}

TEST_F(FactoryTest,thatBoolansCreatedAreValid) {
    Handle_ptr newNode=factory().makeBoolean( true);
    ASSERT_TRUE( newNode->hasType(Handle::ntBOOLEAN));

    newNode = factory().makeBoolean( false);
    ASSERT_TRUE( newNode->hasType(Handle::ntBOOLEAN));
}

TEST_F(FactoryTest,thatIntegersCreatedAreValid) {
    Handle_ptr newNode=factory().makeInteger( 1001);
    ASSERT_TRUE( newNode->hasType( Handle::ntINTEGER));
}

TEST_F(FactoryTest,thatDoublesCreatedAreValid) {
    Handle_ptr newNode=factory().makeDouble( 1001.0);
    ASSERT_TRUE( newNode->hasType( Handle::ntDOUBLE));
}

TEST_F(FactoryTest,thatStringsCreatedAreValid) {
    Handle_ptr newNode = factory().makeString( "a string");
    ASSERT_TRUE(newNode->hasType( Handle::ntSTRING));
}

TEST_F(FactoryTest,thatSymbolsCreatedAreValid) {
    Handle_ptr newNode = factory().makeSymbol( "string->number");
    ASSERT_TRUE( Handle::ntSYMBOL);
}

} //namespace
