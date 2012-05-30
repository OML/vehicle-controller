#ifndef _BUSPROT_H
#define _BUSPROT_H

#include <stdint.h>

#include "protocols/general.h"

typedef uint16_t bus_addr_t;
typedef uint16_t bus_devtype_t;
typedef uint16_t bus_event_type_t;
typedef uint32_t bus_timestamp_t;

// Device types
enum {
        DT_MOTOR,
        DT_DUAL_MOTOR,
        DT_ACCU,
        DT_IPC,
};

// Opcodes
enum
{
        BUSOP_HELLO = 0,
        BUSOP_EVENT,
        BUSOP_DONE,
};


// Events
enum
{
        EV_SET_THROTTLES,
};

struct bus_opc
{
        uint8_t op;
} __attribute__((packed));


struct bus_hdr
{
        struct bus_opc opcode;
        bus_addr_t saddr;
        bus_addr_t daddr;
} __attribute__((packed));


struct bus_hello
{
} __attribute__((packed));


struct bus_hello_reply
{
        bus_devtype_t devtype;
} __attribute__((packed));

struct bus_event_hdr
{
        bus_timestamp_t timestamp;
        bus_event_type_t type;
} __attribute__((packed));

struct bus_set_thresholds
{
        struct event_thresholds thresholds;
} __attribute__((packed));


struct bus_set_motor_driver
{
        throttle_t motors[2];
} __attribute__((packed));

#endif /* busprot.h */
