#include "TestORB_I1.h"
#include <I1_static.h>
#include <I3_static.h>
#include "I3_factory_dynamic.h"
#include "I3_factory_portable.h"
#include "I3_factory_tied.h"
#include "IDL/Test_V3.h"
#include <signal.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {
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

	{ // Value type
		V2_var v = V2_factory::_factory->create ();
		v->val_v2 (1234);
		{
			V1_var out, inout = V1::_duplicate (v);
			V1_var ret = V1::_duplicate (p->value_op (v, out, inout));
			V2_var v2 = V2::_duplicate (V2::_downcast (ret));
			ASSERT_TRUE (v2);
			EXPECT_EQ (v2->val_v2 (), v->val_v2 ());
		}

		{ // Abstract base
			A1_ptr a1 = v;
			AbstractBase_ptr ab = a1;
			ASSERT_TRUE (ab);
			EXPECT_FALSE (ab->_to_object ());
			ValueBase_var vb = ab->_to_value ();
			ASSERT_TRUE (vb);
			V1_ptr v1 = V1::_downcast (vb);
			EXPECT_TRUE (v1);
			V2_ptr v2 = V2::_downcast (vb);
			EXPECT_TRUE (v2);
		}

		A1_var out, inout (v);
		A1_var ret = p->abstract_op (v, out, inout);
		V2_ptr v2 = V2::_downcast (ret->_to_value ());
		EXPECT_TRUE (v2);

	}

	{ // Fixed-length struct
		StructFixLen in, out, inout;
		in.f = fixed_const;
		StructFixLen ret = p->fix_struct_op (in, out, inout);
	}

	{ // Array
		FixStructArray out, inout;
		FixStructArray ret = p->fix_struct_array_op (FixStructArray (), out, inout);
	}

	{ // Value box
		StringValue_var out = new StringValue ("this text will be lost"),
			inout = new StringValue ("inout string");
		StringValue_var in = new StringValue ("in string");
		StringValue_var ret = p->box_op (in, out, inout);
		EXPECT_EQ (ret->_value (), "inout string");
		EXPECT_EQ (out->_value (), "in string");
		EXPECT_EQ (inout->_value (), "in string");
	}

	{ // Union
		U in, out, inout;
		out.z ("this text will be lost");
		inout.z ("inout string");
		in.z ("in string");
		U ret = p->union_op (in, out, inout);
		EXPECT_EQ (ret.z (), "inout string");
		EXPECT_EQ (out.z (), "in string");
		EXPECT_EQ (inout.z (), "in string");
	}

	{ // boolean
		bool out, inout = true;
		bool ret = p->bool_op (false, out, inout);
		EXPECT_TRUE (ret);
		EXPECT_FALSE (out);
		EXPECT_FALSE (inout);
	}

	{ // Fixed
		Fixed_8_2 out (1.5), inout (3.5);
		Fixed_8_2 ret = p->fixed_op (Fixed_8_2 (const_neg_exp), out, inout);
		EXPECT_EQ (ret, Fixed_8_2 (3.5));
		EXPECT_EQ (out, Fixed_8_2 (const_neg_exp));
		EXPECT_EQ (inout, Fixed_8_2 (const_neg_exp));
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

	{ // Fixed-length struct
		StructFixLen in, out, inout;
		in.f (fixed_const);
		StructFixLen ret = p->fix_struct_op (in, out, inout);
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

	{ // Union
		U in, out, inout;
		out.z ("this text will be lost");
		inout.z ("inout string");
		in.z ("in string");
		U ret = p->union_op (in, out, inout);
		EXPECT_EQ (ret.z (), "inout string");
		EXPECT_EQ (out.z (), "in string");
		EXPECT_EQ (inout.z (), "in string");
	}

	{ // boolean
		bool out, inout = true;
		bool ret = p->bool_op (false, out, inout);
		EXPECT_TRUE (ret);
		EXPECT_FALSE (out);
		EXPECT_FALSE (inout);
	}

	{ // Fixed
		Fixed_8_2 out (1.5), inout (3.5);
		Fixed_8_2 ret = p->fixed_op (Fixed (const_neg_exp), out, inout);
		EXPECT_EQ (ret, Fixed_8_2 (3.5));
		EXPECT_EQ (out, Fixed_8_2 (const_neg_exp));
		EXPECT_EQ (inout, Fixed_8_2 (const_neg_exp));
	}
}

#endif

}
