#include <CORBA/Server.h>
#include "I3_factory_tied.h"
#include "ImplI1.h"
#include "ImplI3.h"
#include <IDL/Test_I3_s.h>
#include <IDL/Test_I3_factory_s.h>

namespace Test {

// Tied implementation

class TiedI3 :
	public ImplI1,
	public ImplI3
{
public:
	TiedI3 (int32_t addendum) :
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

	int32_t op2 (int32_t p1) const
	{
		return p1 + 2 * addendum_;
	}

	int32_t op3 (int32_t p1) const
	{
		return p1 + 3 * addendum_;
	}

	int32_t aop (int32_t x) const
	{
		return x - addendum ();
	}

private:
	int32_t addendum_;
};

class I3_factory_tied :
	public CORBA::servant_traits <I3_factory>::ServantStatic <I3_factory_tied>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I3::_ptr_type
#else
		I3::_ref_type
#endif
		create (CORBA::Long addendum)
	{
		return CORBA::make_reference <CORBA::servant_traits <I3>::tie_type <TiedI3> > (new TiedI3 (addendum))->_this ();
	}
};

class TiedDerivedI3 :
	public CORBA::servant_traits <I3>::tie_type <TiedI3>,
	public TiedI3
{
public:
	TiedDerivedI3 (CORBA::Long addendum) :
		CORBA::servant_traits <I3>::tie_type <TiedI3> (static_cast <TiedI3&> (*this)),
		TiedI3 (addendum)
	{}
};

class I3_tied_derived :
	public CORBA::Internal::ServantStatic <I3_tied_derived, I3_factory>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I3::_ptr_type
#else
		I3::_ref_type
#endif
		create (CORBA::Long addendum)
	{
		return CORBA::make_reference <TiedDerivedI3> (addendum)->_this ();
	}
};

}

NIRVANA_STATIC_EXP (Test, I3_factory_tied)
NIRVANA_STATIC_EXP (Test, I3_tied_derived)
