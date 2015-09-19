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

#include "Sensors.h"
#include "FlightService.h"
#include <math.h>

// SDO_XM and SDO_G are both grounded, so our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW

#define FRONT_LEFT_SWITCH_PIN 0 // TODO
#define FRONT_RIGHT_SWITCH_PIN 0 // TODO
#define BACK_RIGHT_SWITCH_PIN 0 // TODO
#define BACK_LEFT_SWITCH_PIN 0 // TODO

#define DEG_TO_RAD 0.017453292519943295769236907684886

namespace org {
namespace hummingdroid {
namespace flightapp {

Sensors::Sensors(FlightService *context) :
    controller(&context->controller),
    telemetry(&context->telemetry),
    dof(LSM9DS0_G, LSM9DS0_XM),
    gyro_roll_bias(0.),
    gyro_roll_gain(1.),
    accel_roll_bias(0.),
    gyro_pitch_bias(0.),
    gyro_pitch_gain(1.),
    accel_pitch_bias(0.),
    gyro_yaw_bias(0.),
    apply_modulo(false)
{
    //pinMode(FRONT_LEFT_SWITCH_PIN, INPUT_PULLUP);
    //pinMode(FRONT_RIGHT_SWITCH_PIN, INPUT_PULLUP);
    //pinMode(BACK_RIGHT_SWITCH_PIN, INPUT_PULLUP);
    //pinMode(BACK_LEFT_SWITCH_PIN, INPUT_PULLUP);
}

void Sensors::setConfig(const CommandPacket::SensorsConfig &config)
{
    synchronized
    float T = config.accel_lowpass_constant();
    roll_accel_lowpass.setT(T);
    roll_gyro_highpass.setT(T);
    pitch_accel_lowpass.setT(T);
    pitch_gyro_highpass.setT(T);
    gyro_roll_bias = config.gyro_roll_bias();
    gyro_roll_gain = config.gyro_roll_gain();
    accel_roll_bias = config.accel_roll_bias();
    gyro_pitch_bias = config.gyro_pitch_bias();
    gyro_pitch_gain = config.gyro_pitch_gain();
    accel_pitch_bias = config.accel_pitch_bias();
    gyro_yaw_bias = config.gyro_yaw_bias();
    apply_modulo = config.apply_modulo();
}

void Sensors::run()
{
    fprintf(stderr, "Sensors: Thread started\n");
    dof.begin();
    while(true) {
        synchronized

        // Read switches
        //switches.set_front_left(digitalRead(FRONT_LEFT_SWITCH_PIN));
        //switches.set_front_right(digitalRead(FRONT_RIGHT_SWITCH_PIN));
        //switches.set_back_right(digitalRead(BACK_RIGHT_SWITCH_PIN));
        //switches.set_back_left(digitalRead(BACK_LEFT_SWITCH_PIN));

        // Read Acceleration
        dof.readAccel();
        Timestamp now = Timestamp::now();
        roll_accel.set(atan2f(dof.calcAccel(dof.ay), dof.calcAccel(-dof.az)), now);
        roll_accel.value -= accel_roll_bias;
        pitch_accel.set(atan2f(dof.calcAccel(-dof.ax), dof.calcAccel(-dof.az)), now);
        pitch_accel.value -= accel_pitch_bias;

        // Read Gyroscope
        dof.readGyro();
        now = Timestamp::now();

        roll_gyro_rate.set(-dof.calcGyro(dof.gx) * DEG_TO_RAD, now);
        roll_gyro_rate.value = (roll_gyro_rate.value - gyro_roll_bias) * gyro_roll_gain;
        roll_gyro.integrate(roll_gyro_rate);

        pitch_gyro_rate.set(dof.calcGyro(-dof.gy) * DEG_TO_RAD, now);
        pitch_gyro_rate.value = (pitch_gyro_rate.value - gyro_pitch_bias) * gyro_pitch_gain;
        pitch_gyro.integrate(pitch_gyro_rate);

        // Apply the low-pass filter on the accelerometer and the
        // high-pass filter on the gyroscope
        roll_accel_lowpass.lowpass(roll_accel);
        roll_gyro_highpass.highpass(roll_gyro);
        roll.add(roll_gyro_highpass, roll_accel_lowpass);

        pitch_accel_lowpass.lowpass(pitch_accel);
        pitch_gyro_highpass.highpass(pitch_gyro);
        pitch.add(pitch_gyro_highpass, pitch_accel_lowpass);

        // Limit the angles between -PI and PI
        if (apply_modulo) {
            while (roll.value > M_PI) {
                roll.value -= M_PI*2;
            }
            while (roll.value < -M_PI) {
                roll.value += M_PI*2;
            }
            while (pitch.value > M_PI) {
                pitch.value -= M_PI*2;
            }
            while (pitch.value < -M_PI) {
                pitch.value += M_PI*2;
            }
        }

        yaw_rate.set(dof.calcGyro(dof.gz) * DEG_TO_RAD, now);
        yaw_rate.value -= gyro_yaw_bias;

        attitude.set_altitude(altitude.value);
        attitude.set_roll(roll.value);
        attitude.set_pitch(pitch.value);
        attitude.set_yaw_rate(yaw_rate.value);
        attitude.set_timestamp(now);

        controller->setAttitude(attitude, now);
        telemetry->setAttitude(attitude);
        telemetry->setSwitches(switches);

        usleep(2500); // about 400 Hz
    }
}

void Sensors::reset()
{
    roll_gyro.reset();
    pitch_gyro.reset();
}

}
}
}
