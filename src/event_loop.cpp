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

#include <sys/select.h>

event_loop::event_loop(std::shared_ptr<server> serv):
	serv(serv)
{
        serv->evl = std::shared_ptr<event_loop>(this);
}

void event_loop::register_client(std::shared_ptr<client> cl)
{
        if(cl == NULL || !cl->fd) {
                std::cout << "Invalid client" << std::cout;
                return;
        }

        clients[cl->fd] = cl;
}

int event_loop::run()
{
        timeval timeout;
        int maxfd;
        fd_set read_fds;

        if(serv == NULL)
		return -1;

	FD_ZERO(&read_fds);

	while(true) {
	        FD_ZERO(&read_fds);
	        FD_SET(serv->sockfd, &read_fds);
	        maxfd = serv->sockfd;
	        for(auto client: clients) {
	                FD_SET(client.first, &read_fds);
	                maxfd = std::max(maxfd, client.first);
	        }

	        timeout.tv_sec = 10;
	        timeout.tv_usec = 0;
	        select(maxfd + 1, &read_fds, NULL, NULL, &timeout);

	        if(timeout.tv_sec == 10 && timeout.tv_usec == 0) {
	                // Timeout
	                continue;
	        }

	        if(FD_ISSET(serv->sockfd, &read_fds))
	                serv->accept();

	        for(auto client: clients) {
	                if(FD_ISSET(client.first, &read_fds))
	                        client.second->data_available();
	        }
	}

	return 0;
}
