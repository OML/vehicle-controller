#ifndef _PROT_TAB2CAR_H
#define _PROT_TAB2CAR_H

#include <cstdint>

enum tab2car_cmds
{
	T2C_CMD_WRITE,
};

struct tab2car_motor
{
	uint16_t	throttle;	// promille
	uint16_t	voltage;	// millivolts
	uint16_t	current;	// milliamperes
	uint16_t	temperature;	// millidegrees
};

struct tab2car_packet
{
	uint8_t		cmd;		// T2C_CMD_WRITE
	union {
		struct {
			struct {
				int8_t		x;
				int8_t		y;
			}		steer;
			struct {
				
		}	payload;
	};
	
};

#endif /* prot_tab2car.h */
