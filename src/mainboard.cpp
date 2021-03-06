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

#include "mainboard.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "bus/bus_types.h"

#include "client.h"

mainboard* mainboard::instance = NULL;

mainboard::mainboard(const std::string& sfile): ufile(), sfile(sfile)
{
        instance = this;
        set_event_mask(UFILE_EVENT_READ);

        config = new config_file("carmen.cfg");
        for(int i = 0; i < NMOTORS; i++)
                motor_multiplier[i] = 1.0f;

        open_fifo();

        motor_front_addr = 3;
        motor_back_addr = 5;

        read_buffer = NULL;
        read_buffer_length = 0;

        my_addr = -1;
        host_addr = -1;

	connected = false;

	memset(motors_buffer, 0, sizeof(motors_buffer));
}

mainboard::~mainboard()
{
        delete config;
}

void mainboard::open_fifo()
{
        int fd = ::open(sfile.c_str(), O_RDWR | O_NOCTTY);
        struct termios opts;
        ioctl(fd, TCGETS, &opts);

        opts.c_iflag &= ~(IGNBRK | BRKINT | ICRNL
		| INLCR | PARMRK | INPCK | ISTRIP | IXON);
	opts.c_oflag = 0;
	opts.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	opts.c_cflag &= ~(CSIZE | PARENB);
	opts.c_cflag |= CS8 | CREAD | B38400;
	opts.c_cc[VMIN] = 1;
	opts.c_cc[VTIME] = 0;

        ioctl(fd, TCSETS, &opts);

        set_fd(fd);

	acquire_address();
}

void mainboard::read_buffer_append(const char* data, size_t len)
{
	if(read_buffer == NULL) {
		read_buffer = new char[len];
		memcpy(read_buffer, data, len);
		read_buffer_length = len;
	} else {
		char* tmp = read_buffer;
		read_buffer =  new char[len + read_buffer_length];
		memcpy(read_buffer, tmp, read_buffer_length);
		memcpy(read_buffer + read_buffer_length, data, len);
		read_buffer_length += len;
		delete [] tmp;
	}
}


void mainboard::data_available()
{
        uint16_t len = bytes_available();

        char tmp[len];
        read(tmp, len);
	if(len == 1 && tmp[0] == '\0' && read_buffer_length == 0) { // spurious disconnect byte
	        std::cout << "Mainboard disconnected" << std::endl;
		my_addr = host_addr = -1;
		delete [] read_buffer;
		read_buffer = NULL;
		read_buffer_length = 0;

		connected = false;
		return;
	}

        read_buffer_append(tmp, len);
        if(read_buffer_length >= 2) {
                len = *reinterpret_cast<uint16_t*>(read_buffer);
//		std::cout << "Reading " << len << "/" << read_buffer_length << std::endl;
		if(read_buffer_length >= len) {
                        size_t rlen = process_packet(read_buffer);
			char* tmp = read_buffer;
			
			if(read_buffer_length == rlen) {
				delete[] read_buffer;
				read_buffer = NULL;
				read_buffer_length = 0;
			} else {
				int new_length = read_buffer_length - rlen;
				read_buffer = new char[new_length];
				memcpy(read_buffer, tmp + rlen, new_length);
				read_buffer_length = new_length;
				delete [] tmp;
			}
                }
        }
}

static struct bus_hdr* get_bus_header(const char* data)
{
        return (bus_hdr*)data;
}

static struct bus_hello* get_bus_hello(char* data)
{
        return (bus_hello*)((char*)get_bus_header(data) + sizeof(bus_hdr));
}

static struct bus_hello_reply* get_bus_hello_reply(char* data)
{
        return (bus_hello_reply*)((char*)get_bus_header(data) + sizeof(bus_hdr));
}

static struct bus_event_hdr* get_bus_event_header(char* data)
{
        return (bus_event_hdr*)((char*)get_bus_header(data) + sizeof(bus_hdr));
}

static struct bus_set_motor_driver* get_bus_set_motor_driver(char* data)
{
        return (bus_set_motor_driver*)((char*)get_bus_event_header(data) + sizeof(bus_event_hdr));
}

static struct bus_motor_sensors_event* get_bus_motor_sensors_event(char* data)
{
	return (bus_motor_sensors_event*)((char*)get_bus_event_header(data) + sizeof(bus_event_hdr));
}

static struct bus_set_digital_io_event* get_bus_set_digital_io_event(char* data)
{
        return (bus_set_digital_io_event*)((char*)get_bus_event_header(data) + sizeof(bus_event_hdr));
}

void mainboard::process_hello(const char* data)
{
        bus_hdr* header = get_bus_header(const_cast<char*>(data));

        //std::cout << "Processing HELLO packet" << std::endl;

        if(le16toh(header->daddr) > my_addr) {
           //     std::cout << "\tIgnoring" << std::endl;
                return;
        }

	connected = true;

        my_addr = le16toh(header->daddr);
        host_addr = le16toh(header->saddr);

        size_t resp_len = (size_t)get_bus_hello_reply(NULL) + sizeof(bus_hello_reply);
        char response_buffer[resp_len];
        header = get_bus_header(response_buffer);
        bus_hello_reply* reply = get_bus_hello_reply(response_buffer);

        header->saddr = htole16(my_addr);
        header->daddr = htole16(host_addr);
        header->opcode.op = htole16(BUSOP_HELLO);
        header->stype = htole16(DT_IPC);
        reply->devtype = htole16(DT_IPC);

        bus_write(response_buffer, resp_len);
}

