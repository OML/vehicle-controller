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

#ifndef _SRC_GPTK_FILE_H
#define _SRC_GPTK_FILE_H

#include <ufile.h>

#include <string>


typedef unsigned int file_flags_t;
enum
{
        FILE_CREATE = 0x01,
        FILE_READONLY = 0x02,
        FILE_WRITEONLY = 0x04,
        FILE_READWRITE = 0x08,
};


typedef unsigned int file_flags_t;

class file: public ufile
{
        public:
                file();
                file(const std::string& path);


                int open(const std::string& path, file_flags_t flags = FILE_CREATE | FILE_READWRITE);
        private:
                std::string path;
};

#endif /* src/gptk/file.h */
