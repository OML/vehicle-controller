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


#ifndef _CLIENT_H
#define _CLIENT_H

#include <sys/types.h>

#include <list>

#include "protocols/protocol.h"
#include "protocols/general.h"

#include "ufile.h"

enum
{
	PROT_STANDARD,
	PROT_CARMA,
};

class client: public ufile
{
	public:
		client(int fd, int protocol = PROT_CARMA);
		~client();

		ssize_t peek(char* buffer, size_t size);

		static void incoming_motor_sensors_event(unsigned long int timestamp, motor_data ev[4]);
		static void incoming_gyro_event(unsigned long int timestamp, int16_t axis[2]);
	friend class event_loop;
	protected:

		protocol* proto;

	private:
		void data_available();

		static std::list<client*> clients;
};



#endif /* src/client.h */
