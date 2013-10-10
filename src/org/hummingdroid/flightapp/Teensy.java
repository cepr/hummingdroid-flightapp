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

import android.content.Context;
import android.hardware.usb.UsbManager;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;

public class Teensy {

	private final Context context;
	private UsbSerialDriver driver;
	private boolean connected;

	public boolean isConnected() {
		return connected;
	}

	public Teensy(Context context) {
		this.context = context;
	}

	/**
	 * Writes as many bytes as possible from the source buffer.
	 * 
	 * @param src
	 *            the source byte buffer
	 * 
	 * @param timeoutMillis
	 *            the timeout for writing
	 */
	public void write(byte[] buffer, int timeout) {
		try {
			// Open the USB driver if necessary
			open();
			// Write
			try {
				driver.write(buffer, timeout);
				connected = true;
			} catch (IOException e) {
				try {
					driver.close();
				} catch (IOException e1) {
				}
				driver = null;
				connected = false;
			}
		} catch (IOException e) {
		}
	}

	/**
	 * Reads as many bytes as possible into the destination buffer.
	 * 
	 * @param dest
	 *            the destination byte buffer
	 * 
	 * @param timeoutMillis
	 *            the timeout for reading
	 * 
	 * @return the actual number of bytes read
	 * 
	 * @throws IOException
	 *             if an error occurred during reading
	 */
	public void read(byte[] dest, int timeoutMillis) throws IOException {
		// Open the USB driver if necessary
		open();

		// Read
		try {
			driver.read(dest, timeoutMillis);
		} catch (IOException e) {
			try {
				driver.close();
			} catch (IOException e1) {
			}
			driver = null;
			throw e;
		}
	}

	private void open() throws IOException {
		// Open the USB driver if necessary
		if (driver == null) {
			UsbManager usb_manager = (UsbManager) context
					.getSystemService(Context.USB_SERVICE);
			driver = UsbSerialProber.findFirstDevice(usb_manager);
			if (driver == null) {
				throw new IOException("Teensy not connected");
			}
			try {
				driver.open();
			} catch (IOException e) {
				driver = null;
				throw e;
			}
		}
	}

	public void close() {
		// Close USB
		if (driver != null) {
			try {
				connected = false;
				driver.close();
			} catch (IOException e) {
			}
		}
	}
}
