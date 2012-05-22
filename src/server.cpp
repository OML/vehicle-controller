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

#include "client.h"
#include "file.h"

#include <iostream>
#include <memory>

#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "event_loop.h"

server::server(int port): file(-1)
{
	struct sockaddr_in addr;
        int fd;
	errno = 0;
        int optval = 1;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
		goto er_socket;

	bzero(&addr, sizeof(struct sockaddr_in));


	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		goto er_bind;

	if(listen(fd, 5) < 0)
	        goto er_listen;

	std::cout << "Server listening on port " << port << "." << std::endl;

	set_fd(fd);

	return;
er_listen:
er_bind:
	::close(fd);
	fd = -1;
er_socket:
        std::cout << "Unable to open socket: " << strerror(errno) << std::endl;
	return;				
}

void server::accept()
{
        sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        client* cli;
        int cli_fd;

        cli_fd = ::accept(fd, (sockaddr*)&cli_addr, &cli_len);
        if(cli_fd) {
                std::cout << "Incoming connection" << std::endl;
                cli = new client(cli_fd);
        }
}

void server::data_available()
{
        accept();
}

