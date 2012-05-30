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
}

void mainboard::data_available()
{
        uint16_t len = bytes_available();

        char tmp[len];
        read(tmp, len);

        if(read_buffer == NULL) {
                read_buffer = new char[len];
                memcpy(read_buffer, tmp, len);
                read_buffer_length = len;
        } else {
                char* tmp_buffer = new char[len + read_buffer_length];
                memcpy(tmp_buffer, read_buffer, read_buffer_length);
                memcpy(tmp_buffer + read_buffer_length, tmp, len);
                free(read_buffer);
                read_buffer = tmp_buffer;
        }

        if(read_buffer_length >= 2) {
                len = *reinterpret_cast<uint16_t*>(read_buffer);
                if(read_buffer_length >= len) {
                        process_packet(read_buffer);
                        free(read_buffer);
                        read_buffer = NULL;
                        read_buffer_length = 0;
                }
        }
}

void mainboard::process_hello(const char* data)
{
        struct bus_hello* pack = (struct bus_hello*)data;
        my_addr = le16toh(pack->hdr.daddr);
        host_addr = le16toh(pack->hdr.saddr);

        struct bus_hello_reply repl;
        repl.hdr.saddr = htole16(my_addr);
        repl.hdr.daddr = htole16(host_addr);
        repl.hdr.opcode.op = htole16(BUSOP_HELLO);
        repl.devtype = htole16(DT_IPC);

        bus_write((char*)&repl, sizeof(repl));
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
        struct bus_opc* opc = (struct bus_opc*)data;

        switch(opc->op) {
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
        struct bus_set_motor_driver front, back;
        back.hdr.opcode.op = front.hdr.opcode.op = BUSOP_EVENT;
        back.hdr.saddr = front.hdr.saddr = my_addr;
        front.hdr.daddr = motor_front_addr;
        back.hdr.daddr = motor_back_addr;
        back.event.timestamp = front.event.timestamp = 0;
        back.event.type = front.event.type = EV_SET_THROTTLES;

        front.motors[MOTOR_LEFT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_FRONT_LEFT] * left);
        front.motors[MOTOR_RIGHT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_FRONT_RIGHT] * right);

        back.motors[MOTOR_LEFT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_BACK_LEFT] * left);
        back.motors[MOTOR_RIGHT] = static_cast<throttle_t>(
                        motor_multiplier[MOTOR_BACK_RIGHT] * right);

        bus_write((char*)&front, sizeof(front));
        bus_write((char*)&back, sizeof(back));

        return 0;
}


int mainboard::halt()
{
#warning "mainboard::set_halt not implemented yet."
        std::cout << "mainboard::set_halt not implemented yet." << std::endl;
        return -1;
}
