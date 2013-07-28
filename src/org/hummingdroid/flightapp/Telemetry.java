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
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import org.hummingdroid.Communication.Attitude;
import org.hummingdroid.Communication.MotorsControl;
import org.hummingdroid.Communication.TelemetryPacket;
import org.hummingdroid.Communication.CommandPacket.TelemetryConfig;


// socat UDP-RECV:49152 STDOUT | trend -ff -c3a -L Alt,Roll,Pitch,Yaw,Control_Alt,Control_Roll,Control_Pitch,Control_Yaw -s -G1.57+2 -v - 1000
public class Telemetry {

	private Thread thread = null;
	private TelemetryConfig config = null;
	private TelemetryPacket.Builder telemetryPacketBuilder = TelemetryPacket
			.newBuilder();
	private final DatagramPacket packet = new DatagramPacket(new byte[0], 0);

	public synchronized void setConfig(TelemetryConfig config) {
		this.config = config;
		try {
			packet.setAddress(InetAddress.getByName(config.getHost()));
			packet.setPort(config.getPort());
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		notify();
	}

	public synchronized void setCommand(Attitude command) {
		if ((config != null) && (config.getCommandEnabled())) {
			telemetryPacketBuilder.setCommand(command);
		}
	}

	public synchronized void setAttitude(Attitude attitude) {
		if ((config != null) && (config.getAttitudeEnabled())) {
			telemetryPacketBuilder.setAttitude(attitude);
		}
	}

	public synchronized void setControl(MotorsControl control) {
		if ((config != null) && (config.getControlEnabled())) {
			telemetryPacketBuilder.setControl(control);
		}
	}

	public synchronized void start() {
		if (thread == null) {
			thread = new TelemetryThread();
			thread.start();
		}
	}

	public synchronized void stop() throws InterruptedException {
		while (thread != null) {
			thread.interrupt();
			wait(100);
		}
	}

	private class TelemetryThread extends Thread {
		public TelemetryThread() {
			super("Telemetry");
		}

		public void run() {
			DatagramSocket client = null;
			try {
				// Wait for the telemetry to be configured
				synchronized (this) {
					while (config == null) {
						wait();
					}
				}

				// Main connection loop
				while (true) {
					// Create the socket and the packet
					try {
						client = new DatagramSocket();
					} catch (SocketException e) {
						// Retry later
						Thread.sleep(1000);
						continue;
					}

					// Sending loop
					while (true) {
						synchronized (this) {
							packet.setData(telemetryPacketBuilder.build()
									.toByteArray());
							telemetryPacketBuilder.clear();
						}
						try {
							client.send(packet);
						} catch (IOException e) {
							e.printStackTrace();
						}
						Thread.sleep(50);
					}
				}
			} catch (InterruptedException e) {
				// Leave the thread
			} finally {
				if (client != null) {
					client.close();
				}
				thread = null;
				Telemetry.this.notify();
			}
		}
	}
}
