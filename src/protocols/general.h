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

#ifndef _SRC_PROTOCOLS_GENERAL_H
#define _SRC_PROTOCOLS_GENERAL_H

#include <cstdint>

typedef uint16_t voltage_t;
typedef uint16_t current_t;
typedef int16_t temperature_t;
typedef int16_t throttle_t;

struct motor_data
{
        throttle_t      throttle;       // promille
        voltage_t       voltage;        // millivolts
        current_t       current;        // milliamperes
        temperature_t   temperature;    // tenth degrees
} __attribute__((packed));

struct device_data
{
        voltage_t       voltage;        // millivolts
        current_t       current;        // milliamperes
        temperature_t   temperature;    // tenth degrees
} __attribute__((packed));

#endif /* src/protocols/general.h */
