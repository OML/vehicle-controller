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

#include <fcntl.h>
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

ssize_t file::peek(char* buffer, size_t size)
{
        return -1; // Not supported
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



int file::open(const std::string& path, unsigned int flags)
{
        file::flags &= ~FF_SELECT_READ;

        int oflags = 0;

        if(flags & F_CREATE)
                oflags |= O_CREAT;
        if(flags & F_READWRITE)
                oflags |= O_RDWR;
        if(flags & F_READONLY)
                oflags |= O_RDONLY;
        if(flags & F_WRITEONLY)
                oflags |= O_WRONLY;

        int fd = ::open(path.c_str(), oflags);
        if(fd == -1)
                return -1;
        set_fd(fd);
        return 0;
}
