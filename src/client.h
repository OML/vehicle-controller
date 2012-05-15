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

#include <memory>

#include <sys/types.h>

#include "protocols/prot_tab2car.h"
#include "protocols/protocol.h"

enum
{
	PROT_STANDARD,
	PROT_CARMA,
};

class client
{
	public:
		client(int fd, int protocol = PROT_CARMA);

		size_t          	        send(const std::shared_ptr<tab2car_packet> p);
	friend class event_loop;
	protected:
	        void            	        data_available();
		size_t				bytes_available();
		int				fd;

		std::unique_ptr<protocol>       proto;

	private:

};



#endif /* src/client.h */
