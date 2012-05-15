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

#ifndef _PROTOCOLS_PROTOCOL_H
#define _PROTOCOLS_PROTOCOL_H

#include "prot_tab2car.h"

#include <memory>

class protocol
{
	public:
                protocol();
                virtual ~protocol();

                virtual int init() = 0;
                virtual int disconnect() = 0;
                virtual size_t fill(std::shared_ptr<std::shared_ptr<char>> buffer, const std::shared_ptr<tab2car_packet> pack) = 0;
                virtual size_t interpret(std::shared_ptr<char> buffer) = 0;
};

#endif /* protocols/protocol.h */
