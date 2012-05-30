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

#ifndef _MAINBOARD_H
#define _MAINBOARD_H

#include <string>

#include <ufile.h>
#include <config_file.h>

#include "bus/busprot.h"

#include "protocols/general.h"

#define MAINBOARD \
        (const_cast<mainboard*>(mainboard::instance))

#define NMOTORS 4


#define MOTOR_FRONT_LEFT 0
#define MOTOR_FRONT_RIGHT 1
#define MOTOR_BACK_RIGHT 2
#define MOTOR_BACK_LEFT 3

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1


class mainboard: public ufile
{
        public:
                mainboard(const std::string& sfile);
                ~mainboard();

                static mainboard*       instance;

                int                     calibrate(uint16_t speeds[NMOTORS]);
                int                     set_thresholds(const event_thresholds& thresh);
                int                     set_throttle(bool fast, int left, int right);
                int                     halt();
        private:
                void                    data_available();
                void                    process_packet(const char* data);
                void                    process_hello(const char* data);


                void                    open_fifo();

                void                    bus_write(const char* data, size_t len);

                float                   motor_multiplier[NMOTORS];
                config_file*            config;

                std::string sfile;


                bus_addr_t              my_addr;
                bus_addr_t              host_addr;

                bus_addr_t              motor_front_addr;
                bus_addr_t              motor_back_addr;
};

#endif /* mainboard.h */
