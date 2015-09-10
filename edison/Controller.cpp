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

#include "Controller.h"
#include "FlightService.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

Controller::Controller(FlightService *context) :
    motors(&context->motors),
    telemetry(&context->telemetry)
{
}

void Controller::setConfig(const CommandPacket::ControllerConfig &config)
{
    synchronized
    this->config = config;
    altitude_control.setParams(config.altitude_pid());
    roll_control.setParams(config.roll_pid());
    pitch_control.setParams(config.pitch_pid());
    yaw_rate_control.setParams(config.yaw_rate_pid());
}

void Controller::setCommand(const Attitude &command)
{
    synchronized
    this->command = command;
    if (command.has_altitude() && command.altitude() == 0.) {
        // Need to reset the integrator of the PID
        altitude_control.reset();
        roll_control.reset();
        pitch_control.reset();
        yaw_rate_control.reset();
    }
}

void Controller::setAttitude(const Attitude &attitude, const Timestamp &timestamp)
{
    synchronized
    // Check for instability
    bool excessive_roll, excessive_pitch, excessive_yaw_rate, excessive_altitude;
    if (config.has_max_inclinaison()) {
        excessive_roll = (attitude.roll() < -config.max_inclinaison()) || (attitude.roll() > config.max_inclinaison());
        excessive_pitch = (attitude.pitch() < -config.max_inclinaison()) || (attitude.pitch() > config.max_inclinaison());
    } else {
        excessive_roll = false;
        excessive_pitch = false;
    }

    if (config.has_max_yaw_rate()) {
        excessive_yaw_rate = (attitude.yaw_rate() < -config.max_yaw_rate()) || (attitude.yaw_rate() > config.max_yaw_rate());
    } else {
        excessive_yaw_rate = false;
    }

    if (config.max_altitude()) {
        excessive_altitude = (attitude.altitude() > config.max_altitude()) || (attitude.altitude() > config.max_altitude());
    } else {
        excessive_altitude = false;
    }

    if (excessive_roll || excessive_pitch || excessive_yaw_rate || excessive_altitude) {

        // emergency stop
        altitude_control.value = 0;
        roll_control.value = 0;
        pitch_control.value = 0;
        yaw_rate_control.value = 0;

    } else {

        // Compute the error
        altitude_error.set(command.altitude() - attitude.altitude(), timestamp);
        roll_error.set(command.roll() - attitude.roll(), timestamp);
        pitch_error.set(command.pitch() - attitude.pitch(), timestamp);
        yaw_rate_error.set(command.yaw_rate() - attitude.yaw_rate(), timestamp);

        // PID
        altitude_control.pid(altitude_error);
        roll_control.pid(roll_error);
        pitch_control.pid(pitch_error);
        yaw_rate_control.pid(yaw_rate_error);
    }

    MotorsControl control;
    control.set_altitude_throttle(altitude_control.value);
    control.set_roll_throttle(roll_control.value);
    control.set_pitch_throttle(pitch_control.value);
    control.set_yaw_throttle(yaw_rate_control.value);
    control.set_timestamp(timestamp);

    // Telemetry
    telemetry->setControl(control);

    // Drive the motors
    motors->setControl(control);
}

}
}
}
