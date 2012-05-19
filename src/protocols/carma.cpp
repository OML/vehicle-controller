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
#include <cstring>


#include <dirent.h>
#include <endian.h>


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

int carma::calibrate(uint16_t motors[4])
{
        char buffer[2];
        int retval;
        if((retval = MAINBOARD->calibrate(motors)) == 0) {
                buffer[0] = COP_OK;
                return cl->write(buffer, 1);
        } else {
                buffer[0] = COP_REJECT;
                buffer[1] = retval;
                return cl->write(buffer, 2);
        }
}

int carma::read_sync(size_t bytes)
{
        carma_t2c_sync_request pack;
        size_t size = cl->read((char*)&pack, sizeof(carma_t2c_sync_request));
        if(size < sizeof(carma_t2c_sync_request)) {
                std::cout << "Protocol error - Invalid packet size" << std::endl;
                return -1;
        }

        if(pack.calibrate == 1) {
                pack.motors[0] = le16toh(pack.motors[0]);
                pack.motors[1] = le16toh(pack.motors[1]);
                pack.motors[2] = le16toh(pack.motors[2]);
                pack.motors[3] = le16toh(pack.motors[3]);
                return calibrate(pack.motors);
        }
        return 0;
}

int carma::respond_report(size_t bytes)
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

        psize = 5; // Opcode, padding, version, number of files
        for(auto f: files)
                psize += f.length() + 1;

        buffer = new char[psize];

        pos = 0;
        buffer[pos++] = COP_REPORT;     // Opcode
        buffer[pos++] = 0;              // Padding
        buffer[pos++] = CARMA_VERSION;  // Protocol version
        buffer[pos++] = CARMA_REVISION; // Protocol revision
        buffer[pos++] = files.size();

        for(auto f: files) {
                buffer[pos++] = f.length();
                memcpy(&buffer[pos], f.c_str(), f.length());
                pos += f.length();
        }

        if(cl->write(buffer, 0) < 0) {
                std::cout << "Protocol error - Write fail" << std::endl;
                goto err;
        }


        delete [] buffer;
        return 0;
err:
        delete [] buffer;
        return -1;
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
                case COP_REPORT:
                        if(respond_report(bytes) < 0)
                                return -1;
                        break;
                default:
                        std::cout << "Protocol error - Invalid opcode (" << opcode << ")" << std::endl;
                        return -1;
        }

        return 0;
}
