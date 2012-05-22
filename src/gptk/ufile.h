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

#ifndef _SRC_UFILE_H
#define _SRC_UFILE_H

#include <sys/types.h>


typedef unsigned int ufile_event_mask_t;
enum
{
        UFILE_EVENT_READ = 0x01,
        UFILE_EVENT_WRITE = 0x02,
};

class ufile
{
        public:
                ufile(int fd = -1);
                virtual ~ufile();

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

                ufile_event_mask_t event_mask;
                void set_event_mask(ufile_event_mask_t mask);
};

#endif /* src/gptk/ufile.h */
