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

#include <ufile.h>
#include <event_loop.h>

#include <sys/ioctl.h>


ufile::ufile(int fd): fd(fd)
{
        set_event_mask(0);
        EVL->register_file(this);
}

ufile::~ufile()
{
        EVL->unregister_file(this);
        if(fd > 0)
                close();
}

void ufile::set_event_mask(ufile_event_mask_t mask)
{
        event_mask = mask;
        if(fd)
                EVL->flush();
}

ssize_t ufile::read(char* buffer, size_t size)
{
        return ::read(fd, buffer, size);
}

ssize_t ufile::peek(char* buffer, size_t size)
{
        return -1; // Not supported
}

ssize_t ufile::write(const char* buffer, size_t size)
{
        return ::write(fd, buffer, size);
}

int ufile::close()
{
        ::close(fd);
        set_fd(-1);
        EVL->flush();
        return 0;
}

void ufile::data_available()
{
        //flags &= ~FF_SELECT_READ;
}

size_t ufile::bytes_available()
{
        size_t size;
        if(::ioctl(fd, FIONREAD, (char*)&size) < 0)
                return 0;
        return size;
}

void ufile::set_fd(int f)
{
        fd = f;
        EVL->flush();
}

void ufile::defer_delete()
{
        event_loop::event ev;
        ev.type = event_loop::EV_DEFER_DELETE;
        ev.sender = this;
        EVL->post_event(ev);
}
