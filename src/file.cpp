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

#include "file.h"

#include "event_loop.h"

#include <sys/ioctl.h>

file::file(int fd): fd(fd)
{
        flags = FF_SELECT_READ;
        EVL->register_file(this);
}

ssize_t file::read(char* buffer, size_t size)
{
        return ::read(fd, buffer, size);
}


ssize_t file::write(const char* buffer, size_t size)
{
        return ::write(fd, buffer, size);
}

int file::close()
{
        ::close(fd);
        set_fd(-1);
        EVL->flush();
        return 0;
}

void file::data_available()
{
        //flags &= ~FF_SELECT_READ;
}

size_t file::bytes_available()
{
        size_t size;
        if(::ioctl(fd, FIONREAD, (char*)&size) < 0)
                return 0;
        return size;
}

void file::set_fd(int f)
{
        fd = f;
        EVL->flush();
}
