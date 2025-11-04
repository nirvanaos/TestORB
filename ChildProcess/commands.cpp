#include "commands.h"
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/signal_defs.h>

using namespace CORBA::Internal;

extern "C" {

void cmd_abort ()
{
	Nirvana::the_posix->raise (SIGABRT);
	NIRVANA_UNREACHABLE_CODE ();
}

}