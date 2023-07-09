/*
* Nirvana test suite.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <Nirvana/Nirvana.h>
#include <gtest/gtest.h>
#include "IDL/Test_I1.h"
#include "IDL/Test_I2.h"
#include "IDL/Test_V3.h"
#include "IDL/RecursiveStruct.h"
#include <I2_factory_dynamic.h>
#include <I2_factory_sysdomain.h>
#include <signal.h>

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
	, Nirvana::Static <I2_factory_sysdomain> // 1
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
	A1_var out, inout (A1::_duplicate (p));
	A1_var ret = p->abstract_op (p, out, inout);
	Object_var obj;
#else
	A1::_ref_type out, inout (p);
	A1::_ref_type ret = p->abstract_op (p, out, inout);
	Object::_ref_type obj;
#endif
	ASSERT_TRUE (out);
	obj = out->_to_object ();
	EXPECT_TRUE (obj);
	EXPECT_TRUE (p->_is_equivalent (obj));
	ASSERT_TRUE (inout);
	obj = inout->_to_object ();
	EXPECT_TRUE (obj);
	EXPECT_TRUE (p->_is_equivalent (obj));
	ASSERT_TRUE (ret);
	obj = ret->_to_object ();
	EXPECT_TRUE (obj);
	EXPECT_TRUE (p->_is_equivalent (obj));
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

void test_type_code (I2::_ptr_type p, TypeCode::_ptr_type tc)
{
	TC out, inout;
	inout = assign_tc (tc);
	TC ret = p->type_code_op (tc, out, inout);
	ASSERT_TRUE (ret);
	EXPECT_TRUE (ret->equal (tc));
	ASSERT_TRUE (out);
	EXPECT_TRUE (out->equal (tc));
	ASSERT_TRUE (inout);
	EXPECT_TRUE (inout->equal (tc));
}

TYPED_TEST (TestORB_I2, TypeCode)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

	// Primitive
	test_type_code (p, _tc_long);

	// Bounded string
	TC tc (_tc_ShortString->content_type ());
	test_type_code (p, tc);

	// Enum
	test_type_code (p, _tc_MyEnum);

	// Exception
	test_type_code (p, _tc_MyException);

	// Struct
	test_type_code (p, _tc_MyStruct);

	// Sequence
	tc = _tc_SeqLong->content_type ();
	test_type_code (p, tc);

	// Recursive
	test_type_code (p, _tc_RecursiveStruct1);
}

TYPED_TEST (TestORB_I2, Stringify)
{
	I2_ref p = TestORB_I2 <TypeParam>::incarnate ();

#ifdef LEGACY_CORBA_CPP
	ORB_var
#else
	ORB::_ref_type
#endif
		orb = p->_get_ORB ();

	ASSERT_TRUE (orb);

#ifdef LEGACY_CORBA_CPP
	String_var
#else
	IDL::String
#endif
		str = orb->object_to_string (p);

#ifdef LEGACY_CORBA_CPP
	Object_var
#else
	Object::_ref_type
#endif
		obj = orb->string_to_object (str);

	I2_ref p1 = I2::_narrow (obj);
	ASSERT_TRUE (p1);
	EXPECT_TRUE (p1->_is_equivalent (p));
	EXPECT_TRUE (p->_is_equivalent (p1));
}

}
