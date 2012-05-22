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

#include <file.h>
#include <ufile.h>

#include <fcntl.h>

file::file(): ufile()
{
}

file::file(const std::string& path): ufile()
{
        open(path);
}

int file::open(const std::string& path, file_flags_t flags)
{
        file::path = path;
        int oflags = 0;

        if(flags & FILE_CREATE)
                oflags |= O_CREAT;
        if(flags & FILE_READWRITE)
                oflags |= O_RDWR;
        if(flags & FILE_READONLY)
                oflags |= O_RDONLY;
        if(flags & FILE_WRITEONLY)
                oflags |= O_WRONLY;

        int fd = ::open(path.c_str(), oflags);
        if(fd == -1)
                return -1;
        set_fd(fd);
        return 0;
}
