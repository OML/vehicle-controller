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

#ifndef _SRC_CONFIG_FILE_H
#define _SRC_CONFIG_FILE_H

#include <file.h>

#include <map>
#include <string>

class config_file: public file
{
        public:
                config_file(const std::string& path);
                config_file();

                ~config_file();


                void set(const std::string& property, const std::string& value);
                std::string get(const std::string& property);

                // Get property, if it doesn't exist set it to default (def).
                std::string getdefault(const std::string& property, const std::string& def);

                int open(const std::string& path, file_flags_t flags = FILE_CREATE | FILE_READWRITE);
                int save();
        private:
                std::string path;

                std::map<std::string, std::map<std::string, std::string> > sections;
};

#endif /* src/config_file.h */
