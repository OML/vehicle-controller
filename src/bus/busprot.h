#ifndef _BUSPROT_H
#define _BUSPROT_H

#include <stdint.h>

typedef uint16_t bus_addr_t;
typedef uint16_t bus_devtype_t;
typedef uint16_t bus_event_type_t;


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
};

struct bus_opc
{
        uint8_t op;
} __attribute__((packed));


struct bus_header
{
        struct bus_opc opcode;
        bus_addr_t saddr;
        bus_addr_t daddr;
} __attribute__((packed));


struct bus_hello
{
        struct bus_header hdr;
} __attribute__((packed));


struct bus_hello_reply
{
        struct bus_header hdr;
        bus_devtype_t devtype;
} __attribute__((packed));

struct bus_event_header
{
        struct bus_header hdr;
        bus_event_type_t event_type;
} __attribute__((packed));

#endif /* busprot.h */
