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

#ifndef _SRC_EVENTLOOP_H
#define _SRC_EVENTLOOP_H

#include "ufile.h"

#include "unixpipe.h"

#include <list>
#include <memory>

#define EVL \
        (const_cast<event_loop*>(event_loop::instance))



class event_loop
{
	public:
		event_loop();
	
		static event_loop*                      instance;

		int 				        run();

		int                                     flush();
		void 				        register_file(ufile* f);
		void                                    unregister_file(ufile* f);

		enum {
		        EV_FLUSH,
		};
		struct event
		{
		        int type;
		};

	private:
		std::list<ufile*>    files;

                class evl_event_queue: public unixpipe
                {
                        public:
                                void post_event(event& ev);

                        private:
                                void process_event(event& ev);
                                void data_available(unixpipe_endpoint* ep);
                };
                std::unique_ptr<evl_event_queue> event_queue;

};

#endif /* src/eventloop.h */
