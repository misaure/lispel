#include <gtest/gtest.h>
#include <sstream>
#include "lispel/lispelvm.hh"
#include "lispel/lispel.hh"

namespace {

class VMTest : public ::testing::Test {
public:
	VMTest();
	virtual ~VMTest();

	virtual void SetUp();
	virtual void TearDown();

	LispelVM *vm;
};

VMTest::VMTest() {

}

VMTest::~VMTest() {

}

void VMTest::SetUp() {
	vm = new LispelVM(1024);
}

void VMTest::TearDown() {
	delete vm;
}

TEST_F(VMTest,testEmptyVM) {

}

} // namespace
