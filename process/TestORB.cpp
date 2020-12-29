#include <Test_I3.h>
#include <gtest/gtest.h>
#include "TestORB.h"

using namespace std;
using namespace Test;

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

}
