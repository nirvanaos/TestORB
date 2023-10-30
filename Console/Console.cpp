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
#include <fnctl.h>
#include <unistd.h>

using namespace Nirvana;

int main (int argc, char* argv [])
{
	{
		// Get console service
		File::_ref_type console = File::_narrow (CORBA::g_ORB->resolve_initial_references ("Console"));

		// Open console access
		AccessChar::_ref_type access = AccessChar::_narrow (console->open (O_RDWR | O_TEXT, 0)->_to_object ());

		// Connect to the console input event
		auto supplier = access->for_consumers ()->obtain_typed_pull_supplier (_tc_PullCharFileSink->id ());
		auto input = PullCharFileSink::_narrow (supplier->get_typed_supplier ());
		supplier->connect_pull_consumer (nullptr);

		access->write ("Press Enter to continue\n");

		for (;;) {
			CharFileEvent evt;
			input->received (evt);
			if (evt.error ()) {
				access->write ("Input error " + std::to_string (evt.error ()) + '\n');
				break;
			} else {
				access->write (evt.data ());
				if (evt.data ().find ('\n') != std::string::npos)
					break;
			}
		}
	}

	static const char test [] = "Test console write()\n";
	write (1, test, sizeof (test) - 1);

	return 0;
}
