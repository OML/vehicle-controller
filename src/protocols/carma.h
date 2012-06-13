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

#include <stdint.h>
#include <sys/types.h>

// Supported version
#define CARMA_VERSION   0
#define CARMA_REVISION  3

typedef int16_t gyro_t;

class client;

enum
{
        COP_REJECT = 0,
        COP_OK,
        COP_SYNC,
        COP_REPORT,
        COP_KEEPALIVE,
};

enum
{
        C_REJECT_REASON_UNKONWN,
        C_REJECT_REASON_INVALID
};

struct carma_opcode
{
        uint8_t op;
} __attribute__((packed));


struct carma_sync_request
{
        carma_opcode opcode;
        uint8_t speed;
        int8_t left;
        int8_t right;
        uint8_t calibrate;
        uint16_t motors[4];
        uint8_t sound;

}__attribute__((packed));

struct carma_sync_response
{
        carma_opcode opcode;
        uint32_t timestamp;
        motor_data motors[4];
        gyro_t gyro[3];
        accu_data accu;
} __attribute__((packed));

struct carma_report_request
{
        carma_opcode opcode;
        uint8_t padding;
        event_thresholds thresholds;
} __attribute__((packed));

struct carma_keepalive_request
{
        carma_opcode opcode;
        uint8_t padding;
} __attribute__((packed));


struct carma_report_response_header
{
        carma_opcode opcode;
        uint8_t padding;

        uint8_t prot_ver;
        uint8_t prot_rev;

        uint8_t files_len;
} __attribute__((packed));

struct carma_response
{
        carma_opcode opcode;
        uint8_t reason;
} __attribute__((packed));

class carma: public protocol
{
        public:
                carma(client* c);
                ~carma();

                int                     init();
                int                     disconnect();
                int                     start_reading();

                int                     calibrate(uint16_t motors[4]);
        private:
                int                     read_report();
                int                     read_sync();
                int                     read_keepalive();

};

#endif /* protocols/carma.h */
