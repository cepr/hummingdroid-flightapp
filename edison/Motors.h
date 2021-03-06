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

#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "Communication.pb.h"
#include "mraa.hpp"

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

namespace org {
namespace hummingdroid {
namespace flightapp {

/**
 * This class commands the ESC for the 4 motors.
 * 
 * <p>
 * This class expects an QuadCopter in X configuration in the following motors
 * order: front-left, front-right, back-right, back-left.
 * </p>
 */
class Motors {
public:
    Motors();
    void begin();
    void setConfig(const CommandPacket::MotorsConfig & config);
    void setControl(const MotorsControl & control);
private:
    float min_pwm, max_pwm;
    mraa_pwm_context pwm_fl;
    mraa_pwm_context pwm_fr;
    mraa_pwm_context pwm_br;
    mraa_pwm_context pwm_bl;
    inline float scale(float value) {
        return MAX(MIN((max_pwm - min_pwm) * value + min_pwm, max_pwm), 0);
    }
};

}
}
}

#endif
