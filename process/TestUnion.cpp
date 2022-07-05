#include "TestORB.h"

// Union.idl contains only declaration for interface Test::I1.
// This file must be successfully compiled without include "Test_I1.h"

#include "IDL/Union.h"

using namespace std;
using namespace CORBA;
using namespace Test;

namespace TestORB {

TEST_F (TestORB, Union)
{
	S s{ 10 };

	U u;
	u.w (s); // member w selected
	u._d (3); // OK, member w selected
	u._d (4); // OK, member w selected
	EXPECT_THROW (u._d (1), BAD_PARAM); // error, different member selected, results in BAD_PARAM

#ifdef LEGACY_CORBA_CPP
	I1_var
#else
	IDL::traits <I1>::ref_type
#endif
		a;
	u.obj (a); // member obj selected
	u._d (7); // OK, member obj selected
	EXPECT_THROW (u._d (1), BAD_PARAM); // error, different member selected, results in BAD_PARAM
	EXPECT_THROW (s = u.w (), BAD_PARAM); // error, member w not active, results in BAD_PARAM
	u.w (s, 4); // OK, member w selected and discriminator is 4
	u.obj (a, 23); // OK, member obj selected and discriminator is 23
	EXPECT_THROW (u.obj (a, 2), BAD_PARAM); // error, 2 is not a valid discriminator for obj, results in BAD_PARAM
	//u.x (0, 1); // compile-time error, x only has 1 possible discriminator value

	Z z;
	z._default (); // implicit default member selected
	EXPECT_FALSE (z._d ());
}

}
