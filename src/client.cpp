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

std::list<client*> client::clients;

client::client(int fd, int prot): ufile(fd)
{
        set_event_mask(UFILE_EVENT_READ);

        switch(prot) {
                case PROT_CARMA:
                        proto = new carma(this);
                        break;
                default:
                        proto = NULL;
                        std::cout << "Unsupported protocol (" << prot << ")" << std::endl;
        }
        clients.push_back(this);
}

client::~client()
{
        clients.remove(this);
        if(proto)
                delete proto;
}

void client::data_available()
{
        if(bytes_available() == 0) {
                std::cout << "Disconnected " << fd << std::endl;
                close();
                defer_delete();
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


void client::incoming_motor_sensors_event(unsigned long int timestamp, motor_data ev[4])
{
        std::list<client*>::iterator it;
        for(it = clients.begin(); it != clients.end(); it++) {
		if((*it)->proto)
	                (*it)->proto->motor_sensors_event(timestamp, ev);
        }
}

void client::incoming_gyro_event(unsigned long int timestamp, int16_t axis[2])
{
	std::list<client*>::iterator it;
	for(it = clients.begin(); it != clients.end(); it++) {
		if((*it)->proto)
			(*it)->proto->gyro_event(timestamp, axis);
	}
}
