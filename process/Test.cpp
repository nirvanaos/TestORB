#include "I1_static.h"
#include "I1_dynamic.h"
#include "I1_portable.h"
#include "I1_tied.h"
#include "I3_static.h"
#include "I3_dynamic.h"
#include "I3_portable.h"
#include "I3_tied.h"
#include "TestORB.h"
#include <gtest/gtest.h>

using namespace std;

namespace TestORB {

using namespace CORBA;
using namespace Test;

void test_interface (I1_ptr p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	Object_ptr object = p;
	ASSERT_FALSE (is_nil (object));
	ASSERT_FALSE (object->_non_existent ());
	AbstractBase_ptr ab = object;
	ASSERT_FALSE (is_nil (ab));
	{
		Object_ptr o1 = ab->_to_object ();
		ASSERT_FALSE (is_nil (o1));
		ASSERT_FALSE (o1->_non_existent ());
		release (o1);
	}
	{
		I1_ptr p1 = I1::_narrow (object);
		ASSERT_FALSE (is_nil (p1));
		ASSERT_FALSE (p1->_non_existent ());
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		release (p1);
	}
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	EXPECT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));

	{
		string out = "this text will be lost", inout = "inout string";
		string in = "in string";
		string ret = p->string_op (in, out, inout);
		EXPECT_STREQ (ret.c_str (), "inout string");
		EXPECT_STREQ (out.c_str (), "in string");
		EXPECT_STREQ (inout.c_str (), "in string");
	}

	{ // Pass string constant as in parameter
		string out = "this text will be lost", inout = "inout string";
		string ret = p->string_op ("in string", out, inout);
		EXPECT_STREQ (ret.c_str (), "inout string");
		EXPECT_STREQ (out.c_str (), "in string");
		EXPECT_STREQ (inout.c_str (), "in string");
	}

	{
		I1_var out, inout (I1::_duplicate (p));
		I1_var ret = p->object_op (p, out, inout);
		EXPECT_TRUE (out && out->_is_equivalent (p));
		EXPECT_TRUE (inout && inout->_is_equivalent (p));
		EXPECT_TRUE (ret && ret->_is_equivalent (p));
	}

	{
		vector <Long> out = { 1, 2, 3, 4 }, inout = { 5, 6, 7, 8 };
		vector <Long> ret = p->seq_op (vector <Long> { 9, 10, 11, 12 }, out, inout);
		EXPECT_EQ (ret, vector <Long> ({ 5, 6, 7, 8 }));
		EXPECT_EQ (out, vector <Long> ({ 9, 10, 11, 12 }));
		EXPECT_EQ (inout, vector <Long> ({ 9, 10, 11, 12 }));
	}

	{
		CORBA::Any out, inout;
		CORBA::Any in;
		CORBA::Any ret = p->any_op (in, out, inout);
	}

	release (p);
}

void test_performance (I1_ptr p)
{
	for (int i = 0; i < 1000000; ++i)
		p->op1 (2);
	release (p);
}

// The fixture for testing simple interface.

typedef ::testing::Types <I1_dynamic
#ifndef TEST_NO_POA
	, I1_portable
#endif
#ifndef TEST_NO_STATIC
	, I1_static
#endif
#ifndef TEST_NO_TIED
	, I1_tied
	, I1_tied_derived
#endif
> ServantTypesI1;

template <class Factory>
class TestORB_I1 :
	public ::testing::Test
{
protected:
	TestORB_I1 ()
	{}

	virtual ~TestORB_I1 ()
	{}

	static I1_ptr incarnate (I1_factory_ptr factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I1_ptr incarnate (I1_ptr obj)
	{
		return obj;
	}

	static I1_ptr incarnate ()
	{
		return incarnate (::Nirvana::Static <Factory>::ptr ());
	}
};

TYPED_TEST_SUITE (TestORB_I1, ServantTypesI1);

TYPED_TEST (TestORB_I1, Interface)
{
	test_interface (TestORB_I1 <TypeParam>::incarnate ());
}

TYPED_TEST (TestORB_I1, Performance)
{
	test_performance (TestORB_I1 <TypeParam>::incarnate ());
}

TYPED_TEST (TestORB_I1, Exception)
{
	I1_ptr p = TestORB_I1 <TypeParam>::incarnate ();
	EXPECT_THROW (p->throw_no_implement (), NO_IMPLEMENT);
	release (p);
}

// The fixture for testing complex interface.

typedef ::testing::Types <I3_dynamic
#ifndef TEST_NO_POA
	, I3_portable
#endif
#ifndef TEST_NO_STATIC
	, I3_static
#endif
#ifndef TEST_NO_TIED
	, I3_tied
	, I3_tied_derived
#endif
> ServantTypesI3;

template <class Factory>
class TestORB_I3 :
	public ::testing::Test
{
protected:
	TestORB_I3 ()
	{}

	virtual ~TestORB_I3 ()
	{}

	static I3_ptr incarnate (I3_factory_ptr factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I3_ptr incarnate (I3_ptr obj)
	{
		return obj;
	}

	static I3_ptr incarnate ()
	{
		return incarnate (::Nirvana::Static <Factory>::ptr ());
	}
};

TYPED_TEST_SUITE (TestORB_I3, ServantTypesI3);

TYPED_TEST (TestORB_I3, Interface)
{
	test_interface (TestORB_I3 <TypeParam>::incarnate ());
}

TYPED_TEST (TestORB_I3, Performance)
{
	test_performance (TestORB_I3 <TypeParam>::incarnate ());
}

TYPED_TEST (TestORB_I3, MultiInherit)
{
	I3_var p = TestORB_I3 <TypeParam>::incarnate ();

	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);
	EXPECT_EQ (p->op3 (1), 3 * MAGIC_CONST + 1);

	{
		I1_ptr p1 = p;
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p1);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
	}

	{
		I2_ptr p2 = p;
		EXPECT_EQ (p2->op2 (1), 2 * MAGIC_CONST + 1);
		I3_ptr p3 = I3::_narrow (p2);
		ASSERT_FALSE (is_nil (p3));
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
		release (p3);
	}

	{
		Object_ptr obj = p;
		ASSERT_FALSE (is_nil (obj));
		I1_ptr p1 = I1::_narrow (obj);
		ASSERT_FALSE (is_nil (p1));
		I2_ptr p2 = I2::_narrow (obj);
		ASSERT_FALSE (is_nil (p2));
		I3_ptr p3 = I3::_narrow (obj);
		ASSERT_FALSE (is_nil (p3));
		release (p1);
		release (p2);
		release (p3);
	}

	// release (p) must be called automatically.
}

}
