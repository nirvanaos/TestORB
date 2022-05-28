#include <Nirvana/Nirvana.h>
#include "I1_static.h"
#include "I1_factory_dynamic.h"
#include "I1_factory_portable.h"
#include "I1_factory_tied.h"
#include "I3_static.h"
#include "I3_factory_dynamic.h"
#include "I3_factory_portable.h"
#include "I3_factory_tied.h"
#include "TestORB.h"
#include <gtest/gtest.h>
#include <signal.h>

using namespace std;

// There is some bug with virtual implementation when CLang for Visual Studio
// is used. Maybe even a bug in CLang.
#if defined (_MSVC_LANG) && defined (__clang__)
#define TEST_NO_POA
#endif

namespace TestORB {

using namespace CORBA;
using namespace Test;

#ifdef LEGACY_CORBA_CPP

void test_interface (I1_ptr p)
{
	ASSERT_FALSE (is_nil (p));
	ASSERT_FALSE (p->_non_existent ());
	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	Object_ptr object = p;
	ASSERT_FALSE (is_nil (object));
	ASSERT_FALSE (object->_non_existent ());
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

	{
		Any out, inout;
		Any in;
		Any ret = p->any_op (in, out, inout);
	}

	{
		String_var out, inout;
		String_var ret;
		EXPECT_THROW (ret = p->short_string_op ("large string", out, inout), BAD_PARAM);
	}

	{
		SeqLong out, inout;
		ShortSeqLong ret;
		EXPECT_THROW (ret = p->short_seq_op (SeqLong { 9, 10, 11, 12, 13 }, out, inout), BAD_PARAM);
	}

	release (p);
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
		I1::_ptr_type p1 = I1::_narrow (object);
		ASSERT_TRUE (p1);
		ASSERT_FALSE (p1->_non_existent ());
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
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
		I1::_ref_type out, inout (p);
		I1::_ref_type ret = p->object_op (p, out, inout);
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
		Any out, inout;
		Any in;
		Any ret = p->any_op (in, out, inout);
	}

	{
		string out, inout;
		string ret;
		EXPECT_THROW (ret = p->short_string_op ("large string", out, inout), BAD_PARAM);
	}

	{
		vector <Long> out, inout;
		vector <Long> ret;
		EXPECT_THROW (ret = p->short_seq_op (vector <Long> { 9, 10, 11, 12, 13 }, out, inout), BAD_PARAM);
	}
}

#endif

#ifdef LEGACY_CORBA_CPP

void test_performance (I1_ptr p)
{
	for (int i = 0; i < 10000; ++i)
		p->op1 (2);
	release (p);
}

#else

void test_performance (I1::_ptr_type p)
{
	for (int i = 0; i < 10000; ++i)
		p->op1 (2);
}

#endif

// The fixture for testing simple interface.

typedef ::testing::Types < ::Nirvana::Static <I1_factory_dynamic>
#ifndef TEST_NO_POA
	, ::Nirvana::Static <I1_factory_portable>
#endif
#ifndef TEST_NO_STATIC
	, ::Nirvana::Static <I1_static>
#endif
#ifndef TEST_NO_TIED
	, ::Nirvana::Static <I1_factory_tied>
	, ::Nirvana::Static <I1_tied_derived>
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

