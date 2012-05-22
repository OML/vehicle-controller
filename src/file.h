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
        FF_SELECT_READ = 0x01,
};

class file
{
        public:
                file(int fd = -1);

                virtual ssize_t read(char*, size_t size);
                virtual ssize_t write(const char* buffer, size_t size);

                /*
                 * Returns -1 if peek is not supported on this file
                 */
                virtual ssize_t peek(char* buffer, size_t size);

                virtual int close();

                virtual void data_available();
                virtual size_t bytes_available();
        friend class event_loop;
        protected:
                void            set_fd(int fd);
                int             fd;
                unsigned int    flags;
};

#endif /* file.h */
