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

namespace org {
namespace hummingdroid {
namespace flightapp {

Sensors::Sensors(FlightService *context) :
    controller(&context->controller),
    telemetry(&context->telemetry),
    dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM)
{

}

void Sensors::setConfig(const CommandPacket::SensorsConfig &config)
{
    synchronized
    float T = config.accel_lowpass_constant();
    roll_accel_lowpass.setT(T);
    roll_gyro_highpass.setT(T);
    pitch_accel_lowpass.setT(T);
    pitch_gyro_highpass.setT(T);
}

void Sensors::run()
{
    fprintf(stderr, "Sensors: Thread started\n");
    dof.begin();
    while(true) {
        synchronized

        // Read Acceleration
        dof.readAccel();
        Timestamp now = Timestamp::now();
        roll_accel.set(atan2f(-dof.calcAccel(dof.ax), dof.calcAccel(dof.az)), now);
        pitch_accel.set(-atan2f(-dof.calcAccel(dof.ay), dof.calcAccel(dof.az)), now);

        // Read Gyroscope
        dof.readGyro();
        now = Timestamp::now();

        roll_gyro_rate.set(dof.calcGyro(dof.gy), now);
        roll_gyro.integrate(roll_gyro_rate);

        pitch_gyro_rate.set(dof.calcGyro(dof.gx), now);
        pitch_gyro.integrate(pitch_gyro_rate);

        // Apply the low-pass filter on the accelerometer and the
        // high-pass filter on the gyroscope
        roll_accel_lowpass.lowpass(roll_accel);
        roll_gyro_highpass.highpass(roll_gyro);
        roll.add(roll_gyro_highpass, roll_accel_lowpass);

        pitch_accel_lowpass.lowpass(pitch_accel);
        pitch_gyro_highpass.highpass(pitch_gyro);
        pitch.add(pitch_gyro_highpass, pitch_accel_lowpass);

        yaw_rate.set(-dof.calcGyro(dof.gz), now);

        attitude.set_altitude(altitude.value);
        attitude.set_roll(roll.value);
        attitude.set_pitch(pitch.value);
        attitude.set_yaw_rate(yaw_rate.value);

        controller->setAttitude(attitude, now);
        telemetry->setAttitude(attitude);

        usleep(2500); // about 400 Hz
    }
}

}
}
}
