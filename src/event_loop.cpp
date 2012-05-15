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

#include "event_loop.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <memory>

#include <sys/select.h>

event_loop* event_loop::instance = NULL;

event_loop::event_loop()
{
        instance = this;
}

void event_loop::register_file(file* f)
{
        if(f == NULL) {
                std::cout << "Invalid file" << std::cout;
                return;
        }

        files.push_back(f);
}

int event_loop::flush()
{
        std::cout << "Flush not implemented" << std::endl;
        return 0;
}

int event_loop::run()
{
        std::cout << "Start event loop" << std::endl;
        timeval timeout;
        int maxfd;
        fd_set read_fds;

	FD_ZERO(&read_fds);

	while(true) {
	        FD_ZERO(&read_fds);
	        for(auto file: files) {
	                if(file->fd > -1 && file->flags & FF_SELECT_READ) {
	                        std::cout << "Select " << file->fd << std::endl;
	                        FD_SET(file->fd, &read_fds);
	                        maxfd = std::max(maxfd, file->fd);
	                }
	        }

	        timeout.tv_sec = 10;
	        timeout.tv_usec = 0;
	        select(maxfd + 1, &read_fds, NULL, NULL, &timeout);
	        if(timeout.tv_sec == 10 && timeout.tv_usec == 0) {
	                // Timeout
	                continue;
	        }

	        for(auto file: files) {
	                if(FD_ISSET(file->fd, &read_fds)) {
	                        std::cout << "Activity " << file->fd << std::endl;
	                        file->data_available();
	                }
	        }
	}

	return 0;
}
