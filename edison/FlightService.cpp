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

#include "FlightService.h"

static org::hummingdroid::flightapp::FlightService app;
int main() {
    app.loop();
    return 0;
}

namespace org {
namespace hummingdroid {
namespace flightapp {

FlightService::FlightService() :
    receiver(this),
    sensors(this),
    controller(this)
{
    // Set default settings
    {
        CommandPacket::ControllerConfig s;
        s.mutable_altitude_pid()->set_kp(0.);
        s.mutable_altitude_pid()->set_ki(0.);
        s.mutable_altitude_pid()->set_kd(0.);
        s.mutable_altitude_pid()->set_td(0.);
        s.mutable_altitude_pid()->set_ko(0.);
        s.mutable_roll_pid()->set_kp(0.);
        s.mutable_roll_pid()->set_ki(0.);
        s.mutable_roll_pid()->set_kd(0.);
        s.mutable_roll_pid()->set_td(0.);
        s.mutable_roll_pid()->set_ko(0.);
        s.mutable_pitch_pid()->set_kp(0.);
        s.mutable_pitch_pid()->set_ki(0.);
        s.mutable_pitch_pid()->set_kd(0.);
        s.mutable_pitch_pid()->set_td(0.);
        s.mutable_pitch_pid()->set_ko(0.);
        s.mutable_yaw_rate_pid()->set_kp(0.);
        s.mutable_yaw_rate_pid()->set_ki(0.);
        s.mutable_yaw_rate_pid()->set_kd(0.);
        s.mutable_yaw_rate_pid()->set_td(0.);
        s.mutable_yaw_rate_pid()->set_ko(0.);
        s.set_max_inclinaison(.2);
        s.set_max_altitude(.2);
        s.set_max_yaw_rate(.2);
        controller.setConfig(s);
    }

    {
        CommandPacket::SensorsConfig s;
        s.set_accel_lowpass_constant(.95);
        sensors.setConfig(s);
    }
}

void FlightService::loop()
{
    // Start the threads
    motors.begin();
    telemetry.start();
    receiver.start();
    sensors.run(); // Note: we directly run the sensor thread in the main thread
}

}
}
}
