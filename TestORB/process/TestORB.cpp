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
#include "TestORB.h"
#include "Test_I1.h"
#include "Test_I3.h"
#include "Test_V1.h"
#include "Test_V3.h"
#include "RecursiveStruct.h"
#include <I1_static.h>
#include "Test_AVT.h"

using namespace CORBA;
using namespace Test;

namespace TestORB {

typedef TypeCode::
#ifdef LEGACY_CORBA_CPP
_var_type
#else
_ref_type
#endif
TC;

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
	CORBA::Internal::Environment ne1 (std::move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");

	CORBA::Internal::EnvironmentEx <::Test::MyException> nex;
	::Test::MyException my_ex;
	CORBA::Internal::set_exception (&nex, my_ex);
	CORBA::Internal::Environment ne2 (std::move (nex));
	ex = ne2.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), ::Test::MyException::__name ());
}

TEST_F (TestORB, TypeCode)
{
	EXPECT_EQ (_tc_Object->id (), "IDL:omg.org/CORBA/Object:1.0");
	EXPECT_EQ (_tc_Object->kind (), TCKind::tk_objref);
	EXPECT_EQ (_tc_Object->name (), "Object");

	EXPECT_EQ (_tc_ValueBase->id (), "IDL:omg.org/CORBA/ValueBase:1.0");
	EXPECT_EQ (_tc_ValueBase->kind (), TCKind::tk_value);
	EXPECT_EQ (_tc_ValueBase->name (), "ValueBase");
	EXPECT_EQ (_tc_ValueBase->member_count (), 0);
	EXPECT_EQ (_tc_ValueBase->type_modifier (), VM_NONE);
	EXPECT_TRUE (is_nil (_tc_ValueBase->concrete_base_type ()));

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

	EXPECT_EQ (_tc_MyEnum->kind (), TCKind::tk_enum);
	EXPECT_EQ (_tc_MyEnum->id (), "IDL:Test/MyEnum:1.0");
	EXPECT_EQ (_tc_MyEnum->name (), "MyEnum");
	EXPECT_EQ (_tc_MyEnum->member_count (), 3u);
	EXPECT_EQ (_tc_MyEnum->member_name (0), "a");
	EXPECT_EQ (_tc_MyEnum->member_name (1), "b");
	EXPECT_EQ (_tc_MyEnum->member_name (2), "c");

	TC compact = _tc_MyEnum->get_compact_typecode ();
	EXPECT_TRUE (_tc_MyEnum->equivalent (compact));
	EXPECT_EQ (compact->name (), "");
	EXPECT_EQ (compact->member_name (0), "");
	EXPECT_EQ (compact->member_name (1), "");
	EXPECT_EQ (compact->member_name (2), "");
	compact = TypeCode::_nil ();

	EXPECT_EQ (_tc_SeqLong->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_SeqLong->id (), "IDL:Test/SeqLong:1.0");
	EXPECT_EQ (_tc_SeqLong->name (), "SeqLong");

	TC cont = _tc_SeqLong->content_type ();

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

	EXPECT_EQ (_tc_U->kind (), TCKind::tk_union);
	EXPECT_EQ (_tc_U->id (), "IDL:U:1.0");
	EXPECT_EQ (_tc_U->name (), "U");
	EXPECT_EQ (_tc_U->member_count (), 4);
	EXPECT_TRUE (_tc_U->discriminator_type ()->equal (_tc_long));
	EXPECT_EQ (_tc_U->member_name (0), "x");
	EXPECT_EQ (_tc_U->member_name (1), "z");
	EXPECT_EQ (_tc_U->member_name (2), "w");
	EXPECT_EQ (_tc_U->member_name (3), "obj");
	EXPECT_EQ (_tc_U->default_index (), 3);
#ifndef LEGACY_CORBA_CPP
	Any
#else
	Any_var
#endif
		a = _tc_U->member_label (0);
	Long l;
	EXPECT_TRUE (a >>= l);
	EXPECT_EQ (l, 1);
	a = _tc_U->member_label (1);
	EXPECT_TRUE (a >>= l);
	EXPECT_EQ (l, 2);
	a = _tc_U->member_label (2);
	EXPECT_TRUE (a >>= l);
	EXPECT_EQ (l, 3);

	a = _tc_U->member_label (3);
	EXPECT_FALSE (a >>= l);
	Octet o;
	EXPECT_TRUE (a >>= Any::to_octet (o));
	EXPECT_EQ (o, 0);

	EXPECT_EQ (_tc_StructFixLen->member_name (2), "f");
	cont = _tc_StructFixLen->member_type (2);
	EXPECT_EQ (cont->kind (), TCKind::tk_fixed);
	EXPECT_EQ (cont->fixed_digits (), 4);
	EXPECT_EQ (cont->fixed_scale (), 3);

	{
		TC compact = _tc_I1->get_compact_typecode ();
		EXPECT_TRUE (compact->equivalent (_tc_I1));
		EXPECT_TRUE (_tc_I1->equivalent (compact));
		EXPECT_FALSE (compact->equal (_tc_I1));
		EXPECT_FALSE (_tc_I1->equal (compact));
	}

	{
		TC compact = _tc_AVT->get_compact_typecode ();
		EXPECT_TRUE (compact->equivalent (_tc_AVT));
		EXPECT_TRUE (_tc_AVT->equivalent (compact));
		EXPECT_FALSE (compact->equal (_tc_AVT));
		EXPECT_FALSE (_tc_AVT->equal (compact));
	}
}

