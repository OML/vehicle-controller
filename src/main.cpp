/*
 *  OML Vehicle Controller - The software for the OML vehicle
 *  Copyright (C) 2012  Leon Colijn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "server.h"
#include "event_loop.h"

#include <memory>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

std::string gpl =
        "OML-vehicle-controller  Copyright (C) 2012  OML\n"
        "This program comes with ABSOLUTELY NO WARRANTY; for details run with `-w'.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under certain conditions; run with `-c' for details.\n";

int port = 2000;
const char* progname = NULL;


void parse_args(int argc, char* argv[])
{
        progname = argv[0];
        for(int i = 1; i < argc; i++) {
                if(strcmp(argv[i], "-c") == 0) {
                        std::cout << "TODO - conditions" << std::endl;
                        exit(0);
                } else if(strcmp(argv[i], "-w") == 0) {
                        std::cout << "TODO - warranty" << std::endl;
                        exit(0);
                } else if(strcmp(argv[i], "--help") == 0) {
                        std::cout << "Syntax: " << progname << " [options]" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Options:" << std::endl;
                        auto ph = [](std::string sw, std::string val) {std::cout << "  " << sw << std::setw(8) << " " << val << std::endl;};
                        ph("-c", "Conditions information");
                        ph("-w", "Warranty information");
                        ph("--help", "This information");

                        exit(0);
                }
        }
}

int main(int argc, char* argv[])
{
        std::cout << gpl << std::endl;

        parse_args(argc, argv);

	auto serv = std::make_shared<server>(port);
	auto evl = std::make_shared<event_loop>(serv);
	return evl->run();
}









