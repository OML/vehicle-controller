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

#include "prot_tab2car.h"

client::client(int fd):
        fd(fd)
{

}


void client::data_available()
{
	char buffer[bytes_available()];
	tab2car_packet* pack = (tab2car_packet*)&buffer;

	read(&buffer, bytes_available());
	
}

size_t client::write(std::shared_ptr<const char> data, size_t size)
{
}

size_t client::read(std::shared_ptr<char> buffer, size_t max)
{
}

size_t client::bytes_available()
{
}
