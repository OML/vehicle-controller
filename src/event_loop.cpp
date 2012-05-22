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

#include "mainboard.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <memory>

#include <sys/select.h>

event_loop* event_loop::instance = NULL;

void event_loop::evl_event_queue::data_available(unixpipe_endpoint* ep)
{
        event_loop::event ev;
        while(ep->bytes_available()) {
                if(ep->read(reinterpret_cast<char*>(&ev),
                            static_cast<size_t>(sizeof(event_loop::event)))
                        < 0) {
                        std::cout << __PRETTY_FUNCTION__
                                  << ": Unable to read events from the queue"
                                  << std::endl;
                        return;
                }
                process_event(ev);
        }
}

void event_loop::evl_event_queue::process_event(event_loop::event& ev)
{
        switch(ev.type) {
                default:
                        return;
        }
}

void event_loop::evl_event_queue::post_event(event_loop::event& ev)
{
        first()->write(reinterpret_cast<char*>(&ev),
                     static_cast<size_t>(sizeof(event_loop::event)));
}

event_loop::event_loop()
{
        instance = this;
        /* This has to be initialized _after_ instance has been assigned.
         * Otherwise the files of the endpoints cannot register their fd's for
         * reading.
         */
        event_queue = NULL;
        event_queue = std::unique_ptr<evl_event_queue>(
                        new evl_event_queue());
}

void event_loop::register_file(file* f)
{
        if(f == NULL) {
                std::cout << "Invalid file" << std::cout;
                return;
        }

        files.push_back(f);
        flush();
}

void event_loop::unregister_file(file* f)
{
        files.remove(f);
        flush();
}

int event_loop::flush()
{
        if(!event_queue)
                return 0;
        event ev;
        ev.type = EV_FLUSH;
        event_queue->post_event(ev);
        return 0;
}

int event_loop::run()
{
        timeval timeout;
        int maxfd;
        fd_set read_fds;

	FD_ZERO(&read_fds);

	while(true) {
	        FD_ZERO(&read_fds);
	        maxfd = 0;
	        for(auto file: files) {
	                if(file->fd > -1 && file->flags & FF_SELECT_READ) {
	                        FD_SET(file->fd, &read_fds);
	                        maxfd = std::max(maxfd, file->fd);
	                }
	        }

	        timeout.tv_sec = 2;
	        timeout.tv_usec = 0;
	        select(maxfd + 1, &read_fds, NULL, NULL, &timeout);
	        if(timeout.tv_sec == 2 && timeout.tv_usec == 0) {
	                //MAINBOARD->halt();
#warning "Not halting mainboard on timeout"
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
