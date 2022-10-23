#include "TestORB.h"
#include "IDL/Test_I1.h"
#include "IDL/Test_I3.h"
#include "IDL/Test_V1.h"
#include "IDL/Test_V3.h"
#include "IDL/RecursiveStruct.h"
#include <I2_factory_impl.h>
#include <I1_static.h>

using namespace std;
using namespace CORBA;
using namespace Test;

namespace TestORB {

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
}

TEST_F (TestORB, TypeCodeRecursive)
{
	EXPECT_TRUE (_tc_RecursiveStruct1->equal (_tc_RecursiveStruct1));
	EXPECT_TRUE (_tc_RecursiveStruct1->equivalent (_tc_RecursiveStruct1));

	typedef TypeCode::
#ifdef LEGACY_CORBA_CPP
		_var_type
#else
		_ref_type
#endif
		TC;

	{
		TC tc_struct;
		{
			TC tc_rec = g_ORB->create_recursive_tc ("");
			TC tc_seq = g_ORB->create_sequence_tc (0, tc_rec);
			StructMemberSeq members;
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (_tc_long);
#else
			members.back ().type = _tc_long;
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

	EXPECT_TRUE (_tc_V1->equal (_tc_V1));
	EXPECT_TRUE (_tc_V1->equivalent (_tc_V1));

	{
		TC tc_v1;
		{
			TC tc_rec = g_ORB->create_recursive_tc (Internal::RepIdOf <V1>::id);
			ValueMemberSeq members;
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (_tc_short);
#else
			members.back ().type = _tc_short;
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (_tc_long);
#else
			members.back ().type = _tc_long;
#endif
			members.emplace_back ();
#ifndef LEGACY_CORBA_CPP
			members.back ().type (_tc_string);
#else
			members.back ().type = _tc_string;
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

TEST_F (TestORB, Factory)
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

TEST_F (TestORB, Any)
{
	Any a;
	{
		string s = "test string";
		a <<= s;
		EXPECT_FALSE (s.empty ());
		const string* ps = nullptr;
		EXPECT_TRUE (a >>= ps);
		EXPECT_TRUE (ps);
		EXPECT_EQ (s, *ps);
		string s1;
		EXPECT_TRUE (a >>= s1);
		EXPECT_EQ (s, s1);
		a <<= move (s);
		EXPECT_TRUE (s.empty ());
		Long l;
		EXPECT_FALSE (a >>= l);
	}
	{
		vector <string> vs = { "test1",  "test2",  "test3" };
		a <<= vs;
		EXPECT_FALSE (vs.empty ());
		vector <string> vs1;
		EXPECT_TRUE (a >>= vs1);
		EXPECT_EQ (vs, vs1);
		a <<= move (vs);
		EXPECT_TRUE (vs.empty ());
		vector <Long> vl;
		EXPECT_FALSE (a >>= vl);
	}
	{
		array <string, 3> as = { "test1",  "test2",  "test3" };
		a <<= as;
		array <string, 3> as1;
		EXPECT_TRUE (a >>= as1);
		EXPECT_EQ (as, as1);
		a <<= move (as);
		EXPECT_TRUE (as [0].empty ());
		array <string, 4> as4;
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

}
