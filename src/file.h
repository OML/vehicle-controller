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

#include <string>

enum
{
        FF_SELECT_READ = 0x01,
};

enum
{
        F_CREATE = 0x01,
        F_READONLY = 0x02,
        F_WRITEONLY = 0x04,
        F_READWRITE = 0x08,
};

class file
{
        public:
                file(int fd = -1);
                virtual ~file();

                virtual ssize_t read(char*, size_t size);
                virtual ssize_t write(const char* buffer, size_t size);

                /*
                 * Returns -1 if peek is not supported on this file
                 */
                virtual ssize_t peek(char* buffer, size_t size);

                virtual int close();

                virtual void data_available();
                virtual size_t bytes_available();



                int open(const std::string& path, unsigned int flags = F_CREATE | F_READWRITE);
        friend class event_loop;
        protected:
                void            set_fd(int fd);
                int             fd;
                unsigned int    flags;
};

#endif /* file.h */
