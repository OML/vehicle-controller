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

#include "bus/busprot.h"

mainboard* mainboard::instance = NULL;

mainboard::mainboard(const std::string& sfile): ufile(), sfile(sfile)
{
        instance = this;
        set_event_mask(UFILE_EVENT_READ);

        config = new config_file("carmen.cfg");
        for(int i = 0; i < NMOTORS; i++)
                motor_multiplier[i] = 1.0f;

        open_fifo();

        motor_front_addr = 1;
        motor_back_addr = 2;

        read_buffer = NULL;
        read_buffer_length = 0;

        std::cout << "Mainboard fd: " << fd << std::endl;
}

mainboard::~mainboard()
{
        delete config;
}

void mainboard::open_fifo()
{
        int fd = ::open(sfile.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        struct termios opts;
        ioctl(fd, TCGETS, &opts);
        opts.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                                | INLCR | IGNCR | ICRNL | IXON);
        opts.c_oflag &= ~OPOST;
        opts.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        opts.c_cflag &= ~(CSIZE | PARENB);
        opts.c_cflag |= CS8 | CREAD | B19200;

        ioctl(fd, TCSETS, &opts);

        set_fd(fd);

	acquire_address();
}

void mainboard::read_buffer_append(const char* data, size_t len)
{
	if(read_buffer == NULL) {
		read_buffer = new char[len];
		memcpy(read_buffer, data, len);
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

        read_buffer_append(tmp, len);

        if(read_buffer_length >= 2) {
                len = *reinterpret_cast<uint16_t*>(read_buffer);
                if(read_buffer_length >= len) {
			std::cout << "Process data" << std::endl;
                        process_packet(read_buffer + sizeof(uint16_t));
                        delete [] read_buffer;
                        read_buffer = NULL;
                        read_buffer_length = 0;
                }
        }
}

static struct bus_hdr* get_bus_header(char* data)
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

void mainboard::process_hello(const char* data)
{
        bus_hdr* header = get_bus_header(const_cast<char*>(data));

        my_addr = le16toh(header->daddr);
        host_addr = le16toh(header->saddr);

        size_t resp_len = (size_t)get_bus_hello_reply(NULL) + sizeof(bus_hello_reply);
        char response_buffer[resp_len];
        header = get_bus_header(response_buffer);
        bus_hello_reply* reply = get_bus_hello_reply(response_buffer);

        header->saddr = htole16(my_addr);
        header->daddr = htole16(host_addr);
        header->opcode.op = htole16(BUSOP_HELLO);
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
        struct bus_opc ok;
        uint16_t size = htole16(len) + sizeof(uint16_t);
        write((char*)&size, sizeof(uint16_t));
        write(data, len);

        read((char*)&ok, sizeof(ok));
}
void mainboard::process_packet(const char* data)
{
        struct bus_hdr* hdr = (struct bus_hdr*)data;

        switch(hdr->opcode.op) {
                case BUSOP_HELLO:
                        std::cout << "mainboard::process_packet(): Hello packet" << std::endl;
                        process_hello(data);
                        break;
        }
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
        size_t psize = (size_t)get_bus_set_motor_driver(NULL) + sizeof(bus_set_motor_driver);
        char buffer[psize];
        bus_hdr* bhdr = get_bus_header(buffer);
        bus_event_hdr* evhdr = get_bus_event_header(buffer);
        bus_set_motor_driver* drv = get_bus_set_motor_driver(buffer);

        bhdr->opcode.op = BUSOP_EVENT;
        bhdr->saddr = htole16(my_addr);
        bhdr->daddr = htole16(motor_front_addr);

        evhdr->timestamp = 0;
        evhdr->type = htole16(EV_SET_THROTTLES);

        drv->motors[MOTOR_LEFT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_FRONT_LEFT] * left);
        drv->motors[MOTOR_RIGHT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_FRONT_RIGHT] * right);

        drv->motors[MOTOR_LEFT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_BACK_LEFT] * left);
        drv->motors[MOTOR_RIGHT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_BACK_RIGHT] * right);

        bus_write(buffer, psize);

        return 0;
}


int mainboard::halt()
{
#warning "mainboard::set_halt not implemented yet."
        std::cout << "mainboard::set_halt not implemented yet." << std::endl;
        return -1;
}
