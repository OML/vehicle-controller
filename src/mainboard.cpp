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

mainboard* mainboard::instance = NULL;

mainboard::mainboard(const char* sfile): ufile()
{
        instance = this;


        //config = std::unique_ptr<config_file>(new config_file("carmen.cfg"));
        for(int i = 0; i < NMOTORS; i++)
                motor_multiplier[i] = 1.0f;

        std::cout << "Mainboard fd: " << fd << std::endl;
}

void mainboard::data_available()
{

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

int mainboard::set_tresholds(const event_tresholds& tresh)
{
#warning "mainboard::set_tresholds not implemented yet."
        std::cout << "mainboard::set_tresholds not implemented yet." << std::endl;
        return -1;
}

int mainboard::set_throttle(bool fast, int left, int right)
{
#warning "mainboard::set_throttle not implemented yet."
        std::cout << "mainboard::set_halt not implemented yet." << std::endl;
        return -1;
}


int mainboard::halt()
{
#warning "mainboard::set_halt not implemented yet."
        std::cout << "mainboard::set_halt not implemented yet." << std::endl;
        return -1;
}
