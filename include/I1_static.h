#ifndef NIRVANA_TESTORB_I1_STATIC_H_
#define NIRVANA_TESTORB_I1_STATIC_H_
#pragma once

#include <CORBA/CORBA.h>
#include <IDL/Test_I1.h>
#include <Nirvana/static.h>

NIRVANA_STATIC((Test, I1_static), ::Test::I1)

namespace TestORB {

const long MAGIC_CONST = 1963;

}

#endif
