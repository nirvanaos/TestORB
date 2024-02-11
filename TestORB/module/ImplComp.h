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
#ifndef NIRVANA_TESTORB_TEST_IMPLCOMP_H_
#define NIRVANA_TESTORB_TEST_IMPLCOMP_H_
#pragma once

#include <CORBA/Server.h>
#include "Test_Comp_s.h"
#include "DynamicI1.h"
#include "PortableI1.h"

namespace Test {

class ImplComp : public CORBA::servant_traits <Test::Comp>::Servant <ImplComp>
{
public:
#ifndef LEGACY_CORBA_CPP

	ImplComp (uint32_t addendum, Comp::_ref_type& comp)
	{
		comp = _this ();
		facet1_ = CORBA::make_reference <DynamicI1> (addendum, comp)->_this ();
	}

#else

	ImplComp (uint32_t addendum, Comp_var& comp)
	{
		comp = _this ();
		facet1_ = PortableServer::Servant_var <DynamicI1> (new DynamicI1 (addendum, comp))->_this ();
	}

#endif

#ifdef LEGACY_CORBA_CPP
	I1_ptr provide_facet1 () const
	{
		return I1::_duplicate (facet1_);
	}
#else
	I1::_ref_type provide_facet1 () const
	{
		return facet1_;
	}
#endif

	SeqI1 get_all_connections () const
	{
		SeqI1 ret;
		if (receptacle_single ())
			ret.push_back (receptacle_single ());
		for (const auto& c : receptacle_multi ()) {
			if (c)
				ret.push_back (c);
		}
		return ret;
	}

private:
#ifdef LEGACY_CORBA_CPP
	I1_var facet1_;
#else
	I1::_ref_type facet1_;
#endif
};

class ImplCompPOA : public CORBA::servant_traits <Test::Comp>::base_type
{
public:
#ifndef LEGACY_CORBA_CPP

	ImplCompPOA (uint32_t addendum, Comp::_ref_type& comp)
	{
		comp = _this ();
		facet1_ = CORBA::make_reference <PortableI1> (addendum, comp)->_this ();
	}

#else

	ImplCompPOA (uint32_t addendum, Comp_var& comp)
	{
		comp = _this ();
		facet1_ = PortableServer::Servant_var <DynamicI1> (new PortableI1 (addendum, comp))->_this ();
	}

#endif

#ifdef LEGACY_CORBA_CPP
	virtual I1_ptr provide_facet1 ()
	{
		return I1::_duplicate (facet1_);
	}
#else
	virtual I1::_ref_type provide_facet1 ()
	{
		return facet1_;
	}
#endif

	virtual SeqI1 get_all_connections ()
	{
		SeqI1 ret;
		if (receptacle_single ())
			ret.push_back (receptacle_single ());
		for (const auto& c : receptacle_multi ()) {
			if (c)
				ret.push_back (c);
		}
		return ret;
	}

private:
#ifdef LEGACY_CORBA_CPP
	I1_var facet1_;
#else
	I1::_ref_type facet1_;
#endif
};

}

#endif
