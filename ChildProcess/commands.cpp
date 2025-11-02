#include "commands.h"
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/signal_defs.h>

using namespace CORBA::Internal;

extern "C" {

void cmd_abort ()
{
	Bridge <Nirvana::POSIX>* br = static_cast <Bridge <Nirvana::POSIX>*> (
		&static_cast <I_ptr <Nirvana::POSIX>> (Nirvana::the_posix));
	br->_epv ().epv.raise (br, SIGABRT, nullptr);
	NIRVANA_UNREACHABLE_CODE ();
}

}