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
#include "Test_I1.h"
#include "Test_I2.h"
#include "Test_V3.h"
#include "RecursiveStruct.h"
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

class TestORB_I2 :
	public ::testing::TestWithParam <I2_ref>
{
protected:
	TestORB_I2 ()
	{}

	virtual ~TestORB_I2 ()
	{}
};

INSTANTIATE_TEST_SUITE_P (ServantTypesI2, TestORB_I2, testing::Values (
	I2_factory_dynamic->create (MAGIC_CONST),
	I2_factory_sysdomain->create (MAGIC_CONST),
	I2_factory_V3->create (MAGIC_CONST)
));

TEST_P (TestORB_I2, Signal)
{
	I2_ref p = GetParam ();

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

TEST_P (TestORB_I2, Abstract)
{
	I2_ref p = GetParam ();

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

TEST_P (TestORB_I2, ValueType)
{
	I2_ref p = GetParam ();
#ifdef LEGACY_CORBA_CPP
	V2_var v = V2::_factory->create ();

	ULong rcnt = v->_refcount_value ();
	EXPECT_EQ (rcnt, 1);

	v->val_v2 (1234);

	{
		V1_var out, inout (V1::_duplicate (v));
		V1_var ret = p->value_op (v, out, inout);
		V2_ptr v2 = V2::_downcast (ret);
		ASSERT_TRUE (v2);
		EXPECT_EQ (v2->val_v2 (), v->val_v2 ());
		rcnt = ret->_refcount_value ();
		EXPECT_EQ (rcnt, 1);
		rcnt = out->_refcount_value ();
		EXPECT_EQ (rcnt, 2);
	}

	rcnt = v->_refcount_value ();
	EXPECT_EQ (rcnt, 1);

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

	rcnt = v->_refcount_value ();
	EXPECT_EQ (rcnt, 1);

	{
		A1_var out, inout (A1::_duplicate (v));
		A1_var ret = p->abstract_op (v, out, inout);
		ValueBase_var vb = ret->_to_value ();
		V2_ptr v2 = V2::_downcast (vb);
		EXPECT_TRUE (v2);
	}
	rcnt = v->_refcount_value ();
	EXPECT_EQ (rcnt, 1);

#else
	V2::_ref_type v = V2::_factory->create ();
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

TEST_P (TestORB_I2, ValueBox)
{
	I2_ref p = GetParam ();
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

TEST_P (TestORB_I2, Union)
{
	I2_ref p = GetParam ();

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

TEST_P (TestORB_I2, TypeCode)
{
	I2_ref p = GetParam ();

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

TEST_P (TestORB_I2, Stringify)
{
	I2_ref p = GetParam ();

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

TEST_P (TestORB_I2, Oneway)
{
	I2_ref p = GetParam ();
	p->oneway_op (1);
}

TEST_P (TestORB_I2, AMI)
{
	I2_ref p = GetParam ();
#ifdef LEGACY_CORBA_CPP
	typedef AMI_I2Poller_var
#else
	typedef AMI_I2Poller::_ref_type
#endif
	Poller;

	Poller poller = p->sendp_op2 (1);
#ifdef LEGACY_CORBA_CPP
	String_var
#else
	std::string
#endif
		op_name = poller->operation_name ();
	EXPECT_EQ (op_name, "op2");
	Long ret;
	poller->op2 (std::numeric_limits <uint32_t>::max (), ret);
	EXPECT_EQ (ret, 2 * MAGIC_CONST + 1);
}

}