#ifdef LEGACY_CORBA_CPP

	static I1_ptr incarnate (I1_factory_ptr factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I1_ptr incarnate (I1_ptr obj)
	{
		return I1::_duplicate (obj);
	}

	static I1_ptr incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

#else

	static I1::_ref_type incarnate (I1_factory::_ptr_type factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I1::_ref_type incarnate (I1::_ptr_type obj)
	{
		return obj;
	}

	static I1::_ref_type incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

#endif
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

#ifdef LEGACY_CORBA_CPP

TYPED_TEST (TestORB_I1, SystemException)
{
	I1_ptr p = TestORB_I1 <TypeParam>::incarnate ();
	EXPECT_THROW (p->throw_no_implement (), NO_IMPLEMENT);
	release (p);
}

#else

TYPED_TEST (TestORB_I1, SystemException)
{
	I1::_ref_type p = TestORB_I1 <TypeParam>::incarnate ();
	EXPECT_THROW (p->throw_no_implement (), NO_IMPLEMENT);
}

#endif

#ifdef LEGACY_CORBA_CPP

TYPED_TEST (TestORB_I1, UserException)
{
	I1_ptr p = TestORB_I1 <TypeParam>::incarnate ();
	bool thrown = false;
	try {
		p->throw_user ();
	} catch (const MyException& ex) {
		thrown = true;
		EXPECT_EQ (ex.param, "test");
		EXPECT_TRUE (ex.bparam);
	}
	EXPECT_TRUE (thrown);
	release (p);
}

#else

TYPED_TEST (TestORB_I1, UserException)
{
	I1::_ref_type p = TestORB_I1 <TypeParam>::incarnate ();
	bool thrown = false;
	try {
		p->throw_user ();
	} catch (const MyException& ex) {
		thrown = true;
		EXPECT_EQ (ex.param (), "test");
		EXPECT_TRUE (ex.bparam ());
	}
	EXPECT_TRUE (thrown);
}

#endif

// The fixture for testing complex interface.

typedef ::testing::Types < ::Nirvana::Static <I3_factory_dynamic>
#ifndef TEST_NO_POA
	, ::Nirvana::Static <I3_factory_portable>
#endif
#ifndef TEST_NO_STATIC
	, ::Nirvana::Static <I3_static>
#endif
#ifndef TEST_NO_TIED
	, ::Nirvana::Static <I3_factory_tied>
	, ::Nirvana::Static <I3_tied_derived>
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

#ifdef LEGACY_CORBA_CPP

	static I3_ptr incarnate (I3_factory_ptr factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I3_ptr incarnate (I3_ptr obj)
	{
		return I3::_duplicate (obj);
	}

	static I3_ptr incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

#else

	static I3::_ref_type incarnate (I3_factory::_ptr_type factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I3::_ref_type incarnate (I3::_ptr_type obj)
	{
		return obj;
	}

	static I3::_ref_type incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

#endif
};

TYPED_TEST_SUITE (TestORB_I3, ServantTypesI3);

TYPED_TEST (TestORB_I3, Interface)
{
	test_interface (TestORB_I3 <TypeParam>::incarnate ());
}

TYPED_TEST (TestORB_I3, Signal)
{
#ifdef LEGACY_CORBA_CPP
	I3_var
#else
	I3::_ref_type
#endif
	p = TestORB_I3 <TypeParam>::incarnate ();

	EXPECT_EQ (p->divide (6, 2), 3);

	bool ok = false;
	try {
		p->divide (5, 0);
	} catch (const CORBA::SystemException& ex) {
		ok = true;
		const CORBA::ARITHMETIC_ERROR* ae = CORBA::ARITHMETIC_ERROR::_downcast (&ex);
		EXPECT_TRUE (ae);
		EXPECT_EQ (ae->minor (), FPE_INTDIV);
	}
	EXPECT_TRUE (ok);
}

TYPED_TEST (TestORB_I3, Performance)
{
	test_performance (TestORB_I3 <TypeParam>::incarnate ());
}

#ifdef LEGACY_CORBA_CPP

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

#else

TYPED_TEST (TestORB_I3, MultiInherit)
{
	I3::_ref_type p = TestORB_I3 <TypeParam>::incarnate ();

	EXPECT_EQ (p->op1 (1), MAGIC_CONST + 1);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);
	EXPECT_EQ (p->op3 (1), 3 * MAGIC_CONST + 1);

	{
		I1::_ref_type p1 = p;
		EXPECT_EQ (p1->op1 (1), MAGIC_CONST + 1);
		I3::_ref_type p3 = I3::_narrow (p1);
		ASSERT_TRUE (p3);
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
	}

	{
		I2::_ref_type p2 = p;
		EXPECT_EQ (p2->op2 (1), 2 * MAGIC_CONST + 1);
		I3::_ref_type p3 = I3::_narrow (p2);
		ASSERT_TRUE (p3);
		EXPECT_EQ (p3->op3 (1), 3 * MAGIC_CONST + 1);
	}

	{
		Object::_ref_type obj = p;
		ASSERT_TRUE (obj);
		I1::_ref_type p1 = I1::_narrow (obj);
		ASSERT_TRUE (p1);
		I2::_ref_type p2 = I2::_narrow (obj);
		ASSERT_TRUE (p2);
		I3::_ref_type p3 = I3::_narrow (obj);
		ASSERT_TRUE (p3);
	}
}

#endif

class TestORB : public ::testing::Test
{
public:
};

TEST_F (TestORB, UserException)
{
	::Test::MyException e;
	EXPECT_THROW (e._raise (), ::Test::MyException);
}

TEST_F (TestORB, Environment)
{
	CORBA::Internal::Environment ne;
	CORBA::NO_MEMORY no_mem;
	set_exception (&ne, no_mem);
	const CORBA::Exception* ex = ne.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	CORBA::Internal::Environment ne1 (move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");

	CORBA::Internal::EnvironmentEx <::Test::MyException> nex;
	::Test::MyException my_ex;
	CORBA::Internal::set_exception (&nex, my_ex);
	CORBA::Internal::Environment ne2 (move (nex));
	ex = ne2.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), ::Test::MyException::__name ());
}

TEST_F (TestORB, TypeCode)
{
	EXPECT_EQ (_tc_I1->id (), "IDL:Test/I1:1.0");
	EXPECT_EQ (_tc_I1->kind (), TCKind::tk_objref);

	EXPECT_EQ (_tc_MyException->kind (), TCKind::tk_except);
	EXPECT_EQ (_tc_MyException->id (), "IDL:Test/MyException:1.0");
	EXPECT_EQ (_tc_MyException->name (), "MyException");
	EXPECT_EQ (_tc_MyException->member_count (), 2);
	EXPECT_EQ (_tc_MyException->member_name (0), "param");
	EXPECT_EQ (_tc_MyException->member_name (1), "bparam");
	EXPECT_EQ (_tc_MyException->member_type (0)->kind (), TCKind::tk_string);
	EXPECT_EQ (_tc_MyException->member_type (1)->kind (), TCKind::tk_boolean);

	EXPECT_EQ (_tc_MyStruct->kind (), TCKind::tk_struct);
	EXPECT_EQ (_tc_MyStruct->id (), "IDL:Test/MyStruct:1.0");
	EXPECT_EQ (_tc_MyStruct->name (), "MyStruct");
	EXPECT_GE (_tc_MyStruct->member_count (), 2u);
	EXPECT_LE (_tc_MyStruct->member_count (), 3u);
	EXPECT_EQ (_tc_MyStruct->member_name (0), "ws_member");
	EXPECT_EQ (_tc_MyStruct->member_name (1), "l_member");
	EXPECT_EQ (_tc_MyStruct->member_type (0)->kind (), TCKind::tk_wstring);
	EXPECT_EQ (_tc_MyStruct->member_type (1)->kind (), TCKind::tk_long);
	if (_tc_MyStruct->member_count () > 2)
		EXPECT_EQ (_tc_MyStruct->member_type (2)->kind (), TCKind::tk_objref);

	EXPECT_EQ (_tc_SeqLong->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_SeqLong->id (), "IDL:Test/SeqLong:1.0");
	EXPECT_EQ (_tc_SeqLong->name (), "SeqLong");

#ifdef LEGACY_CORBA_CPP
	TypeCode::_var_type seq = _tc_SeqLong->content_type ();
#else
	TypeCode::_ref_type seq = _tc_SeqLong->content_type ();
#endif
	EXPECT_EQ (seq->kind (), TCKind::tk_sequence);
	EXPECT_EQ (seq->length (), 0);

	EXPECT_FALSE (_tc_SeqLong->equal (seq));
	EXPECT_TRUE (_tc_SeqLong->equivalent (seq));
	EXPECT_TRUE (_tc_SeqLong->equal (_tc_SeqLong));

#ifdef LEGACY_CORBA_CPP
	TypeCode::_var_type l = seq->content_type ();
#else
	TypeCode::_ref_type l = seq->content_type ();
#endif
	EXPECT_EQ (l->kind (), TCKind::tk_long);

	EXPECT_EQ (_tc_MyAlias->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_MyAlias->id (), "IDL:Test/MyAlias:1.0");
	EXPECT_EQ (_tc_MyAlias->name (), "MyAlias");
	EXPECT_TRUE (_tc_MyAlias->content_type ()->equal (_tc_SeqLong));
	EXPECT_TRUE (_tc_MyAlias->equivalent (_tc_SeqLong));

	EXPECT_EQ (_tc_TypeCode->kind (), TCKind::tk_TypeCode);
	EXPECT_THROW (_tc_TypeCode->id (), TypeCode::BadKind);

	EXPECT_EQ (_tc_Object->kind (), TCKind::tk_objref);
	EXPECT_EQ (_tc_Object->id (), CORBA_REPOSITORY_ID ("Object"));
	EXPECT_EQ (_tc_Object->name (), "Object");

	EXPECT_EQ (_tc_string->kind (), TCKind::tk_string);
	EXPECT_THROW (_tc_string->id (), TypeCode::BadKind);

	EXPECT_EQ (_tc_wstring->kind (), TCKind::tk_wstring);
	EXPECT_THROW (_tc_wstring->id (), TypeCode::BadKind);

	EXPECT_EQ (_tc_ValueBase->kind (), TCKind::tk_value);
	EXPECT_EQ (_tc_ValueBase->id (), CORBA_REPOSITORY_ID ("ValueBase"));
	EXPECT_EQ (_tc_ValueBase->name (), "ValueBase");
}

TEST_F (TestORB, ORBInit)
{
	int argc = 0;
	ORB::
#ifdef LEGACY_CORBA_CPP
		_var_type
#else
		_ref_type
#endif
		orb = ORB_init (argc, nullptr, nullptr);
}

}
