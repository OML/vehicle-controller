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

#include "client.h"

#include <memory.h>
#include <iostream>

#include "protocols/carma.h"

#include <sys/socket.h>

client::client(int fd, int prot): file(fd)
{
        switch(prot) {
                case PROT_CARMA:
                        proto = std::unique_ptr<carma>(new carma(this));
                        break;
                default:
                        std::cout << "Unsupported protocol (" << prot << ")" << std::endl;
        }
}

client::~client()
{

}

void client::data_available()
{
        if(bytes_available() == 0) {
                std::cout << "Disconnected " << fd << std::endl;
                close();
                delete this;
                return;
        }

        if(proto->start_reading() == -1) {
                std::cout << "Protocol error. Disconnecting client." << std::endl;
                close();
        }
}

ssize_t client::peek(char* buffer, size_t size)
{
       return ::recv(fd, buffer, size, MSG_PEEK);
}
