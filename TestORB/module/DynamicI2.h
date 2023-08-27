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
#ifndef NIRVANA_TESTORB_TEST_DYNAMICI2_H_
#define NIRVANA_TESTORB_TEST_DYNAMICI2_H_
#pragma once

#include "ImplI2.h"
#include <IDL/Test_I2_s.h>

namespace Test {

// Dynamic implementation of the interface Test::I2
class DynamicI2 :
	public CORBA::servant_traits <I2>::Servant <DynamicI2>,
	public ImplI2
{
public:
	DynamicI2 (int32_t addendum) :
		addendum_ (addendum)
	{
	}

	~DynamicI2 ()
	{
	}

	// abstract interface A1::aop()

	int32_t aop (int32_t x) const
	{
		return x - addendum_;
	}

	// interface I2

	int32_t op2 (int32_t p1) const
	{
		return p1 + 2 * addendum_;
	}

private:
	int32_t addendum_;
};

}

#endif
