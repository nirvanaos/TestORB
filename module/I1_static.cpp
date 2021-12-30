#include <CORBA/Server.h>
#include "I1_static.h"
#include "ImplI1.h"
#include "Test_I1_s.h"
#include "TestORB.h"

// Static implementation

namespace Test {

class I1_static :
	public ::CORBA::servant_traits <I1>::ServantStatic <I1_static>,
	public ImplI1
{
public:
	static int32_t op1 (CORBA::Long p1)
	{
		return p1 + TestORB::MAGIC_CONST;
	}

	static int32_t addendum ()
	{
		return TestORB::MAGIC_CONST;
	}

	void addendum (int32_t)
	{
		throw CORBA::NO_IMPLEMENT ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_static)