void mainboard::acquire_address()
{
        size_t req_size = (size_t)get_bus_header(NULL) + sizeof(bus_hdr);
        char req[req_size];
        bus_hdr* header = get_bus_header(req);
        header->opcode.op = BUSOP_ACQUIRE_ADDRESS;
        header->daddr = 0;
        header->saddr = 0;

        bus_write(req, req_size);
}

void mainboard::bus_write(const char* data, size_t len)
{
        get_bus_header(data)->len = len;
	write(data, len);
}

int mainboard::process_packet(const char* data)
{
        char* data_nc = const_cast<char*>(data);

	connected = true;
        struct bus_hdr* hdr = (struct bus_hdr*)data;

        switch(hdr->opcode.op) {
                case BUSOP_HELLO:
                        process_hello(data);
                        break;
		case BUSOP_EVENT: {
		        unsigned int offset = 0;
		        if(get_bus_header(data_nc)->stype == DT_DUAL_MOTOR_BACK)
		                offset = 2;
		        bus_motor_sensors_event* ev = get_bus_motor_sensors_event(data_nc);
		        motors_buffer[offset+0].temperature = ev->sensors[0].temperature;
		        motors_buffer[offset+0].voltage = ev->sensors[0].voltage;
		        motors_buffer[offset+0].current = ev->sensors[0].current;

		        motors_buffer[offset+1].temperature = ev->sensors[1].temperature;
		        motors_buffer[offset+1].voltage = ev->sensors[1].voltage;
		        motors_buffer[offset+1].current = ev->sensors[1].current;

			std::cout << "Recv: " << ev->sensors[0].voltage << ", " << ev->sensors[1].voltage << std::endl;

//			std::cout << "Set:  " << motors_buffer[0].voltage << std::endl;
//			std::cout << "Rem:  " << motors_buffer[1].voltage << std::endl << motors[2].voltage << std::endl << motors[3].voltage << std::endl;
			client::incoming_motor_sensors_event(get_bus_event_header(data_nc)->timestamp, motors_buffer);
			break;
		}
                case BUSOP_DONE:
                        break;
        }
	return hdr->len;
}

int mainboard::calibrate(uint16_t speeds[NMOTORS])
{
        unsigned int min = -1;
        for(int i = 0; i < NMOTORS; i++)
                min = std::max(min, (unsigned int)speeds[i]);

        for(int i = 0; i < NMOTORS; i++)
                motor_multiplier[i] = static_cast<float>(min)/static_cast<float>(speeds[i]);

        return 0;
}

int mainboard::set_thresholds(const event_thresholds& thresh)
{
#warning "mainboard::set_thresholds not implemented yet."
        std::cout << "mainboard::set_thresholds not implemented yet." << std::endl;
        return -1;
}

int mainboard::set_throttle(bool fast, int left, int right)
{
//	uint16_t motors[4] = {100, 100, 100, 100};
//	calibrate((uint16_t*)&motors);
	std::cout << "Set throttles "  << left << ", " << right << std::endl;
	
        size_t psize = (size_t)get_bus_set_motor_driver(NULL) + sizeof(bus_set_motor_driver);
        char buffer[psize];
        bus_hdr* bhdr = get_bus_header(buffer);
        bus_event_hdr* evhdr = get_bus_event_header(buffer);
        bus_set_motor_driver* drv = get_bus_set_motor_driver(buffer);

        bhdr->opcode.op = BUSOP_EVENT;
        bhdr->saddr = htole16(my_addr);
        bhdr->daddr = htole16(0);
        bhdr->dtype = htole16(DT_DUAL_MOTOR_FRONT);
        bhdr->stype = htole16(DT_IPC);

        evhdr->timestamp = 0;
        evhdr->type = htole16(EV_SET_THROTTLES);

        drv->motors[MOTOR_LEFT] = htole16(left*10);//static_cast<throttle_t>(
                        //motor_multiplier[MOTOR_FRONT_LEFT] * left);
        drv->motors[MOTOR_RIGHT] = htole16(right*10);//static_cast<throttle_t>(
                        //motor_multiplier[MOTOR_FRONT_RIGHT] * right);
        bus_write(buffer, psize);

        bhdr->dtype = htole16(DT_DUAL_MOTOR_BACK);
        bhdr->daddr = htole16(0);

        drv->motors[MOTOR_LEFT] = htole16(left*10);//static_cast<throttle_t>(
                        //motor_multiplier[MOTOR_BACK_LEFT] * left);
        drv->motors[MOTOR_RIGHT] = htole16(right*10);//static_cast<throttle_t>(
                        //motor_multiplier[MOTOR_BACK_RIGHT] * right);
        bus_write(buffer, psize);

        return 0;
}

int mainboard::set_digital_outputs(uint8_t bits)
{
	//std::cout << "Set digital outputs " << std::hex << (int)bits << std::endl;
        size_t buflen = (size_t)((char*)get_bus_set_digital_io_event(NULL)) + sizeof(struct bus_set_digital_io_event);
        char buffer[buflen];
        struct bus_set_digital_io_event* ev = get_bus_set_digital_io_event(buffer);
        struct bus_event_hdr* evhdr = get_bus_event_header(buffer);
        struct bus_hdr* bhdr = get_bus_header(buffer);

        bhdr->stype = htole16(DT_IPC);
        bhdr->saddr = htole16(my_addr);
        bhdr->daddr = 0;
        bhdr->dtype = htole16(DT_IO);
	bhdr->opcode.op = htole16(BUSOP_EVENT);
	
        evhdr->timestamp = 0;
        evhdr->type = htole16(EV_SET_OUTPUTS);

        ev->bits = bits;

        bus_write(buffer, buflen);
        return 0;
}


int mainboard::halt()
{
#warning "mainboard::set_halt not implemented yet."
        std::cout << "mainboard::set_halt not implemented yet." << std::endl;
        return -1;
}
