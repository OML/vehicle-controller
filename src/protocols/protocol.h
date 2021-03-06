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

#include <sys/types.h>

#include "protocols/general.h"

class client;

class protocol
{
	public:
                protocol(client* c);
                virtual ~protocol();

                virtual int init() = 0;
                virtual int disconnect() = 0;
                virtual int start_reading() = 0;


                virtual void motor_sensors_event(unsigned long int timestamp, motor_data ev[2]) = 0;
		virtual void gyro_event(unsigned long int timestamp, int16_t axis[2]) = 0;
	protected:
                client* cl;
};

#endif /* protocols/protocol.h */
