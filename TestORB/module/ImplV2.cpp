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
#include "Test_V2_s.h"

namespace Test {

#ifndef LEGACY_CORBA_CPP

class ImplV2 : public IDL::traits <V2>::Servant <ImplV2>
{
public:
	ImplV2 ()
	{}

	~ImplV2 ()
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

	int32_t aop (int32_t x)
	{
		return x;
	}
};

#else

class ImplV2 : public IDL::traits <V2>::obv_type
{
public:
	ImplV2 ()
	{}

	~ImplV2 ()
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

	virtual int32_t aop (int32_t x) override
	{
		return x;
	}
};

#endif

}

NIRVANA_VALUETYPE_IMPL (Test::V2, Test::ImplV2)
