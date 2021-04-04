#include "I3_factory_tied.h"
#include "ImplI1.h"
#include "Test_I3_s.h"
#include "Test_I3_factory_s.h"

namespace Test {

// Tied implementation

class TiedI3 :
	public ImplI1
{
public:
	TiedI3 (CORBA::Long addendum) :
		addendum_ (addendum)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		return p1 + addendum_;
	}

	void throw_no_implement ()
	{
		throw CORBA::NO_IMPLEMENT ();
	}

	CORBA::Long op2 (CORBA::Long p1) const
	{
		return p1 + 2 * addendum_;
	}

	CORBA::Long op3 (CORBA::Long p1) const
	{
		return p1 + 3 * addendum_;
	}

private:
	CORBA::Long addendum_;
};

class I3_factory_tied :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_tied>
{
public:
	static I3_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <POA_Test::I3_tie <TiedI3> > (new POA_Test::I3_tie <TiedI3> (new TiedI3 (addendum)))->_this ();
	}
};

class TiedDerivedI3 :
	public POA_Test::I3_tie <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (CORBA::Long addendum) :
		POA_Test::I3_tie <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}
};

class I3_tied_derived :
	public ::CORBA::Nirvana::ServantStatic <I3_tied_derived, I3_factory>
{
public:
	static I3_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <TiedDerivedI3> (new TiedDerivedI3 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_tied)
NIRVANA_STATIC_EXP (Test, I3_tied_derived)
