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
#include <CORBA/Server.h>
#include "Test_V3_s.h"
#include "ImplI2.h"
#include "PortableI2.h"

using namespace CORBA;

namespace Test {

#ifndef LEGACY_CORBA_CPP

class ImplV3 : public IDL::traits <V3>::Servant <ImplV3>,
	public ImplI2
{
public:
	ImplV3 ()
	{}

	ImplV3 (int32_t a)
	{
		addendum (a);
	}

	~ImplV3 ()
	{}

	int16_t vop1 ()
	{
		return 0;
	}

	int32_t vop2 (V1::_ptr_type pv)
	{
		return pv->val2 ();
	}

	int32_t op_v2 (int32_t x)
	{
		val_v2 (x);
		return x;
	}

	int32_t op2 (int32_t p1)
	{
		return p1 + 2 * addendum ();
	}

	int32_t aop (int32_t x)
	{
		return x - addendum ();
	}
};

#else

class ImplV3 : public OBV_V3,
	public PortableI2
{
public:
	ImplV3 () :
		PortableI2 (0)
	{}

	ImplV3 (int32_t a) :
		PortableI2 (a)
	{
		addendum (a);
	}

	~ImplV3 ()
	{}

	virtual int16_t vop1 () override
	{
		return 0;
	}

	virtual int32_t vop2 (V1::_ptr_type pv) override
	{
		return pv->val2 ();
	}

	virtual int32_t op_v2 (int32_t x) override
	{
		val_v2 (x);
		return x;
	}

	virtual int32_t op2 (int32_t p1) override
	{
		return p1 + 2 * addendum ();
	}

	virtual int32_t aop (int32_t x) override
	{
		return x - addendum ();
	}

};

#endif

class Static_I2_factory_V3 :
	public servant_traits <I2_factory>::ServantStatic <Static_I2_factory_V3>
{
public:
	static
#ifdef LEGACY_CORBA_CPP
		I2::_var_type
#else
		I2::_ref_type
#endif
		create (int32_t addendum)
	{
#ifdef LEGACY_CORBA_CPP
		V3::_var_type
#else
		V3::_ref_type
#endif
			v = V3::_factory->create (addendum);

		// Supported interface operations must be available
		v->op2 (0);
		v->aop (0);

		// Conversion to supported interface ptr must be unavailable:
		// I2::_ptr_type p = v; // compiler error

		// Direct conversion to ValueBase must be available
		ValueBase::_ptr_type vb = v;

		// Return I2 proxy.
		return v->_this ();
	}
};

}

NIRVANA_VALUETYPE_IMPL (_exp_Test_V3, Test::V3, Test::ImplV3)
NIRVANA_EXPORT_OBJECT (_exp_Test_I2_factory_V3, Test::Static_I2_factory_V3)
