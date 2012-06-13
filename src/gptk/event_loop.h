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

#ifndef _SRC_GPTK_EVENTLOOP_H
#define _SRC_GPTK_EVENTLOOP_H

#include <ufile.h>
#include <unixpipe.h>

#include <list>

#define EVL \
        (const_cast<event_loop*>(event_loop::instance))



class event_loop
{
	public:
		event_loop();
		~event_loop();
	
		static event_loop*                      instance;

		int 				        run();
		void                                    stop();

		int                                     flush();
		void 				        register_file(ufile* f);
		void                                    unregister_file(ufile* f);

		enum {
		        EV_FLUSH,
		        EV_STOP,
		        EV_DEFER_DELETE,
		};
		struct event
		{
		        int type;
		        ufile* sender;
		};

		void post_event(event& ev)
		{
		        if(event_queue)
		                event_queue->post_event(ev);
		}
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
                evl_event_queue* event_queue;

                bool gracious_stop;
};

#endif /* src/gptk/eventloop.h */
