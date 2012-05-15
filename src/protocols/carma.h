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

#include <cstdint>

struct tab2car_motor
{
        int16_t         throttle;       // promille
        uint16_t        voltage;        // millivolts
        uint16_t        current;        // milliamperes
        uint16_t        temperature;    // tenth degrees
} __attribute__((packed));


struct tab2car_packet
{
        uint8_t turbo;
        uint8_t calibrate;
        int8_t motors[4];
        int8_t left;
        int8_t right;
        uint8_t sound;
}__attribute__((packed));

class carma: public protocol
{
        public:
                carma();
                ~carma();

                int                     init();
                int                     disconnect();
                size_t                  fill(char** buffer, std::shared_ptr<const tab2car_packet> pack);
                size_t                  interpret(std::shared_ptr<char> buffer);

};

#endif /* protocols/carma.h */
