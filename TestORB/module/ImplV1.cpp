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
#include <IDL/Test_V1_s.h>

namespace Test {

class ImplV1 : public CORBA::servant_traits <V1>::Servant <ImplV1>
{
public:
	ImplV1 ()
	{}

	ImplV1 (int16_t v1, std::string v3)
	{
		val1 (v1);
		val3 (std::move (v3));
	}

	~ImplV1 ()
	{}

	int16_t vop1 ()
	{
		return 0;
	}

	int32_t vop2 (V1::_ptr_type pv)
	{
		return pv->val2 ();
	}
};

}

NIRVANA_VALUETYPE_IMPL (Test::V1, Test::ImplV1)