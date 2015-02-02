/*
 * HummingDroid, Android QuadCopter Controller
 * Copyright (C) 2013 Cedric Priscal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "Thread.h"
#include "Controller.h"
#include "Sensors.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

class FlightService;

/**
 * Remote commands receiver
 * 
 * <p>
 * This class listens to incoming commands from UDP port 49152.
 * </p>
 */
class Receiver : public Thread {

public:
    /**
	 * UDP port for remote commands.
	 */
    static const int REMOTE_COMMAND_UDP_PORT = 49152;

	/**
	 * Delay without receiving remote command after which we consider the link
	 * disconnected.
	 */
    static const int REMOTE_COMMAND_TIMEOUT_MS = 200;

    Controller *controller;
    Telemetry *telemetry;
    Sensors *sensors;

    boolean connected;

	/**
	 * Constructor.
	 */
    Receiver(FlightService* context);

    // Thread entry point
    void run();
};

}
}
}

#endif
