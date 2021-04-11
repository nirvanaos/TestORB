#include <CORBA/Server.h>
#include "I3_static.h"
#include "ImplI1.h"
#include "Test_I3_s.h"
#include "TestORB.h"

namespace Test {

// Static implementation

class I3_static :
	public CORBA::servant_traits <I3>::ServantStatic <I3_static>,
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

	static CORBA::Long op2 (CORBA::Long p1)
	{
		return p1 + 2 * TestORB::MAGIC_CONST;
	}

	static CORBA::Long op3 (CORBA::Long p1)
	{
		return p1 + 3 * TestORB::MAGIC_CONST;
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_static)
