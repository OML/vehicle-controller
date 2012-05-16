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

#include "unixpipe.h"

unixpipe::unixpipe(): file()
{

}

unixpipe::unixpipe(int fd): file(fd)
{
}

std::pair<unixpipe*, unixpipe*> unixpipe::create_pair()
{
        int pipes[2];
        if(pipe(pipes) < 0)
                return {nullptr, nullptr};
        return {new unixpipe(pipes[0]), new unixpipe(pipes[1])};
}
