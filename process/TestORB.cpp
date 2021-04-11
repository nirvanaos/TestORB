#include <Test_I3.h>
#include <gtest/gtest.h>
#include "TestORB.h"

using namespace std;
using namespace Test;
using namespace CORBA;

/*
void must_not_compile1 (const Test::I3_var& var)
{
	Test::I1_var i1 = var; // Implicit cast between var must cause a compilation error
}

bool must_not_compile2 (const Test::I3_ptr p1, const Test::I3_ptr p2)
{
	return p1 == p2;
}

bool must_not_compile3 (const Test::I3_var p1, const Test::I3_var p2)
{
	return p1 == p2;
}
*/
namespace TestORB {

class TestORB : public ::testing::Test
{
public:
};

TEST_F (TestORB, RepositoryId)
{
	EXPECT_TRUE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.0"));
	EXPECT_TRUE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.1", "IDL:aaa/bbb/type:1.0"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/type:1.1"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "IDL:aaa/bbb/other:1.0"));
	EXPECT_FALSE (CORBA::Nirvana::RepositoryId::compatible ("IDL:aaa/bbb/type:1.0", "aaa/bbb/type:1.0"));

	EXPECT_LT (CORBA::Nirvana::RepositoryId::compare (::Test::I1::repository_id_, size (::Test::I1::repository_id_) - 1,
		::Test::I3::repository_id_, size (::Test::I3::repository_id_) - 1), 0);
}

TEST_F (TestORB, SystemException)
{
	CORBA::NO_MEMORY nm;
	EXPECT_THROW (nm._raise (), CORBA::NO_MEMORY);
}

TEST_F (TestORB, UserException)
{
	::Test::MyException e;
	EXPECT_THROW (e._raise (), ::Test::MyException);
}

TEST_F (TestORB, CORBA_Environment)
{
	CORBA::Environment_ptr env;
	CORBA::ORB::create_environment (env);
	CORBA::Nirvana::Interface* eb = env;
	CORBA::Nirvana::set_exception (eb, CORBA::NO_MEMORY ());
	const CORBA::Exception* ex = env->exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	env->clear ();
	CORBA::Environment_var ev = env;
	CORBA::ORB::create_environment (ev);
	EXPECT_FALSE (ev->exception ());
}

TEST_F (TestORB, Environment)
{
	CORBA::Nirvana::Environment ne;
	set_exception (&ne, CORBA::NO_MEMORY ());
	const CORBA::Exception* ex = ne.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");
	CORBA::Nirvana::Environment ne1 (move (ne));
	EXPECT_FALSE (ne.exception ());
	ex = ne1.exception ();
	ASSERT_TRUE (ex);
	EXPECT_STREQ (ex->_name (), "NO_MEMORY");

	CORBA::Nirvana::EnvironmentEx <::Test::MyException> nex;
	CORBA::Nirvana::set_exception (&nex, ::Test::MyException ());
	CORBA::Nirvana::Environment ne2 (move (nex));
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
	EXPECT_EQ (_tc_MyStruct->member_count (), 2);
	EXPECT_EQ (_tc_MyStruct->member_name (0), "ws_member");
	EXPECT_EQ (_tc_MyStruct->member_name (1), "l_member");
	EXPECT_EQ (_tc_MyStruct->member_type (0)->kind (), TCKind::tk_wstring);
	EXPECT_EQ (_tc_MyStruct->member_type (1)->kind (), TCKind::tk_long);

	EXPECT_EQ (_tc_SeqLong->kind (), TCKind::tk_alias);
	EXPECT_EQ (_tc_SeqLong->id (), "IDL:Test/SeqLong:1.0");
	EXPECT_EQ (_tc_SeqLong->name (), "SeqLong");
	
	TypeCode_var seq = _tc_SeqLong->content_type ();
	EXPECT_EQ (seq->kind (), TCKind::tk_sequence);
	EXPECT_EQ (seq->length (), 0);

	EXPECT_FALSE (_tc_SeqLong->equal (seq));
	EXPECT_TRUE (_tc_SeqLong->equivalent (seq));
	EXPECT_TRUE (_tc_SeqLong->equal (_tc_SeqLong));

	TypeCode_var l = seq->content_type ();
	EXPECT_EQ (l->kind (), TCKind::tk_long);
}

}
