#include "I1_static.h"
#include "ImplI1.h"
#include "Test_I1_s.h"
#include "TestORB.h"

// Static implementation

namespace Test {

class I1_static :
	public ::CORBA::Nirvana::ServantStatic <I1_static, I1>,
	public ImplI1
{
public:
	static CORBA::Long op1 (CORBA::Long p1)
	{
		return p1 + TestORB::MAGIC_CONST;
	}

	static void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_static)
