#include <CORBA/Server.h>
#include "I1_factory_tied.h"
#include "ImplI1.h"
#include "Test_I1_s.h"
#include "Test_I1_factory_s.h"

namespace Test {

// Tied implementation

class TiedI1 :
	public ImplI1
{
public:
	TiedI1 (int32_t addendum) :
		addendum_ (addendum)
	{}

	int32_t op1 (int32_t p1) const
	{
		return p1 + addendum_;
	}

	int32_t addendum () const
	{
		return addendum_;
	}

	void addendum (int32_t a)
	{
		addendum_ = a;
	}

private:
	int32_t addendum_;
};

class I1_factory_tied :
	public CORBA::servant_traits <I1_factory>::ServantStatic <I1_factory_tied>
{
public:
	static I1::_ref_type create (CORBA::Long addendum)
	{
		return CORBA::make_reference <POA_Test::I1_tie <TiedI1> > (new TiedI1 (addendum))->_this ();
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
	public CORBA::Internal::ServantStatic <I1_tied_derived, I1_factory>
{
public:
	static I1::_ref_type create (CORBA::Long addendum)
	{
		return CORBA::make_reference <TiedDerivedI1> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I1_factory_tied)
NIRVANA_STATIC_EXP (Test, I1_tied_derived)
