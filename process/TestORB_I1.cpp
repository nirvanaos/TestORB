#include "TestORB_I1.h"
#include <I1_static.h>
#include <I1_factory_dynamic.h>
#include <I1_factory_portable.h>
#include <I1_factory_tied.h>
#include <I1_factory_sysdomain.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

#ifdef LEGACY_CORBA_CPP
typedef I1_var I1_ref;
#else
typedef I1::_ref_type I1_ref;
#endif

#ifdef LEGACY_CORBA_CPP

void test_interface (I1::_ptr_type p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	Object_ptr object = p;
	ASSERT_FALSE (is_nil (object));
	ASSERT_FALSE (object->_non_existent ());
	{
		I1_var p1 = I1::_narrow (object);
		ASSERT_FALSE (is_nil (p1));
		ASSERT_FALSE (p1->_non_existent ());
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
	}
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	EXPECT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));

	{
		String_var out = "this text will be lost", inout = "inout string";
		String_var in = "in string";
		String_var ret = p->string_op (in, out, inout);
		EXPECT_STREQ (ret.c_str (), "inout string");
		EXPECT_STREQ (out.c_str (), "in string");
		EXPECT_STREQ (inout.c_str (), "in string");
	}

	{ // Pass string constant as in parameter
		String_var out = "this text will be lost", inout = "inout string";
		String_var ret = p->string_op ("in string", out, inout);
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
		SeqLong out = { 1, 2, 3, 4 }, inout = { 5, 6, 7, 8 };
		SeqLong ret = p->seq_op (SeqLong{ 9, 10, 11, 12 }, out, inout);
		EXPECT_EQ (ret, SeqLong ({ 5, 6, 7, 8 }));
		EXPECT_EQ (out, SeqLong ({ 9, 10, 11, 12 }));
		EXPECT_EQ (inout, SeqLong ({ 9, 10, 11, 12 }));
	}

	{ // Any
		Any out, inout;
		Any in;
		Any ret = p->any_op (in, out, inout);
	}

	{ // Bounded string violation
		String_var out, inout;
		String_var ret;
		EXPECT_THROW (ret = p->short_string_op ("large string", out, inout), BAD_PARAM);
	}

	{ // Bounded sequence violation
		SeqLong out, inout;
		ShortSeqLong ret;
		EXPECT_THROW (ret = p->short_seq_op (SeqLong{ 9, 10, 11, 12, 13 }, out, inout), BAD_PARAM);
	}

	{ // Structure
		MyStruct_var out (new MyStruct{ L"out", 2, I1::_duplicate (p) });
		MyStruct_var inout (new MyStruct{ L"inout", 3, I1::_duplicate (p) });
		MyStruct_var ret = p->struct_op (MyStruct{ L"in", 1, I1::_duplicate (p) }, out.out (), inout.inout ());
		EXPECT_EQ (ret->ws_member, L"inout");
		EXPECT_EQ (out->ws_member, L"in");
		EXPECT_EQ (inout->ws_member, L"in");
	}
}

#else

void test_interface (I1::_ptr_type p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	Object::_ptr_type object = p;
	ASSERT_TRUE (object);
	ASSERT_FALSE (object->_non_existent ());
	{
		I1::_ref_type p1 = I1::_narrow (object);
		ASSERT_TRUE (p1);
		ASSERT_FALSE (p1->_non_existent ());
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
	}
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_TRUE (p->_is_a ("IDL:omg.org/CORBA/Object:1.0"));
	EXPECT_TRUE (p->_is_a ("IDL:Test/I1:1.0"));

	{
		std::string out = "this text will be lost", inout = "inout string";
		std::string in = "in string";
		std::string ret = p->string_op (in, out, inout);
		EXPECT_STREQ (ret.c_str (), "inout string");
		EXPECT_STREQ (out.c_str (), "in string");
		EXPECT_STREQ (inout.c_str (), "in string");
	}

	{ // Pass string constant as in parameter
		std::string out = "this text will be lost", inout = "inout string";
		std::string ret = p->string_op ("in string", out, inout);
		EXPECT_STREQ (ret.c_str (), "inout string");
		EXPECT_STREQ (out.c_str (), "in string");
		EXPECT_STREQ (inout.c_str (), "in string");
	}

	{
		I1::_ref_type out, inout (p);
		I1::_ref_type ret = p->object_op (p, out, inout);
		EXPECT_TRUE (out && out->_is_equivalent (p));
		EXPECT_TRUE (inout && inout->_is_equivalent (p));
		EXPECT_TRUE (ret && ret->_is_equivalent (p));
	}

	{
		std::vector <Long> out = { 1, 2, 3, 4 }, inout = { 5, 6, 7, 8 };
		std::vector <Long> ret = p->seq_op (std::vector <Long> { 9, 10, 11, 12 }, out, inout);
		EXPECT_EQ (ret, std::vector <Long> ({ 5, 6, 7, 8 }));
		EXPECT_EQ (out, std::vector <Long> ({ 9, 10, 11, 12 }));
		EXPECT_EQ (inout, std::vector <Long> ({ 9, 10, 11, 12 }));
	}

	{ // Any
		Any out, inout;
		Any in;
		Any ret = p->any_op (in, out, inout);
	}

	{ // Bounded string violation
		std::string out, inout;
		std::string ret;
		EXPECT_THROW (ret = p->short_string_op ("large string", out, inout), BAD_PARAM);
	}

	{ // Bounded sequence violation
		std::vector <Long> out, inout;
		std::vector <Long> ret;
		EXPECT_THROW (ret = p->short_seq_op (std::vector <Long> { 9, 10, 11, 12, 13 }, out, inout), BAD_PARAM);
	}

	{ // Structure
		MyStruct out (L"out", 2, p), inout (L"inout", 3, p);
		MyStruct ret = p->struct_op (MyStruct (L"in", 1, p), out, inout);
		EXPECT_EQ (ret.ws_member (), L"inout");
		EXPECT_EQ (out.ws_member (), L"in");
		EXPECT_EQ (inout.ws_member (), L"in");
	}
}

