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
#include <Nirvana/Nirvana.h>
#include <Nirvana/File.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/posix_defs.h>

using namespace Nirvana;

int main (int argc, char* argv [])
{
	{
		// Get console service
		File::_ref_type console = File::_narrow (CORBA::the_orb->resolve_initial_references ("Console"));

		// Open console access
		AccessChar::_ref_type access = AccessChar::_narrow (console->open (O_RDWR | O_TEXT, 0)->_to_object ());

		// Connect to the console input event
		access->connect_pull_consumer (nullptr);

		access->write ("Press Enter to continue\n");

		for (;;) {
			CORBA::Any evt = access->pull ();
			const std::string* s;
			if (evt >>= s) {
				access->write (*s);
				if (s->find ('\n') != std::string::npos)
					break;
			} else {
				AccessChar::Error err;
				if (evt >>= err)
					access->write ("Input error " + std::to_string (err.error ()) + '\n');
			}
		}
	}

	static const char test [] = "Test console write()\n";
	the_posix->write (1, test, sizeof (test) - 1);

	return 0;
}
