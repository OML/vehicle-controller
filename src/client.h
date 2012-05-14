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

class client
{
	public:
		client(int fd);

		size_t          	write(std::shared_ptr<const char> data, size_t len);
		size_t				read(std::shared_ptr<char> buffer, size_t maxlen);
	friend class event_loop;
	protected:
	    void            	data_available();
		size_t				bytes_available();
		int					fd;

	private:

};



#endif /* src/client.h */
