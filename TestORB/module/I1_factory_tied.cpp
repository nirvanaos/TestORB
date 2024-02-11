/*
* Nirvana test suite.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include "pch.h"
#include "ImplI1.h"
#include "IDL/Test_I1_s.h"
#include "IDL/Test_I1_factory_s.h"

using namespace CORBA;
using namespace PortableServer;

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

class Static_I1_factory_tied :
	public servant_traits <I1_factory>::ServantStatic <Static_I1_factory_tied>
{
public:
#ifdef LEGACY_CORBA_CPP

	static I1_ptr create (Long addendum)
	{
		Servant_var <POA_Test::I1_tie <TiedI1> > serv = new POA_Test::I1_tie <TiedI1> (new TiedI1 (addendum));

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#else

	static I1::_ref_type create (Long addendum)
	{
		auto serv = make_reference <servant_traits <I1>::tie_type <TiedI1> > (new TiedI1 (addendum));

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#endif
};

#ifdef LEGACY_CORBA_CPP

class TiedDerivedI1 :
	public POA_Test::I1_tie <TiedI1>,
	public TiedI1
{
public:
	TiedDerivedI1 (Long addendum) :
		POA_Test::I1_tie <TiedI1> (static_cast <TiedI1&> (*this)),
		TiedI1 (addendum)
	{}
};

#else

class TiedDerivedI1 :
	public servant_traits <I1>::tie_type <TiedI1>,
	public TiedI1
{
public:
	TiedDerivedI1 (Long addendum) :
		servant_traits <I1>::tie_type <TiedI1> (static_cast <TiedI1&> (*this)),
		TiedI1 (addendum)
	{}
};

#endif

class Static_I1_tied_derived :
	public servant_traits <I1_factory>::ServantStatic <Static_I1_tied_derived>
{
public:
	
#ifdef LEGACY_CORBA_CPP

	static I1_ptr create (Long addendum)
	{
		Servant_var <TiedDerivedI1> serv = new TiedDerivedI1 (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#else

	static I1::_ref_type create (Long addendum)
	{
		auto serv = make_reference <TiedDerivedI1> (addendum);

		// Direct conversion to ServantBase must be available
		Servant s = serv;
		assert (s);

		return serv->_this ();
	}

#endif

};

}

NIRVANA_EXPORT_OBJECT (_exp_Test_I1_factory_tied, Test::Static_I1_factory_tied)
NIRVANA_EXPORT_OBJECT (_exp_Test_I1_tied_derived, Test::Static_I1_tied_derived)
