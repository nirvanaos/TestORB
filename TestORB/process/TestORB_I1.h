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
#ifndef NIRVANA_TESTORB_TESTORB_I1_H_
#define NIRVANA_TESTORB_TESTORB_I1_H_
#pragma once

#include <CORBA/CORBA.h>
#include "idl/Test_I1.h"

namespace TestORB {

#ifdef LEGACY_CORBA_CPP

typedef Test::I1_var I1_ref;
typedef CORBA::Object_var Object_ref;

#else

typedef Test::I1::_ref_type I1_ref;
typedef CORBA::Object::_ref_type Object_ref;

#endif

void test_interface (Test::I1::_ptr_type p);
void test_performance (Test::I1::_ptr_type p);

}

#endif