TEST_F (TestORB, TypeCodeRecursive)
{
	EXPECT_TRUE (_tc_RecursiveStruct1->equal (_tc_RecursiveStruct1));
	EXPECT_TRUE (_tc_RecursiveStruct1->equivalent (_tc_RecursiveStruct1));

	{
		TC tc_struct;
		{
			TC tc_rec = g_ORB->create_recursive_tc ("");
			TC tc_seq = g_ORB->create_sequence_tc (0, tc_rec);
			StructMemberSeq members;
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (TC (_tc_long));
#else
			members.back ().type = TypeCode::_duplicate (_tc_long);
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (tc_seq);
#else
			members.back ().type = tc_seq;
#endif
			tc_struct = g_ORB->create_struct_tc ("", "", members);
		}
		EXPECT_TRUE (_tc_RecursiveStruct1->equivalent (tc_struct));
		EXPECT_TRUE (tc_struct->equivalent (_tc_RecursiveStruct1));
	}

	{
		TC compact = _tc_RecursiveStruct1->get_compact_typecode ();
		EXPECT_TRUE (_tc_RecursiveStruct1->equivalent (compact));
		EXPECT_TRUE (compact->equivalent (_tc_RecursiveStruct1));
	}

	EXPECT_TRUE (_tc_V1->equal (_tc_V1));
	EXPECT_TRUE (_tc_V1->equivalent (_tc_V1));

	{
		TC tc_v1;
		{
			TC tc_rec = g_ORB->create_recursive_tc (Internal::RepIdOf <V1>::id);
			ValueMemberSeq members;
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (TC (_tc_short));
#else
			members.back ().type = TypeCode::_duplicate (_tc_short);
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (TC (_tc_long));
#else
			members.back ().type = TypeCode::_duplicate (_tc_long);
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (TC (_tc_string));
#else
			members.back ().type = TypeCode::_duplicate (_tc_string);
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (tc_rec);
#else
			members.back ().type = tc_rec;
#endif
			tc_v1 = g_ORB->create_value_tc (Internal::RepIdOf <V1>::id, "", VM_NONE, nullptr, members);
		}
		EXPECT_TRUE (_tc_V1->equivalent (tc_v1));
		EXPECT_TRUE (tc_v1->equivalent (_tc_V1));
	}
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

TEST_F (TestORB, V3)
{
	// Create V3 value with interface I2
	I2::
#ifdef LEGACY_CORBA_CPP
		_var_type
#else
		_ref_type
#endif
		p = I2_factory_V3->create (MAGIC_CONST);
	EXPECT_EQ (p->op2 (1), 2 * MAGIC_CONST + 1);

	// Legacy process can not create value with concrete interface support
	EXPECT_THROW (V3::_factory->create (MAGIC_CONST), NO_PERMISSION);
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

TEST_F (TestORB, FactoryBase)
{
	ValueFactoryBase::
#ifndef LEGACY_CORBA_CPP
		_ref_type
#else
		_var_type
#endif
		f;

	f = g_ORB->lookup_value_factory ("IDL:Test/V1:1.0");
	EXPECT_TRUE (f);
	f = g_ORB->lookup_value_factory ("IDL:Test/StringValue:1.0");
	EXPECT_TRUE (f);
}

TEST_F (TestORB, Factory)
{
#ifndef LEGACY_CORBA_CPP
	V1::_ref_type
#else
	V1_var
#endif
		v1 = V1::_factory->create (1, "test");
}

TEST_F (TestORB, Any)
{
	Any a;
	{
		std::string s = "test string";
		a <<= s;
		EXPECT_FALSE (s.empty ());
		const std::string* ps = nullptr;
		EXPECT_TRUE (a >>= ps);
		EXPECT_TRUE (ps);
		EXPECT_EQ (s, *ps);
		std::string s1;
		EXPECT_TRUE (a >>= s1);
		EXPECT_EQ (s, s1);
		a <<= std::move (s);
		EXPECT_TRUE (s.empty ());
		Long l;
		EXPECT_FALSE (a >>= l);
	}
	{
		std::vector <std::string> vs = { "test1",  "test2",  "test3" };
		a <<= vs;
		EXPECT_FALSE (vs.empty ());
		std::vector <std::string> vs1;
		EXPECT_TRUE (a >>= vs1);
		EXPECT_EQ (vs, vs1);
		a <<= std::move (vs);
		EXPECT_TRUE (vs.empty ());
		std::vector <Long> vl;
		EXPECT_FALSE (a >>= vl);
	}
	{
		std::array <std::string, 3> as = { "test1",  "test2",  "test3" };
		a <<= as;
		std::array <std::string, 3> as1;
		EXPECT_TRUE (a >>= as1);
		EXPECT_EQ (as, as1);
		a <<= std::move (as);
		EXPECT_TRUE (as [0].empty ());
		std::array <std::string, 4> as4;
		EXPECT_FALSE (a >>= as4);
	}
	{
		MyEnum en = MyEnum::b;
		a <<= en;
		MyEnum en1;
		EXPECT_TRUE (a >>= en1);
		EXPECT_EQ (en, en1);
	}
}

#ifndef LEGACY_CORBA_CPP

TEST_F (TestORB, Swap)
{
	I1::_ref_type r1, r2;
	swap (r1, r2);

	MyStruct ms1, ms2;
	swap (ms1, ms2);
}

#endif

TEST_F (TestORB, AbstractValueStatic)
{
	long result = abstract_value_type->sum (1, 2);
	EXPECT_EQ (result, 3);
}

}
