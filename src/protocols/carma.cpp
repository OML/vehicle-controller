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

#include "carma.h"

#include <iostream>
#include <vector>
#include <dirent.h>
#include <cstring>


#include "client.h"

#include "mainboard.h"

carma::carma(client* c): protocol(c)
{

}

carma::~carma()
{

}

int carma::init()
{
        return -1;
}

int carma::disconnect()
{
        return -1;
}

int carma::read_sync(size_t bytes)
{
        carma_t2c_sync_request pack;
        size_t size = cl->read((char*)&pack, sizeof(carma_t2c_sync_request));
        if(size < sizeof(carma_t2c_sync_request)) {
                std::cout << "Protocol error - Invalid packet size" << std::endl;
                return -1;
        }
        MAINBOARD->write((char*)&pack, sizeof(carma_t2c_sync_request));



        carma_c2t_sync_response res;
        res.opcode = COP_SYNC;
        res.motors[0].throttle = 42;
        res.motors[0].voltage = 0;
        res.motors[0].current = 0;
        res.motors[0].temperature = 1000;
        res.motors[3] = res.motors[2]
                = res.motors[1] = res.motors[0];
        res.gyro[0] = 32;
        res.gyro[1] = 23;
        res.gyro[2] = 5;
        res.accu_voltage = 48000;
        res.accu_current = 60000;

        cl->write((char*)&pack, sizeof(carma_c2t_sync_response));

        return 0;
}

int carma::read_list_names(size_t bytes)
{
        std::string path = "/home/leon/workspace/vehicle-controller/resources/sounds/";
        std::vector<std::string> files;
        char* buffer;
        DIR* dp;
        struct dirent* dirp;
        size_t pos, psize;

        if((dp = opendir(path.c_str())) == NULL) {
                std::cout << "Unable to open sound directory" << std::endl;
                return -1;
        }

        while((dirp = readdir(dp)) != NULL) {
                files.push_back(std::string(dirp->d_name));
        }

        closedir(dp);

        psize = 2; // Opcode, number of files
        for(auto f: files)
                psize += f.length() + 1;

        buffer = new char[psize];

        pos = 0;
        buffer[pos++] = COP_LIST_FILES;
        buffer[pos++] = files.size();
        for(auto f: files) {
                buffer[pos++] = f.length();
                memcpy(&buffer[pos], f.c_str(), f.length());
                pos += f.length();
        }

        if(cl->write(buffer, 0) < 0) {
                std::cout << "Protocol error - Write fail" << std::endl;
                return -1;
        }

        return 0;
}

int carma::start_reading(size_t bytes)
{
        std::cout << "Carma start reading" << std::endl;

        uint8_t opcode;
        if(cl->read((char*)&opcode, 1) == -1) {
                std::cout << "Protocol error" << std::endl;
                return -1;
        }

        switch(opcode) {
                case COP_SYNC:
                        if(read_sync(bytes) < 0)
                                return -1;
                        break;
                case COP_LIST_FILES:
                        if(read_list_names(bytes) < 0)
                                return -1;
                        break;
                default:
                        std::cout << "Protocol error - Invalid opcode (" << opcode << ")" << std::endl;
                        return -1;
        }

        return 0;
}
