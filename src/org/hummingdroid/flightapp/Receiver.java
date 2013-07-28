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
import java.io.InterruptedIOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import org.hummingdroid.Communication.CommandPacket;

/**
 * Remote commands receiver
 * 
 * <p>
 * This class listens to incoming commands from UDP port 49152.
 * </p>
 */
public class Receiver {

	/**
	 * UDP port for remote commands.
	 */
	private static final int REMOTE_COMMAND_UDP_PORT = 49152;

	/**
	 * Delay without receiving remote command after which we consider the link
	 * disconnected.
	 */
	private static final int REMOTE_COMMAND_TIMEOUT_MS = 100;

	private Thread thread = null;

	private final Controller controller;
	private final Telemetry telemetry;
	private final Sensors sensors;

	/**
	 * Constructor.
	 * 
	 * @param controller
	 * @param telemetry
	 * @param attitude
	 */
	public Receiver(FlightService context) {
		this.controller = context.getController();
		this.telemetry = context.getTelemetry();
		this.sensors = context.getSensors();
	}

	/**
	 * Flag indicating if the remote connection is active or not.
	 */
	private boolean connected;

	public boolean isConnected() {
		return connected;
	}

	public synchronized void start() {
		if (thread == null) {
			thread = new ReceiverThread();
			thread.start();
		}
	}

	public synchronized void stop() throws InterruptedException {
		while (thread != null) {
			thread.interrupt();
			wait(100);
		}
	}

	/**
	 * Remote command listening thread.
	 */
	private class ReceiverThread extends Thread {

		/**
		 * Constructs a new remote receiver reading thread.
		 */
		public ReceiverThread() {
			super("ReceiverThread");
		}

		@Override
		public void run() {
			final DatagramPacket packet = new DatagramPacket(new byte[1024],
					1024);
			try {
				// Main connection loop
				while (true) {
					// Create the socket
					DatagramSocket server = null;
					try {
						// Listen to datagram packets
						server = new DatagramSocket(REMOTE_COMMAND_UDP_PORT);
						server.setSoTimeout(REMOTE_COMMAND_TIMEOUT_MS);

						// Main reading loop
						while (true) {
							try {
								server.receive(packet);
								connected = true;
								CommandPacket command = CommandPacket
										.parseFrom(packet.getData());
								if (command.hasCommand()) {
									controller.setCommand(command.getCommand());
									telemetry.setCommand(command.getCommand());
								}
								if (command.hasControllerConfig()) {
									controller.setConfig(command
											.getControllerConfig());
								}
								if (command.hasTelemetryConfig()) {
									telemetry.setConfig(command
											.getTelemetryConfig());
								}
								if (command.hasSensorsConfig()) {
									sensors.setConfig(command
											.getSensorsConfig());
								}
							} catch (InterruptedIOException e) {
								// Nothing received for a long time
								connected = false;
							}
						}
					} catch (SocketException e) {
					} catch (IOException e) {
					} finally {
						// Close server
						if (server != null) {
							server.close();
						}
					}

					// Retry later
					Thread.sleep(1000);
				}
			} catch (InterruptedException e) {
				// Terminate thread
			} finally {
				thread = null;
				Receiver.this.notify();
			}
		}
	}
}
