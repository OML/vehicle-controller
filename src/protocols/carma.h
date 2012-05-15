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

#ifndef _PROTOCOLS_CARMA_H
#define _PROTOCOLS_CARMA_H

#include "protocol.h"

class carma: public protocol
{
        public:
                carma();
                ~carma();

                int             init();
                int             disconnect();
                size_t          send(const std::shared_ptr<tab2car_packet> p);
                size_t          receive(std::shared_ptr<tab2car_packet> p);

};

#endif /* protocols/carma.h */
