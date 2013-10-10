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

import org.hummingdroid.Communication;
import org.hummingdroid.Communication.CommandPacket;
import org.hummingdroid.Communication.CommandPacket.ControllerConfig;
import org.hummingdroid.Communication.CommandPacket.SensorsConfig;
import org.hummingdroid.Communication.PID;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

public class FlightService extends Service {

	public static final String TAG = "Hummingdroid";

	private static FlightService instance;
	private Teensy teensy;
	private Receiver receiver;
	private Motors motors;
	private Telemetry telemetry;
	private Sensors sensors;
	private WakeLock wake;
	private Controller controller;

	/**
	 * Gets the FlightService instance.
	 * 
	 * @return The FlightService instance.
	 */
	static FlightService getInstance() {
		return instance;
	}

	/**
	 * Gets the Receiver instance.
	 * 
	 * @return The Receiver instance.
	 */
	public Receiver getReceiver() {
		return receiver;
	}

	/**
	 * Gets the Controller instance.
	 * 
	 * @return The Controller instance.
	 */
	public Controller getController() {
		return controller;
	}

	/**
	 * Gets the Telemetry instance.
	 * 
	 * @return The telemetry instance.
	 */
	public Telemetry getTelemetry() {
		return telemetry;
	}

	/**
	 * Gets the Teensy instance.
	 * 
	 * @return The Teensy instance.
	 */
	public Teensy getTeensy() {
		return teensy;
	}

	/**
	 * Gets the Motors instance.
	 * 
	 * @return The Motors instance.
	 */
	public Motors getMotors() {
		return motors;
	}

	/**
	 * Gets the Sensors instance.
	 * 
	 * @return The Sensors instance.
	 */
	public Sensors getSensors() {
		return sensors;
	}

	/**
	 * Service creating entry point.
	 */
	@Override
	public void onCreate() {
		super.onCreate();

		// Prevent the phone from going to sleep
		PowerManager pm = (PowerManager) getSystemService(POWER_SERVICE);
		wake = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "FlightService");
		wake.acquire();

		teensy = new Teensy(this);
		motors = new Motors(this);
		telemetry = new Telemetry();
		controller = new Controller(this);
		sensors = new Sensors(this);
		receiver = new Receiver(this);

		// Load default settings
		// @formatter:off
		Communication.CommandPacket default_settings =
				CommandPacket.newBuilder()
					.setControllerConfig(ControllerConfig.newBuilder()
							.setAltitudePid(PID.newBuilder()
									.setKp(0.0f)
									.setKi(0.0f)
									.setKd(0.0f)
									.setTd(0.0f)
									.setKo(0.0f))
							.setRollPid(PID.newBuilder()
									.setKp(0.0f)
									.setKi(0.0f)
									.setKd(0.0f)
									.setTd(0.0f)
									.setKo(0.0f))
							.setPitchPid(PID.newBuilder()
									.setKp(0.0f)
									.setKi(0.0f)
									.setKd(0.0f)
									.setTd(0.0f)
									.setKo(0.0f))
							.setYawRatePid(PID.newBuilder()
									.setKp(0.0f)
									.setKi(0.0f)
									.setKd(0.0f)
									.setTd(0.0f)
									.setKo(0.0f))
							.setMaxInclinaison(0.2f)
							.setMaxAltitude(0.2f)
							.setMaxYawRate(0.2f))
					.setSensorsConfig(SensorsConfig.newBuilder()
							.setAccelLowpassConstant(0.95f))
					.build();
		// @formatter:on
		controller.setConfig(default_settings.getControllerConfig());
		controller.setCommand(default_settings.getCommand());
		sensors.setConfig(default_settings.getSensorsConfig());

		// Start the threads
		telemetry.start();
		sensors.start();
		receiver.start();

		instance = this;
	}

	/**
	 * Service destruction entry point.
	 */
	@Override
	public void onDestroy() {

		instance = null;

		// Stop the telemetry
		try {
			telemetry.stop();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		// Stop the Attitude determination
		try {
			sensors.stop();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		// Close receiver
		try {
			receiver.stop();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		teensy.close();

		wake.release();

		super.onDestroy();
	}

	/**
	 * Service start command handler.
	 */
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		return START_STICKY;
	}

	/**
	 * Service binding entry point.
	 * 
	 * <p>
	 * The service cannot be bound.
	 * </p>
	 */
	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
}
