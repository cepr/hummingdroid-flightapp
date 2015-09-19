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
#include "mraa.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

#define PWM_FL_PIN 0
#define PWM_FR_PIN 14
#define PWM_BR_PIN 20
#define PWM_BL_PIN 21

Motors::Motors() : min_pwm(0), max_pwm(0)
{
    this->pwm_fl = mraa_pwm_init(PWM_FL_PIN);
    this->pwm_fr = mraa_pwm_init(PWM_FR_PIN);
    this->pwm_br = mraa_pwm_init(PWM_BR_PIN);
    this->pwm_bl = mraa_pwm_init(PWM_BL_PIN);

    if (!this->pwm_fl ||
            !this->pwm_fr ||
            !this->pwm_br ||
            !this->pwm_bl) {
        printf("PWM not available.\n");
        exit(1);
    }

    // Set the frequency to 450Hz
    mraa_pwm_period_us(this->pwm_fl, 1000000/450);
    mraa_pwm_period_us(this->pwm_fr, 1000000/450);
    mraa_pwm_period_us(this->pwm_br, 1000000/450);
    mraa_pwm_period_us(this->pwm_bl, 1000000/450);
}

void Motors::begin()
{
    mraa_pwm_enable(this->pwm_fl, 1);
    mraa_pwm_enable(this->pwm_fr, 1);
    mraa_pwm_enable(this->pwm_br, 1);
    mraa_pwm_enable(this->pwm_bl, 1);
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

    float fl =  r + p - y + g;
    float fr = -r + p + y + g;
    float br = -r - p - y + g;
    float bl =  r - p + y + g;

    mraa_pwm_write(this->pwm_fl, scale(fl));
    mraa_pwm_write(this->pwm_fr, scale(fr));
    mraa_pwm_write(this->pwm_br, scale(br));
    mraa_pwm_write(this->pwm_bl, scale(bl));
}

}
}
}
