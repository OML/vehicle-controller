#ifndef _BUSPROT_TYPES_H
#define _BUSPROT_TYPES_H

#include <stdint.h>


typedef uint16_t voltage_t;
typedef uint16_t current_t;
typedef int16_t temperature_t;
typedef int16_t throttle_t;
typedef int16_t gyro_t;



typedef uint16_t bus_addr_t;
typedef uint16_t bus_devtype_t;
typedef uint16_t bus_event_type_t;
typedef uint32_t bus_timestamp_t;



/* Device types */
enum {
        DT_NULL,
        DT_ROOT,
        DT_DUAL_MOTOR_FRONT,
        DT_DUAL_MOTOR_BACK,
        DT_ACCU_1,
        DT_ACCU_2,
        DT_IPC,
        DT_IO,
};

/* Opcodes */
enum
{
        BUSOP_HELLO = 0,
        BUSOP_EVENT,
        BUSOP_DONE,
        BUSOP_ACQUIRE_ADDRESS,
        BUSOP_SET_THRESHOLDS
};


/* Events */
enum
{
        EV_SET_THROTTLES,
        EV_SET_OUTPUTS,
        EV_MOTOR_SENSORS,
};

struct bus_opc
{
        uint8_t op;
} __attribute__((packed));


struct bus_hdr
{
        uint16_t len;
        uint16_t crc;

        struct bus_opc opcode;
        bus_addr_t saddr;
        bus_addr_t daddr;
        bus_devtype_t dtype;
        bus_devtype_t stype;
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
        throttle_t throttle_threshold;
        voltage_t voltage_threshold;
        current_t current_threshold;
        temperature_t temperature_threshold;
        gyro_t gyro_threshold;
} __attribute__((packed));


struct bus_set_motor_driver
{
        throttle_t motors[2];
} __attribute__((packed));

struct bus_set_outputs
{
        uint8_t bits;
} __attribute__((packed));

struct bus_motor_sensors
{
        voltage_t voltage;
        current_t current;
        temperature_t temperature;
} __attribute__((packed));

struct bus_motor_sensors_event
{
        struct bus_motor_sensors sensors[2];
} __attribute__((packed));

struct bus_set_digital_io_event
{
        uint8_t bits;
} __attribute__((packed));

#endif /* busprot_types.h */
