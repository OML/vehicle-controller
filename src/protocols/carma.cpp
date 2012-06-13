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

static int print_err_packet_size(size_t have, size_t expect)
{
        std::cout << "Protocol error - Invalid packet size. Have " << have << ", expect " << expect << "." << std::endl;
        return -1;
}

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
        carma_response resp;

        int retval;
        if((retval = MAINBOARD->calibrate(motors)) == 0) {
                resp.opcode.op = COP_OK;
                resp.reason = 0;
                return cl->write(reinterpret_cast<char*>(&resp), sizeof(resp));
        } else {
                resp.opcode.op = COP_REJECT;
                resp.reason = retval;
                return cl->write(reinterpret_cast<char*>(&resp), sizeof(resp));
        }
}

int carma::read_keepalive()
{
        carma_keepalive_request req;
        size_t size = cl->read(reinterpret_cast<char*>(&req), sizeof(req));
        if(size != sizeof(carma_keepalive_request))
                return print_err_packet_size(size, sizeof(carma_keepalive_request));

        cl->write(reinterpret_cast<char*>(&req), sizeof(req));
        return 0;
}

int carma::read_sync()
{
        carma_sync_request pack;
        size_t size = cl->read((char*)&pack, sizeof(carma_sync_request));
        if(size < sizeof(carma_sync_request))
                return print_err_packet_size(size, sizeof(carma_sync_request));


        if(pack.calibrate == 0xFF) {
                pack.motors[0] = le16toh(pack.motors[0]);
                pack.motors[1] = le16toh(pack.motors[1]);
                pack.motors[2] = le16toh(pack.motors[2]);
                pack.motors[3] = le16toh(pack.motors[3]);
                calibrate(pack.motors);
        } 
        MAINBOARD->set_throttle((pack.speed == 1), pack.left, pack.right);
       
        return 0;
}

int carma::read_report()
{
        carma_report_request req;
        if(cl->bytes_available() < sizeof(carma_report_request))
                return print_err_packet_size(cl->bytes_available(), sizeof(carma_keepalive_request));

        size_t size = cl->read((char*)&req, sizeof(carma_report_request));
        if(size != sizeof(carma_report_request)) {
                std::cout << "System error - Unable to read buffer" << std::endl;
                return -1;
        }
        event_thresholds thresh;
        thresh.throttle_threshold = le16toh(req.thresholds.throttle_threshold);
        thresh.voltage_threshold = le16toh(req.thresholds.voltage_threshold);
        thresh.current_threshold = le16toh(req.thresholds.current_threshold);
        thresh.temperature_threshold = le16toh(req.thresholds.temperature_threshold);
        thresh.gyro_threshold = le16toh(req.thresholds.gyro_threshold);
        MAINBOARD->set_thresholds(thresh);




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

        psize = sizeof(carma_report_response_header);
        for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
                psize += (*it).length() + 1;
        buffer = new char[psize];

        struct carma_report_response_header* hdr;
        hdr = reinterpret_cast<carma_report_response_header*>(buffer);

        hdr->opcode.op = COP_REPORT;
        hdr->prot_ver = CARMA_VERSION;
        hdr->prot_rev = CARMA_REVISION;
        hdr->files_len = files.size();

        pos = sizeof(carma_report_response_header);

        for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++) {
                buffer[pos++] = (*it).length();
                memcpy(&buffer[pos], (*it).c_str(), (*it).length());
                pos += (*it).length();
        }

        if(cl->write(buffer, psize) < 0) {
                std::cout << "Protocol error - Write fail" << std::endl;
                goto err;
        }


        delete [] buffer;
        return 0;
err:
        delete [] buffer;
        return -1;
}

int carma::start_reading()
{
//        std::cout << "System buffer has " << cl->bytes_available() << " bytes available." << std::endl;
        carma_opcode opcode;
        if(cl->peek((char*)&opcode, sizeof(carma_opcode)) == -1) {
                std::cout << "System error - Unable to peek" << std::endl;
                return -1;
        }
        switch(opcode.op) {
                case COP_SYNC:
//                        std::cout << "Read sync packet" << std::endl;
                        if(read_sync() < 0)
                                return -1;
                        break;
                case COP_REPORT:
//                        std::cout << "Read report packet" << std::endl;
                        if(read_report() < 0)
                                return -1;
                        break;
                case COP_KEEPALIVE:
//                        std::cout << "Read keepalive packet" << std::endl;
                        if(read_keepalive() < 0)
                                return -1;
                        break;
                default:
                        std::cout << "Protocol error - Invalid opcode (" << static_cast<short>(opcode.op) << ")" << std::endl;
                        return -1;
        }

        return 0;
}
