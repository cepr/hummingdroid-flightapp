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

#include "Motors.h"
#include <AnalogIO.h>

namespace org {
namespace hummingdroid {
namespace flightapp {

Motors::Motors() : min_pwm(0), max_pwm(0)
{
}

void Motors::begin()
{
    // TODO set PWM frequency (defaults to 483Hz)
}

void Motors::setConfig(const CommandPacket::MotorsConfig &config)
{
    min_pwm = config.min_pwm();
    max_pwm = config.max_pwm();
}

void Motors::setControl(const MotorsControl &control)
{
    float g = control.altitude_throttle();
    float r = control.roll_throttle();
    float p = control.pitch_throttle();
    float y = control.yaw_throttle();

    float fl = (1 + r) * (1 + p) * (1 - y);
    float fr = (1 - r) * (1 + p) * (1 + y);
    float br = (1 - r) * (1 - p) * (1 - y);
    float bl = (1 + r) * (1 - p) * (1 + y);

    analogWrite(0, scale(fl + g));
    analogWrite(1, scale(fr + g));
    analogWrite(2, scale(br + g));
    analogWrite(3, scale(bl + g));
}

}
}
}
