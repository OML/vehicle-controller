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

file::file(int fd): fd(fd)
{
        flags = FF_SELECT_READ;
        EVL->register_file(std::shared_ptr<file>(this));
}

size_t file::read(std::shared_ptr<char> buffer, size_t size)
{
        return ::read(fd, &*buffer, size);
}


size_t file::write(std::shared_ptr<const char> buffer, size_t size)
{
        return ::write(fd, &*buffer, size);
}

void file::data_available()
{
        flags &= ~FF_SELECT_READ;
}
