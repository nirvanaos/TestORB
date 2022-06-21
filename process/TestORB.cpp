#include <Nirvana/Nirvana.h>
#include "I1_static.h"
#include "I1_factory_dynamic.h"
#include "I1_factory_portable.h"
#include "I1_factory_tied.h"
#include "I3_static.h"
#include "I3_factory_dynamic.h"
#include "I3_factory_portable.h"
#include "I3_factory_tied.h"
#include "IDL/Test_V3.h"
#include "I2_factory_impl.h"
#include "TestORB.h"
#include <gtest/gtest.h>
#include <signal.h>

using namespace std;

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

	{
		MyStruct out (L"out", 2, p), inout (L"inout", 3, p);
		MyStruct ret = p->struct_op (MyStruct (L"in", 1, p), out, inout);
		EXPECT_EQ (ret.ws_member (), L"inout");
		EXPECT_EQ (out.ws_member (), L"in");
		EXPECT_EQ (inout.ws_member (), L"in");
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
	, Nirvana::Static <I1_factory_portable>
#endif
#ifndef TEST_NO_STATIC
	, Nirvana::Static <I1_static>
#endif
#ifndef TEST_NO_TIED
	, Nirvana::Static <I1_factory_tied>
	, Nirvana::Static <I1_tied_derived>
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
		ASSERT_TRUE (p->_is_equivalent (p3));
		A1_ptr a1 = A1::_narrow (obj);
		ASSERT_TRUE (a1);
		release (p1);
		release (p2);
		release (p3);
		release (a1);
	}

	{ // Abstract interface
		A1_ptr a1 = p;
		AbstractBase_ptr ab = a1;
		ASSERT_TRUE (ab);
		EXPECT_FALSE (ab->_to_value ());
		Object::_ptr_type obj = ab->_to_object ();
		ASSERT_TRUE (obj);
		I3::_ptr_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
		release (obj);
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
	EXPECT_EQ (p->aop (4), 4 - MAGIC_CONST);

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
		A1::_ref_type out, inout (p);
		A1::_ref_type ret = p->abstract_op (p, out, inout);
		EXPECT_TRUE (p->_is_equivalent (out->_to_object ()));
		EXPECT_TRUE (p->_is_equivalent (inout->_to_object ()));
		EXPECT_TRUE (p->_is_equivalent (ret->_to_object ()));
	}

	{
		Object::_ref_type obj = p;
		ASSERT_TRUE (obj);
		I1::_ref_type p1 = I1::_narrow (obj);
		EXPECT_TRUE (p1);
		I2::_ref_type p2 = I2::_narrow (obj);
		EXPECT_TRUE (p2);
		I3::_ref_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
		A1::_ref_type a1 = A1::_narrow (obj);
		EXPECT_TRUE (a1);
	}

	{ // Abstract interface
		A1::_ptr_type a1 = p;
		AbstractBase::_ptr_type ab = a1;
		ASSERT_TRUE (ab);
		EXPECT_FALSE (ab->_to_value ());
		Object::_ref_type obj = ab->_to_object ();
		ASSERT_TRUE (obj);
		I3::_ref_type p3 = I3::_narrow (obj);
		EXPECT_TRUE (p3);
		EXPECT_TRUE (p->_is_equivalent (p3));
	}

	{ // Value type
		V2::_ref_type v = V2_factory::_factory->create ();
		v->val_v2 (1234);
		{
			V1::_ref_type out, inout = v;
			V1::_ref_type ret = p->value_op (v, out, inout);
			V2::_ref_type v2 = V2::_downcast (ret);
			ASSERT_TRUE (v2);
			EXPECT_EQ (v2->val_v2 (), v->val_v2 ());
		}

		{ // Abstract base
			A1::_ptr_type a1 = v;
			AbstractBase::_ptr_type ab = a1;
			ASSERT_TRUE (ab);
			EXPECT_FALSE (ab->_to_object ());
			ValueBase::_ref_type vb = ab->_to_value ();
			ASSERT_TRUE (vb);
			V1::_ptr_type v1 = V1::_downcast (vb);
			EXPECT_TRUE (v1);
			V2::_ptr_type v2 = V2::_downcast (vb);
			EXPECT_TRUE (v2);
		}

		A1::_ref_type out, inout (v);
		A1::_ref_type ret = p->abstract_op (v, out, inout);
		V2::_ptr_type v2 = V2::_downcast (ret->_to_value ());
		EXPECT_TRUE (v2);

	}

	{ // Array
		FixStructArray out, inout;
		FixStructArray ret = p->fix_struct_array_op (FixStructArray (), out, inout);
	}

	{ // Value box
		StringValue::_ref_type out = make_reference <StringValue> ("this text will be lost"),
			inout = make_reference <StringValue> ("inout string");
		StringValue::_ref_type in = make_reference <StringValue> ("in string");
		StringValue::_ref_type ret = p->box_op (in, out, inout);
		EXPECT_EQ (ret->_value (), "inout string");
		EXPECT_EQ (out->_value (), "in string");
		EXPECT_EQ (inout->_value (), "in string");
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
	EXPECT_EQ (_tc_I1->name (), "I1");

	EXPECT_EQ (_tc_I3->id (), "IDL:Test/I3:1.0");
	EXPECT_EQ (_tc_I3->kind (), TCKind::tk_local_interface);
	EXPECT_EQ (_tc_I3->name (), "I3");

	EXPECT_FALSE (_tc_I1->equal (_tc_I3));
	EXPECT_FALSE (_tc_I1->equivalent (_tc_I3));

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
#ifdef LEGACY_CORBA_CPP
	{
		TypeCode::_var_type mt = _tc_MyStruct->member_type (0);
		EXPECT_EQ (mt->kind (), TCKind::tk_wstring);
		mt = _tc_MyStruct->member_type (1);
		EXPECT_EQ (mt->kind (), TCKind::tk_long);
		if (_tc_MyStruct->member_count () > 2) {
			mt = _tc_MyStruct->member_type (2);
			EXPECT_EQ (mt->kind (), TCKind::tk_objref);
		}
	}
#else
	EXPECT_EQ (_tc_MyStruct->member_type (0)->kind (), TCKind::tk_wstring);
	EXPECT_EQ (_tc_MyStruct->member_type (1)->kind (), TCKind::tk_long);
	if (_tc_MyStruct->member_count () > 2)
		EXPECT_EQ (_tc_MyStruct->member_type (2)->kind (), TCKind::tk_objref);
#endif

	EXPECT_EQ (_tc_SeqLong->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_SeqLong->id (), "IDL:Test/SeqLong:1.0");
	EXPECT_EQ (_tc_SeqLong->name (), "SeqLong");

	TypeCode::
#ifdef LEGACY_CORBA_CPP
		_var_type
#else
		_ref_type
#endif
		cont;

	cont = _tc_SeqLong->content_type ();

	EXPECT_EQ (cont->kind (), TCKind::tk_sequence);
	EXPECT_EQ (cont->length (), 0);

	EXPECT_FALSE (_tc_SeqLong->equal (cont));
	EXPECT_TRUE (_tc_SeqLong->equivalent (cont));
	EXPECT_TRUE (_tc_SeqLong->equal (_tc_SeqLong));
	EXPECT_TRUE (_tc_SeqLong->equivalent (_tc_SeqLong));

	cont = cont->content_type ();
	EXPECT_EQ (cont->kind (), TCKind::tk_long);

	EXPECT_EQ (_tc_MyAlias->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_MyAlias->id (), "IDL:Test/MyAlias:1.0");
	EXPECT_EQ (_tc_MyAlias->name (), "MyAlias");

	cont = _tc_MyAlias->content_type ();
	EXPECT_TRUE (cont->equal (_tc_SeqLong));

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

	EXPECT_EQ (_tc_A1->id (), "IDL:Test/A1:1.0");
	EXPECT_EQ (_tc_A1->kind (), TCKind::tk_abstract_interface);
	EXPECT_EQ (_tc_A1->name (), "A1");

	EXPECT_EQ (_tc_V1->id (), "IDL:Test/V1:1.0");
	EXPECT_EQ (_tc_V1->kind (), TCKind::tk_value);
	EXPECT_EQ (_tc_V1->name (), "V1");
	EXPECT_FALSE (_tc_V1->concrete_base_type ());
	EXPECT_EQ (_tc_V1->member_count (), 4);
	EXPECT_EQ (_tc_V1->member_name (0), "val1");
	EXPECT_EQ (_tc_V1->member_visibility (0), PRIVATE_MEMBER);
	EXPECT_EQ (_tc_V1->member_name (1), "val2");
	EXPECT_EQ (_tc_V1->member_visibility (1), PUBLIC_MEMBER);
	EXPECT_EQ (_tc_V1->member_name (2), "val3");
	EXPECT_EQ (_tc_V1->member_visibility (2), PRIVATE_MEMBER);
	EXPECT_EQ (_tc_V1->member_name (3), "val4");
	EXPECT_EQ (_tc_V1->member_visibility (3), PUBLIC_MEMBER);
#ifndef LEGACY_CORBA_CPP
	EXPECT_EQ (_tc_V1->member_type (0)->kind (), TCKind::tk_short);
	EXPECT_EQ (_tc_V1->member_type (1)->kind (), TCKind::tk_long);
	EXPECT_EQ (_tc_V1->member_type (2)->kind (), TCKind::tk_string);
	EXPECT_TRUE (_tc_V1->member_type (3)->equal (_tc_V1));
	EXPECT_TRUE (_tc_V2->concrete_base_type ()->equal (_tc_V1));
#endif

	EXPECT_EQ (_tc_LongArray->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_LongArray->id (), "IDL:Test/LongArray:1.0");
	EXPECT_EQ (_tc_LongArray->name (), "LongArray");

	cont = _tc_LongArray->content_type ();
	EXPECT_EQ (cont->kind (), TCKind::tk_array);
	EXPECT_EQ (cont->length (), 3);
	cont = cont->content_type ();
	EXPECT_EQ (cont->kind (), TCKind::tk_array);
	EXPECT_EQ (cont->length (), 4);
	cont = cont->content_type ();
	EXPECT_EQ (cont->kind (), TCKind::tk_long);

	EXPECT_EQ (_tc_StringValue->kind (), TCKind::tk_value_box);
	EXPECT_EQ (_tc_StringValue->id (), "IDL:Test/StringValue:1.0");
	EXPECT_EQ (_tc_StringValue->name (), "StringValue");
	cont = _tc_StringValue->content_type ();
	EXPECT_TRUE (cont->equal (_tc_string));
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

TEST_F (TestORB, I2)
{
	I2::
#ifdef LEGACY_CORBA_CPP
		_var_type
#else
		_ref_type
#endif
		p = Nirvana::Static <I2_factory_impl>::ptr ()->create (MAGIC_CONST);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);

	// Legacy process can not create value with concrete interface support
	EXPECT_THROW (V3_factory::_factory->create (MAGIC_CONST), NO_PERMISSION);
}

TEST_F (TestORB, ValueBox)
{
#ifndef LEGACY_CORBA_CPP
	StringValue::_ref_type sv = make_reference <StringValue> ("test");
#else
	StringValue_var sv = new StringValue ("test");
#endif
	EXPECT_EQ (sv->_value (), "test");
	sv->_value ().append ("1");
	EXPECT_EQ (sv->_value (), "test1");

	StringValue::_ptr_type p = sv;

	ValueBase::_ptr_type base = sv;

	p = StringValue::_downcast (base);
	EXPECT_TRUE (p);

	CORBA::Internal::Interface::_ptr_type pi (p);

	CORBA::Internal::Type <StringValue>::Var var;
}

}
