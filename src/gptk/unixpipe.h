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

#ifndef _SRC_GPTK_PIPE_H
#define _SRC_GPTK_PIPE_H

#include <ufile.h>

class unixpipe;
class unixpipe_endpoint: public ufile
{
        friend class unixpipe;

        private:
                // Prevent copying
                unixpipe_endpoint(const unixpipe_endpoint& other) {*this = other;};
                unixpipe_endpoint& operator=(const unixpipe_endpoint& other) {return *this;};

                virtual ~unixpipe_endpoint();

        protected:
                unixpipe_endpoint(unixpipe* pipe, int fd);
        private:
                unixpipe* pipe;

                virtual void data_available();
};

class unixpipe
{
        friend class unixpipe_endpoint;

        public:
                unixpipe();
                ~unixpipe();

                // Beware, these pointers do not exists longer than the lifetime of
                //      the complete pipe
                inline unixpipe_endpoint* first() {return m_first;};
                inline unixpipe_endpoint* second() {return m_second;};

        private:
                // Prevent copying
                unixpipe(const unixpipe& other) {*this = other;};
                unixpipe& operator=(const unixpipe& other) {return *this;};

        protected:
                virtual void data_available(unixpipe_endpoint* ep) = 0;
        private:
                unixpipe_endpoint* m_first;
                unixpipe_endpoint* m_second;
};

#endif /* src/gptk/pipe.h */
