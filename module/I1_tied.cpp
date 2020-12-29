#include "I1_tied.h"
#include "ImplI1.h"
#include "Test_I1_s.h"
#include "Test_I1_factory_s.h"

namespace Test {

// Tied implementation

class TiedI1 :
	public ImplI1
{
public:
	TiedI1 (CORBA::Long addendum) :
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

private:
	CORBA::Long addendum_;
};

class I1_tied :
	public ::CORBA::Nirvana::ServantStatic <I1_tied, I1_factory>
{
public:
	static I1_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <POA_Test::I1_tie <TiedI1> > (new POA_Test::I1_tie <TiedI1> (new TiedI1 (addendum)))->_this ();
	}
};

class TiedDerivedI1 :
	public POA_Test::I1_tie <TiedI1>,
	public TiedI1
{
public:
	TiedDerivedI1 (CORBA::Long addendum) :
		POA_Test::I1_tie <TiedI1> (static_cast <TiedI1&> (*this)),
		TiedI1 (addendum)
	{}
};

class I1_tied_derived :
	public ::CORBA::Nirvana::ServantStatic <I1_tied_derived, I1_factory>
{
public:
	static I1_var create (CORBA::Long addendum)
	{
		return PortableServer::Servant_var <TiedDerivedI1> (new TiedDerivedI1 (addendum))->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_tied)
NIRVANA_STATIC_EXP (Test, I1_tied_derived)
