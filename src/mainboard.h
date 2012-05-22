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

#include <ufile.h>
//#include <config_file.h>

#include "protocols/general.h"

#include <memory>

#define MAINBOARD \
        (const_cast<mainboard*>(mainboard::instance))

#define NMOTORS 4


// Possible values for mbtophy_packet.flags
#define MBP_ERROR               (1 << 0)
#define MBP_RESET               (1 << 1)
#define MBP_UNDERVOLTAGE        (1 << 2)

struct mbtophy_packet
{
        uint32_t        flags;
        motor_data      motors[NMOTORS];
} __attribute__((packed));

class mainboard: public ufile
{
        public:
                mainboard(const char* sfile);

                static mainboard*       instance;

                int                     calibrate(uint16_t speeds[NMOTORS]);
                int                     set_tresholds(const event_tresholds& tresh);
                int                     set_throttle(bool fast, int left, int right);
                int                     halt();
        private:
                void                    data_available();

                float                   motor_multiplier[NMOTORS];
  //              std::unique_ptr<config_file> config;

};

#endif /* mainboard.h */
