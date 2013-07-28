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
package org.hummingdroid.flightapp;

import org.hummingdroid.Communication.Attitude;
import org.hummingdroid.Communication.MotorsControl;
import org.hummingdroid.Communication.CommandPacket.ControllerConfig;
import org.hummingdroid.flightapp.Value.PID;

/**
 * Flight controller.
 * 
 * <p>
 * This class implements the closed-loop control of the vehicle attitude.
 * </p>
 */
public class Controller {

	private final Motors motors;
	private final Telemetry telemetry;

	// Config
	private ControllerConfig config;

	// Command
	private Attitude command;

	// Altitude control
	private final Value altitude_error = new Value();
	private final Value.PID altitude_control = new PID();

	// Roll control
	private final Value roll_error = new Value();
	private final Value.PID roll_control = new PID();

	// Pitch control
	private final Value pitch_error = new Value();
	private final Value.PID pitch_control = new PID();

	// Yaw rate control
	private final Value yaw_rate_error = new Value();
	private final Value.PID yaw_rate_control = new PID();

	// Motors control
	private final MotorsControl.Builder output = MotorsControl.newBuilder();

	/**
	 * Constructor.
	 * 
	 * @param context
	 *            FlightService instance.
	 */
	public Controller(FlightService context) {
		this.motors = context.getMotors();
		this.telemetry = context.getTelemetry();
	}

	/**
	 * Sets the controller configuration.
	 * 
	 * @param config
	 *            New configuration.
	 */
	public synchronized void setConfig(ControllerConfig config) {
		this.config = config;
		altitude_control.setParams(config.getAltitudePid());
		roll_control.setParams(config.getRollPid());
		pitch_control.setParams(config.getPitchPid());
		yaw_rate_control.setParams(config.getYawRatePid());
	}

	/**
	 * Sets the new command.
	 * 
	 * <p>
	 * The command is the attitude we want to reach.
	 * </p>
	 * 
	 * @param command
	 *            The new command.
	 */
	public synchronized void setCommand(Attitude command) {
		this.command = command;
	}

	/**
	 * Sets the latest measured attitude.
	 * 
	 * @param attitude
	 *            The latest measured attitude.
	 * 
	 * @param timestamp
	 *            The time in nanosecond of this measure.
	 */
	public synchronized void setAttitude(Attitude attitude, long timestamp) {

		// Check for instability
		boolean excessive_roll, excessive_pitch, excessive_yaw_rate, excessive_altitude;
		if (config.hasMaxInclinaison()) {
			excessive_roll = (attitude.getRoll() < -config.getMaxInclinaison())
					|| (attitude.getRoll() > config.getMaxInclinaison());
			excessive_pitch = (attitude.getPitch() < -config
					.getMaxInclinaison())
					|| (attitude.getPitch() > config.getMaxInclinaison());
		} else {
			excessive_roll = false;
			excessive_pitch = false;
		}

		if (config.hasMaxYawRate()) {
			excessive_yaw_rate = (attitude.getYawRate() < -config
					.getMaxYawRate())
					|| (attitude.getYawRate() > config.getMaxYawRate());
		} else {
			excessive_yaw_rate = false;
		}

		if (config.hasMaxAltitude()) {
			excessive_altitude = (attitude.getAltitude() > config
					.getMaxAltitude())
					|| (attitude.getAltitude() > config.getMaxAltitude());
		} else {
			excessive_altitude = false;
		}

		if (excessive_roll || excessive_pitch || excessive_yaw_rate
				|| excessive_altitude) {

			// emergency stop
			altitude_control.value = 0;
			roll_control.value = 0;
			pitch_control.value = 0;
			yaw_rate_control.value = 0;

		} else {

			// Compute error
			altitude_error.set(command.getAltitude() - attitude.getAltitude(),
					timestamp);
			roll_error.set(command.getRoll() - attitude.getRoll(), timestamp);
			pitch_error
					.set(command.getPitch() - attitude.getPitch(), timestamp);
			yaw_rate_error.set(command.getYawRate() - attitude.getYawRate(),
					timestamp);

			// PID
			altitude_control.pid(altitude_error);
			roll_control.pid(roll_error);
			pitch_control.pid(pitch_error);
			yaw_rate_control.pid(yaw_rate_error);
		}

		MotorsControl control = output.clear()
				.setAltitudeThrottle(altitude_control.value)
				.setRollThrottle(roll_control.value)
				.setPitchThrottle(pitch_control.value)
				.setYawThrottle(yaw_rate_control.value).build();

		// Telemetry
		telemetry.setControl(control);

		// Drive the motors
		motors.setControl(control);
	}
}
