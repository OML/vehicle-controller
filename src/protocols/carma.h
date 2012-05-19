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

#include "general.h"

#include <cstdint>
#include <sys/types.h>

// Supported version
#define CARMA_VERSION   0
#define CARMA_REVISION  2

class client;

enum
{
        COP_REJECT = 0,
        COP_OK,
        COP_SYNC,
        COP_REPORT,
};

enum
{
        C_REJECT_REASON_UNKONWN,
        C_REJECT_REASON_INVALID
};

struct carma_t2c_sync_request
{
        uint8_t speed;
        int8_t left;
        int8_t right;
        uint8_t calibrate;
        uint16_t motors[4];
        uint8_t sound;

}__attribute__((packed));

struct carma_c2t_sync_response
{
        uint8_t opcode;
        uint32_t timestamp;
        motor_data motors[4];
        int16_t gyro[3];
        int16_t accu_voltage;
        int16_t accu_current;
};

class carma: public protocol
{
        public:
                carma(client* c);
                ~carma();

                int                     init();
                int                     disconnect();
                int                     start_reading(size_t bytes);

                int                     calibrate(uint16_t motors[4]);
        private:
                int                     respond_report(size_t bytes);
                int                     read_sync(size_t bytes);

};

#endif /* protocols/carma.h */
