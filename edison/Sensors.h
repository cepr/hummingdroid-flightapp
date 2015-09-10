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
#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "Thread.h"
#include "Object.h"
#include "Value.h"
#include "Communication.pb.h"
#include <SFE_LSM9DS0.h>

namespace org {
namespace hummingdroid {
namespace flightapp {

class Controller;
class Telemetry;
class FlightService;

/**
 * Attitude determination using the phone gyroscope and accelerometer.
 * 
 * <p>
 * This class listens for gyroscope and accelerometer values and compute the
 * current attitude of the QuadCopter.
 * </p>
 */
class Sensors : public Thread, public Object {

private:
    Controller* controller;
    Telemetry* telemetry;

    // LSM3DS0 sensor
    LSM9DS0 dof;

	// Roll
    Integrator roll_gyro;
    Value roll_accel;
    Value roll_gyro_rate;
    LowPass roll_accel_lowpass;
    HighPass roll_gyro_highpass;
    Value roll;
    float gyro_roll_bias;
    float gyro_roll_gain;
    float accel_roll_bias;

	// Pitch
    Integrator pitch_gyro;
    Value pitch_accel;
    Value pitch_gyro_rate;
    LowPass pitch_accel_lowpass;
    HighPass pitch_gyro_highpass;
    Value pitch;
    float gyro_pitch_bias;
    float gyro_pitch_gain;
    float accel_pitch_bias;

	// Yaw rate
    Value yaw_rate;
    float gyro_yaw_bias;

	// Altitude
    Value altitude;

    // Computed attitude
    Attitude attitude;

    // Switches
    Switches switches;

    // Misc
    bool apply_modulo;

public:
	/**
	 * Constructor.
	 * 
	 * @param context
	 *            FlightService instance.
	 */
    Sensors(FlightService *context);

    void setConfig(const CommandPacket::SensorsConfig & config);

    void run();
};

}
}
}

#endif
