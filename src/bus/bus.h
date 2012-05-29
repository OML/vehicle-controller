#ifndef _SRC_BUS_H
#define _SRC_BUS_H


#include "busprot.h"

#define N_BUSSES 4
#define BUS_TIMEOUT 250

struct bus_node
{
        struct bus_node* next;

        int devtype;
        uint16_t addr;
};

struct bus_descriptor
{
        struct uart_endpoint* uart;
        
        struct bus_node* layout;       
};

extern struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void);
void bus_do_work(void);

#endif /* src/bus.h */
