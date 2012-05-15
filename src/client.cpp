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

client::client(int fd, int prot): file(fd)
{
        switch(prot) {
                case PROT_CARMA:
                        proto = std::unique_ptr<carma>(new carma());
                        break;
                default:
                        std::cout << "Unsupported protocol (" << prot << ")" << std::endl;
        }
}


void client::data_available()
{
        // Read shit and die
}

size_t client::send(const std::shared_ptr<tab2car_packet> pack)
{
        char* buffer;
        size_t size = proto->fill(&buffer, pack);
        write(std::shared_ptr<const char>(buffer), size);
        return 0;
}

size_t client::bytes_available()
{
        return 0;
}
