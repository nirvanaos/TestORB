#ifndef NIRVANA_TESTORB_TESTORB_I1_H_
#define NIRVANA_TESTORB_TESTORB_I1_H_
#pragma once

#include "TestORB.h"
#include "IDL/Test_I1.h"

namespace TestORB {

void test_interface (Test::I1::_ptr_type p);
void test_performance (Test::I1::_ptr_type p);

}

#endif