#endif

void test_performance (I1::_ptr_type p)
{
	for (int i = 0; i < 1000; ++i)
		p->op1 (2);
}

// The fixture for testing simple interface.

typedef ::testing::Types <Nirvana::Static <I1_factory_dynamic> // 0
	, Nirvana::Static <I1_factory_portable> // 1
	, Nirvana::Static <I1_static> // 2
	, Nirvana::Static <I1_factory_tied> // 3
	, Nirvana::Static <I1_tied_derived> // 4
	, Nirvana::Static <I1_factory_sysdomain> // 5
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

	static I1_ref incarnate (I1_factory::_ptr_type factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I1_ref incarnate (I1::_ptr_type obj)
	{
#ifdef LEGACY_CORBA_CPP
		return I1::_duplicate (obj);
#else
		return obj;
#endif
	}

	static I1_ref incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

};

TYPED_TEST_SUITE (TestORB_I1, ServantTypesI1);

TYPED_TEST (TestORB_I1, Interface)
{
	I1_ref p = TestORB_I1 <TypeParam>::incarnate ();
	test_interface (p);
}

TYPED_TEST (TestORB_I1, LargeSeq)
{
	// Large sequence
	size_t sa = (size_t)Nirvana::g_memory->query (nullptr, Nirvana::Memory::QueryParam::SHARING_ASSOCIATIVITY);
	if (sa) {
		I1_ref p = TestORB_I1 <TypeParam>::incarnate ();

		Long size = (Long)(sa * 2 / sizeof (Long));
		std::vector <Long> out, inout, in;
		out.resize (size);
		inout.resize (size);
		in.resize (size);
		for (Long i = 0; i < size; ++i) {
			out [i] = i;
		}
		for (Long i = 0; i < size; ++i) {
			inout [i] = i + size;
		}
		for (Long i = 0; i < size; ++i) {
			in [i] = i + size * 2;
		}
		std::vector <Long> inout_copy = inout;
		std::vector <Long> ret = p->seq_op (in, out, inout);
		EXPECT_EQ (ret, inout_copy);
		EXPECT_EQ (out, in);
		EXPECT_EQ (inout, in);
	}
}

TYPED_TEST (TestORB_I1, LargeString)
{
	// Large string
	size_t sa = (size_t)Nirvana::g_memory->query (nullptr, Nirvana::Memory::QueryParam::SHARING_ASSOCIATIVITY);
	if (sa) {
		I1_ref p = TestORB_I1 <TypeParam>::incarnate ();

		size_t size = sa * 2;
		std::string out, inout, in;
		out.resize (size, 'o');
		inout.resize (size, 'b');
		in.resize (size, 'i');
		std::string inout_copy = inout;
		std::string ret = p->string_op (in, out, inout);
		EXPECT_EQ (ret, inout_copy);
		EXPECT_EQ (out, in);
		EXPECT_EQ (inout, in);
	}
}

TYPED_TEST (TestORB_I1, Performance)
{
	I1_ref p = TestORB_I1 <TypeParam>::incarnate ();
	test_performance (p);
}

TYPED_TEST (TestORB_I1, SystemException)
{
	I1_ref p = TestORB_I1 <TypeParam>::incarnate ();
	EXPECT_THROW (p->throw_no_implement (), NO_IMPLEMENT);
}

TYPED_TEST (TestORB_I1, UserException)
{
	I1_ref p = TestORB_I1 <TypeParam>::incarnate ();
	bool thrown = false;
	try {
		p->throw_user ();
	} catch (const MyException& ex) {
		thrown = true;
#ifdef LEGACY_CORBA_CPP
		EXPECT_EQ (ex.param, "test");
		EXPECT_TRUE (ex.bparam);
#else
		EXPECT_EQ (ex.param (), "test");
		EXPECT_TRUE (ex.bparam ());
#endif
	}
	EXPECT_TRUE (thrown);
}

}
