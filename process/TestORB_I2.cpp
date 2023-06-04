#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include "IDL/Test_I1.h"
#include "IDL/Test_I2.h"
#include "IDL/Test_V3.h"
#include "IDL/RecursiveStruct.h"
#include <I2_factory_dynamic.h>

using namespace CORBA;
using namespace Test;

namespace TestORB {

const long MAGIC_CONST = 1963;

#ifdef LEGACY_CORBA_CPP
typedef I2_var I2_ref;
#else
typedef I2::_ref_type I2_ref;
#endif

// The fixture for testing complex interface.

typedef ::testing::Types <Nirvana::Static <I2_factory_dynamic> // 0
//	, Nirvana::Static <I2_factory_sysdomain> // 5
> ServantTypesI2;

template <class Factory>
class TestORB_I2 :
	public ::testing::Test
{
protected:
	TestORB_I2 ()
	{
	}

	virtual ~TestORB_I2 ()
	{
	}

	static I2_ref incarnate (I2_factory::_ptr_type factory)
	{
		return factory->create (MAGIC_CONST);
	}

	static I2_ref incarnate (I2::_ptr_type obj)
	{
#ifdef LEGACY_CORBA_CPP
		return I2::_duplicate (obj);
#else
		return obj;
#endif
	}

	static I2_ref incarnate ()
	{
		return incarnate (Factory::ptr ());
	}

};

TYPED_TEST_SUITE (TestORB_I2, ServantTypesI2);

TYPED_TEST (TestORB_I2, Signal)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

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

TYPED_TEST (TestORB_I2, Abstract)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

#ifdef LEGACY_CORBA_CPP
	A1_var out, inout (I2::_duplicate (p));
	A1_var ret = p->abstract_op (p, out, inout);
#else
	A1::_ref_type out, inout (p);
	A1::_ref_type ret = p->abstract_op (p, out, inout);
#endif
	ASSERT_TRUE (out);
	EXPECT_TRUE (p->_is_equivalent (out->_to_object ()));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (p->_is_equivalent (inout->_to_object ()));
	ASSERT_TRUE (ret);
	EXPECT_TRUE (p->_is_equivalent (ret->_to_object ()));
}

TYPED_TEST (TestORB_I2, ValueType)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

#ifdef LEGACY_CORBA_CPP
	V2_var v = V2_factory::_factory->create ();
	v->val_v2 (1234);
	{
		V1_var out, inout (V1::_duplicate (v));
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

	A1_var out, inout (A1::_duplicate (v));
	A1_var ret = p->abstract_op (v, out, inout);
	V2_ptr v2 = V2::_downcast (ret->_to_value ());
	EXPECT_TRUE (v2);
#else
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
#endif
}

TYPED_TEST (TestORB_I2, ValueBox)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();
#ifdef LEGACY_CORBA_CPP
	StringValue_var out = new StringValue ("this text will be lost"),
		inout = new StringValue ("inout string");
	StringValue_var in = new StringValue ("in string");
	StringValue_var ret = p->box_op (in, out, inout);
	EXPECT_EQ (ret->_value (), "inout string");
	EXPECT_EQ (out->_value (), "in string");
	EXPECT_EQ (inout->_value (), "in string");
#else
	StringValue::_ref_type out = make_reference <StringValue> ("this text will be lost"),
		inout = make_reference <StringValue> ("inout string");
	StringValue::_ref_type in = make_reference <StringValue> ("in string");
	StringValue::_ref_type ret = p->box_op (in, out, inout);
	EXPECT_EQ (ret->_value (), "inout string");
	EXPECT_EQ (out->_value (), "in string");
	EXPECT_EQ (inout->_value (), "in string");
#endif
}

TYPED_TEST (TestORB_I2, Union)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

	U in, out, inout;
	out.z ("this text will be lost");
	inout.z ("inout string");
	in.z ("in string");
	U ret = p->union_op (in, out, inout);
	EXPECT_EQ (ret.z (), "inout string");
	EXPECT_EQ (out.z (), "in string");
	EXPECT_EQ (inout.z (), "in string");
}

typedef TypeCode::
#ifdef LEGACY_CORBA_CPP
_var_type
#else
_ref_type
#endif
TC;

#ifdef LEGACY_CORBA_CPP
inline TC assign_tc (TypeCode_ptr tc)
{
	return TypeCode::_duplicate (tc);
}
#else
inline TC assign_tc (TypeCode::_ptr_type tc)
{
	return tc;
}
#endif

TYPED_TEST (TestORB_I2, TypeCode)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

	// Primitive
	TC out, inout;
	inout = assign_tc (_tc_long);
	TC ret = p->type_code_op (_tc_long, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_long));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_long));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_long));

	// Exception
	inout = assign_tc (_tc_MyException);
	ret = p->type_code_op (_tc_MyException, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_MyException));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_MyException));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_MyException));

	// Struct
	inout = assign_tc (_tc_MyStruct);
	ret = p->type_code_op (_tc_MyStruct, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_MyStruct));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_MyStruct));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_MyStruct));

	// Enum
	inout = assign_tc (_tc_MyEnum);
	ret = p->type_code_op (_tc_MyEnum, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_MyEnum));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_MyEnum));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_MyEnum));

	// Sequence
	inout = assign_tc (_tc_SeqLong);
	ret = p->type_code_op (_tc_SeqLong, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_SeqLong));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_SeqLong));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_SeqLong));

	// Bounded string
	inout = assign_tc (_tc_ShortString);
	ret = p->type_code_op (_tc_ShortString, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_ShortString));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_ShortString));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_ShortString));

	// Recursive
	inout = assign_tc (_tc_RecursiveStruct1);
	ret = p->type_code_op (_tc_RecursiveStruct1, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (_tc_RecursiveStruct1));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (_tc_RecursiveStruct1));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (_tc_RecursiveStruct1));
}

}
