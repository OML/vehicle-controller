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

#ifndef _FILE_H
#define _FILE_H

#include <sys/types.h>

#include <memory>

enum
{
        FF_SELECT_READ,
};

class file
{
        public:
                file(int fd);

                unsigned int    flags;

                virtual size_t read(char* buffer, size_t size);
                virtual size_t write(const char* buffer, size_t size);

                virtual void data_available();
        friend class event_loop;
        protected:
                int             fd;
};

#endif /* file.h */
