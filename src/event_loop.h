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

#ifndef _SRC_EVENTLOOP_H
#define _SRC_EVENTLOOP_H

#include "server.h"
#include "client.h"

#include <map>
#include <memory>

class event_loop
{
	public:
		event_loop(std::shared_ptr<server> serv);
	
		int 				        run();
		void 				        register_client(std::shared_ptr<client> client);


		std::shared_ptr<server> 	        serv;
	private:
		std::map<int, std::shared_ptr<client>>  clients;
};

#endif /* src/eventloop.h */
