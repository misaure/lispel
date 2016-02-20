#include <gtest/gtest.h>
#include "lispel/environment.hh"
#include "lispel/nodefactory.hh"
#include "lispel/interpreter.hh"

namespace {

class EnvTest : public ::testing::Test {
public:
	EnvTest() : toplevel(0), current(0) { }
	virtual ~EnvTest();
	virtual void SetUp();
	virtual void TearDown();
	virtual NodeFactory &factory();

	Interpreter interp;
	Environment *toplevel, *current;
};

EnvTest::~EnvTest() {
}

void EnvTest::SetUp() {
	toplevel = current = new Environment();
}

void EnvTest::TearDown() {
	current = toplevel = 0;
}

NodeFactory &EnvTest::factory() {
	return *(interp.context().factory);
}

TEST_F(EnvTest,testIsToplevelEnvironment) {
	Environment *env1 = new Environment();
	Environment *env2 = new Environment(env1);

	ASSERT_TRUE(env1->isToplevelEnvironment());
	ASSERT_TRUE(env2->isToplevelEnvironment());
}

TEST_F(EnvTest,testMakeChildEnvironment) {
	Environment *env1 = new Environment();
	Environment *env2 = env1->makeChildEnvironment();

	ASSERT_TRUE(0 != env2);
	ASSERT_TRUE(0 == env1->getParent());
	ASSERT_TRUE(0 != env2->getParent());
	ASSERT_TRUE(env1->isToplevelEnvironment());
	ASSERT_FALSE(env2->isToplevelEnvironment());
}

TEST_F(EnvTest,testDirectLookup) {
	Environment *env = new Environment();
	env->put(std::string("testval"), factory().makeInteger(0));

	ASSERT_TRUE(env->exists("testval"));
	Handle_ptr found = env->lookup("testval");

	ASSERT_FALSE(0 == found);
}

TEST_F(EnvTest,testParentLookup) {
	Environment *parent = new Environment();
	Environment *child = new Environment(parent);

	parent->put("parentOnly", factory().makeInteger(1));
	child->put("childOnly", factory().makeInteger(2));
	parent->put("both", factory().makeInteger(7));
	child->put("both", factory().makeInteger(8));

	ASSERT_EQ(1, parent->lookup("parentOnly")->integerValue());
	ASSERT_EQ(2, child->lookup("childOnly")->integerValue());
	ASSERT_EQ(7, parent->lookup("both")->integerValue());
	ASSERT_EQ(8, child->lookup("both")->integerValue());

	ASSERT_TRUE(0 == parent->lookup("childOnly"));
	ASSERT_TRUE(0 == child->lookup("parentOnly"));
}

// TODO replace debug output with assertions
TEST_F(EnvTest,testCascadedEnvironments) {
	int i;
	for (i = 0; i < 100; ++i) {
		char buffer[100];
		sprintf(buffer, "%d", i);
		std::string s(buffer);
		current->put(s, factory().makeString(buffer));
		if (0 == (i % 10)) {
			current = current->makeChildEnvironment();
		}
	}

	for (i = 100; i >= 0; --i) {
		char buffer[100];
		sprintf(buffer, "%d", i);
		std::string s(buffer);
		Handle_ptr res = current->lookup(s);
		if (0 != res) {
			std::cout << s << " == " << *res << std::endl;
		} else {
			std::cout << s << " == (null)" << std::endl;
		}
	}
}

} //namespace
