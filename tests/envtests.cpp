#include <gtest/gtest.h>
#include "lispel/environment.hh"

TEST(EnvTest,testIsToplevelEnvironment) {
	Environment *env1 = new Environment();
	Environment *env2 = new Environment(env1);

	ASSERT_TRUE(env1->isToplevelEnvironment());
	ASSERT_TRUE(env2->isToplevelEnvironment());
}

TEST(EnvTest,testMakeChildEnvironment) {
	Environment *env1 = new Environment();
	Environment *env2 = env1->makeChildEnvironment();

	ASSERT_TRUE(0 != env2);
	ASSERT_TRUE(0 == env1->getParent());
	ASSERT_TRUE(0 != env2->getParent());
	ASSERT_TRUE(env1->isToplevelEnvironment());
	ASSERT_FALSE(env2->isToplevelEnvironment());
}

TEST(EnvTest,testDirectLookup) {
	Environment *env = new Environment();
	env->put(std::string("testval"), new IntegerHandle());

	ASSERT_TRUE(env->exists("testval"));
	Handle found = env->lookup("testval");

	ASSERT_FALSE(0 == found);
}

TEST(EnvTest,testParentLookup) {
	Environment *parent = new Environment();
	Environment *child = new Environment(parent);

	parent->put("parentOnly", new IntegerHandle(1));
	child->put("childOnly", new IntegerHandle(2));
	parent->put("both", new IntegerHandle(7));
	child->put("both", new IntegerHandle(8));

	ASSERT_EQ(1, parent->lookup("parentOnly")->integerValue());
	ASSERT_EQ(2, child->lookup("childOnly")->integerValue());
	ASSERT_EQ(7, parent->lookup("both")->integerValue());
	ASSERT_EQ(8, child->lookup("both")->integerValue());

	ASSERT_TRUE(0 == parent->lookup("childOnly"));
	ASSERT_TRUE(0 == child->lookup("parentOnly"));
}

