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

import java.io.IOException;
import java.nio.ByteBuffer;

import org.hummingdroid.Communication.Attitude;
import org.hummingdroid.Communication.CommandPacket.SensorsConfig;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

/**
 * Attitude determination using the phone gyroscope and accelerometer.
 * 
 * <p>
 * This class listens for gyroscope and accelerometer values and compute the
 * current attitude of the QuadCopter.
 * </p>
 */
public class Sensors {

	private final Teensy teensy;
	private final Controller controller;
	private final Telemetry telemetry;

	private final SensorManager sensor_manager;
	private Thread thread;

	// Roll
	private final Value roll_gyro = new Value();
	private final Value roll_accel = new Value();
	private final Value roll_gyro_rate = new Value();
	private final Value.LowPass roll_accel_lowpass = new Value.LowPass();
	private final Value.HighPass roll_gyro_highpass = new Value.HighPass();
	private final Value roll = new Value();

	// Pitch
	private final Value pitch_gyro = new Value();
	private final Value pitch_accel = new Value();
	private final Value pitch_gyro_rate = new Value();
	private final Value.LowPass pitch_accel_lowpass = new Value.LowPass();
	private final Value.HighPass pitch_gyro_highpass = new Value.HighPass();
	private final Value pitch = new Value();

	// Yaw rate
	private final Value yaw_rate = new Value();

	// Altitude
	private final Value altitude = new Value();

	private final Attitude.Builder builder = Attitude.newBuilder();

	/**
	 * Constructor.
	 * 
	 * @param context
	 *            FlightService instance.
	 */
	public Sensors(FlightService context) {
		this.teensy = context.getTeensy();
		this.controller = context.getController();
		this.telemetry = context.getTelemetry();
		sensor_manager = (SensorManager) context
				.getSystemService(Context.SENSOR_SERVICE);
	}

	public synchronized void setConfig(SensorsConfig config) {
		float T = config.getAccelLowpassConstant();
		roll_accel_lowpass.setT(T);
		roll_gyro_highpass.setT(T);
		pitch_accel_lowpass.setT(T);
		pitch_gyro_highpass.setT(T);
	}

	public synchronized void start() {
		// Listen to gyroscope
		sensor_manager.registerListener(gyro_listener,
				sensor_manager.getDefaultSensor(Sensor.TYPE_GYROSCOPE),
				SensorManager.SENSOR_DELAY_FASTEST);

		// Listen to accelerometer
		sensor_manager.registerListener(accel_listener,
				sensor_manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
				SensorManager.SENSOR_DELAY_FASTEST);

		// Start the altitude reading thread
		if (thread == null) {
			thread = new AltitudeReadingThread();
			thread.start();
		}
	}

	public synchronized void stop() throws InterruptedException {
		// Stop sensors
		sensor_manager.unregisterListener(accel_listener);
		sensor_manager.unregisterListener(gyro_listener);

		// Stop the altitude reading thread
		while (thread != null) {
			thread.interrupt();
			wait(100);
		}
	}

	/**
	 * Gyroscope listener.
	 */
	private final SensorEventListener gyro_listener = new SensorEventListener() {
		@Override
		public void onSensorChanged(SensorEvent event) {
			synchronized (this) {
				roll_gyro_rate.set(event.values[1], event.timestamp);
				roll_gyro.integrate(roll_gyro_rate);

				pitch_gyro_rate.set(event.values[0], event.timestamp);
				pitch_gyro.integrate(pitch_gyro_rate);

				// Apply the low-pass filter on the accelerometer and the
				// high-pass filter on the gyroscope
				roll_accel_lowpass.lowpass(roll_accel);
				roll_gyro_highpass.highpass(roll_gyro);
				roll.add(roll_gyro_highpass, roll_accel_lowpass);

				pitch_accel_lowpass.lowpass(pitch_accel);
				pitch_gyro_highpass.highpass(pitch_gyro);
				pitch.add(pitch_gyro_highpass, pitch_accel_lowpass);

				yaw_rate.set(-event.values[2], event.timestamp);

				Attitude attitude = builder.clear().setAltitude(altitude.value)
						.setRoll(roll.value).setPitch(pitch.value)
						.setYawRate(yaw_rate.value).build();

				controller.setAttitude(attitude, event.timestamp);

				telemetry.setAttitude(attitude);
			}
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
		}
	};

	/**
	 * Accelerometer listener.
	 */
	private final SensorEventListener accel_listener = new SensorEventListener() {
		@Override
		public void onSensorChanged(SensorEvent event) {
			synchronized (this) {
				roll_accel.set(
						(float) Math.atan2(-event.values[0], event.values[2]),
						event.timestamp);

				pitch_accel.set(
						(float) -Math.atan2(-event.values[1], event.values[2]),
						event.timestamp);
			}
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
		}
	};

	private class AltitudeReadingThread extends Thread {

		public AltitudeReadingThread() {
			super("AltitudeReading");
		}

		/**
		 * Altitude reading thread entry point.
		 */
		@Override
		public void run() {
			ByteBuffer buffer = ByteBuffer.allocate(4);
			try {
				while (true) {
					try {
						teensy.read(buffer.array(), 200);
						synchronized (this) {
							altitude.set(buffer.getInt(0), System.nanoTime());
						}
					} catch (IOException e) {
						// To avoid high CPU load
						Thread.sleep(200);
					}
				}
			} catch (InterruptedException e) {
				// Leave the thread
			} finally {
				synchronized(Sensors.this) {
					thread = null;
					Sensors.this.notify();
				}
			}
		}
	}
}
