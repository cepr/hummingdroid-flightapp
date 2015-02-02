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

#ifndef _FLIGHTSERVICE_H_
#define _FLIGHTSERVICE_H_

#include "Receiver.h"
#include "Motors.h"
#include "Telemetry.h"
#include "Sensors.h"
#include "Controller.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

class FlightService {

public:
    Receiver receiver;
    Motors motors;
    Telemetry telemetry;
    Sensors sensors;
    Controller controller;

    // Constructor
    FlightService();

    // Loop
    void loop();
};

}
}
}

#endif
