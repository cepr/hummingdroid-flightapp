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

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

#include "Communication.pb.h"
#include "DatagramSocket.h"
#include "Thread.h"
#include "Object.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

class Telemetry : public Thread, public Object {

public:
    void setConfig(const CommandPacket::TelemetryConfig & config);
    void setCommand(const Attitude & command);
    void setAttitude(const Attitude & attitude);
    void setControl(const MotorsControl & control);
    // Thread entry point, do not call directly
    void run();
private:
    DatagramSocket socket;
    CommandPacket::TelemetryConfig config;
    TelemetryPacket packet;
};

}
}
}

#endif
