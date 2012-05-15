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

#ifndef _PROTOCOLS_PROT_TAB2CAR_H
#define _PROTOCOLS_PROT_TAB2CAR_H

#include <cstdint>

enum tab2car_cmds
{
	T2C_CMD_WRITE,
};

enum tab2car_steermode
{
	T2C_SM_XY,
	T2C_SM_THROTTLE
};

struct tab2car_motor
{
	uint16_t	throttle;	// promille
	uint16_t	voltage;	// millivolts
	uint16_t	current;	// milliamperes
	uint16_t	temperature;	// tenth degrees
} __attribute__((packed));

struct tab2car_packet
{
	uint8_t		cmd;		// T2C_CMD_WRITE
	union {
		struct {
			struct {
				int8_t		left;
				int8_t		right;
			}		steer;
			struct {
				
			};
			tab2car_motor	motors[4];
		}	payload;
	};
	
} __attribute__((packed));

#endif /* protocols/prot_tab2car.h */
