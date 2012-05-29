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
#include "mainboard.h"

#include <memory>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdlib>

std::string gpl =
        "OML-vehicle-controller  Copyright (C) 2012  OML\n"
        "This program comes with ABSOLUTELY NO WARRANTY; for details run with `-w'.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under certain conditions; run with `-c' for details.\n";

const char* ttyS0 = "/dev/ttyS0";

int port = 1337;
const char* progname = NULL;
const char* mainboard_file = ttyS0;


static void print_helpline(const std::string& sw, const std::string& value)
{
        std::cout << "  " << sw << std::setw(8) << " " << value << std::endl;
}

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
                } else if(strcmp(argv[i], "-f") == 0) {
                        mainboard_file = argv[++i];
                } else if(strcmp(argv[i], "--help") == 0) {
                        std::cout << "Syntax: " << progname << " [options]" << std::endl;
                        std::cout << std::endl;
                        std::cout << "Options:" << std::endl;
                        print_helpline("-c    ", "Conditions information");
                        print_helpline("-w    ", "Warranty information");
                        print_helpline("-f    ", "Specifiy Mainboard FIFO (default /dev/ttyS0)");
                        print_helpline("--help", "This information");
                        exit(0);
                }

        }
}

class stdin_ep: public ufile
{
        public:
                stdin_ep()
                {
                        set_event_mask(UFILE_EVENT_READ);
                        set_fd(1);
                }

                void data_available()
                {
                        size_t buflen = bytes_available();
                        char buffer[buflen];
                        read(buffer, buflen);

                        parse(buffer, buflen);
                }


                void parse(char* buffer, size_t len)
                {
                        char* start = buffer;
                        size_t cur = 0;
                        while(cur < len) {
                                while(cur < len && isalnum(buffer[cur]))
                                        cur++;

                                if(strncmp(start, "exit", cur - (start - buffer)) == 0) {
                                        EVL->stop();
                                        return;
                                }
                        }
                }
};

int main(int argc, char* argv[])
{
        std::cout << gpl << std::endl;

        parse_args(argc, argv);

        event_loop* evl = new event_loop();

	server* serv = new server(port);
	mainboard* main = new mainboard(mainboard_file);

	stdin_ep* stdin = new stdin_ep();

	int rv = evl->run();

	delete serv;
	delete main;
	delete stdin;
        delete evl;

        return rv;
}









