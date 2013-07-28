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

import org.hummingdroid.Communication.MotorsControl;

/**
 * This class commands the ESC for the 4 motors.
 * 
 * <p>
 * This class expects an QuadCopter in X configuration in the following motors
 * order: front-left, front-right, back-right, back-left.
 * </p>
 */
public class Motors {

	private final byte[] data = new byte[4];

	private final Teensy teensy;

	public Motors(FlightService context) {
		this.teensy = context.getTeensy();
	}

	private byte cast(float value) {
		if (value < 0f) {
			return 0;
		} else if (value > 255f) {
			return (byte) 255;
		} else {
			return (byte) value;
		}
	}

	public void setControl(MotorsControl control) {
		float g = control.getAltitudeThrottle();
		float r = control.getRollThrottle();
		float p = control.getPitchThrottle();
		float y = control.getYawThrottle();

		float fl = (1 + r) * (1 + p) * (1 - y);
		float fr = (1 - r) * (1 + p) * (1 + y);
		float br = (1 - r) * (1 - p) * (1 - y);
		float bl = (1 + r) * (1 - p) * (1 + y);
		float max = Math.max(Math.max(fl, fr), Math.max(br, bl));
		g = Math.max(g, 1f / max);

		data[0] = cast(fl * g * 255f);
		data[1] = cast(fr * g * 255f);
		data[2] = cast(br * g * 255f);
		data[3] = cast(bl * g * 255f);

		teensy.write(data, 50);
	}
}
