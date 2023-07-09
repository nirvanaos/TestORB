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
#ifndef NIRVANA_TESTORB_DYNAMICI1_H_
#define NIRVANA_TESTORB_DYNAMICI1_H_
#pragma once

#include "ImplI1.h"
#include <IDL/Test_I1_s.h>

namespace Test {

// Dynamic implementation of the interface Test::I1
class DynamicI1 :
	public CORBA::servant_traits <I1>::Servant <DynamicI1>,
	public ImplI1
{
public:
	DynamicI1 (int32_t addendum) :
		addendum_ (addendum)
	{}

	~DynamicI1 ()
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

}

#endif
