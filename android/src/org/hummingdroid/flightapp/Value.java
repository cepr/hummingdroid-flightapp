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

public class Value {

	protected float value;
	protected long timestamp;

	public void set(Value value) {
		this.value = value.value;
		this.timestamp = value.timestamp;
	}

	public void set(float value, long timestamp) {
		this.value = value;
		this.timestamp = timestamp;
	}

	public void add(Value a, Value b) {
		value = a.value + b.value;
		timestamp = a.timestamp;
	}

	public void constrain(float min, float max) {
		if (value < min) {
			value = min;
		} else if (value > max) {
			value = max;
		}
	}

	public void amplify(Value value, float gain) {
		this.value = value.value * gain;
		this.timestamp = value.timestamp;
	}

	public void amplify(float gain) {
		this.value *= gain;
	}

	public void integrate(Value value) {
		if (timestamp > 0f) {
			float dt = (value.timestamp - timestamp) * 1e-9f;
			this.value += value.value * dt;
		}
		timestamp = value.timestamp;
	}

	public void reset() {
		value = 0f;
	}

	public static class Derivator extends Value {

		private float prev;

		public void derive(Value value) {
			if (timestamp != 0) {
				float dt = (value.timestamp - timestamp) * 1e-9f;
				if (dt > 0f) {
					this.value = (value.value - prev) / dt;
				}
			}
			prev = value.value;
			this.timestamp = value.timestamp;
		}

	}

	public static class LowPass extends Value {	
		protected float T;

		public void setT(float T) {
			this.T = T;
		}

		public void lowpass(Value value) {
			this.value = T * this.value + (1f - T) * value.value;
			this.timestamp = value.timestamp;
		}
	}
	
	public static class HighPass extends Value {

		protected final LowPass lowpass = new LowPass();

		public void setT(float T) {
			lowpass.setT(T);
		}
		
		public void highpass(Value value) {
			lowpass.lowpass(value);
			this.value = value.value - lowpass.value;
			this.timestamp = value.timestamp;
		}
	}

	public static class PID extends Value {

		private final Value propo = new Value();
		private final Value integ = new Value();
		private final Value integ_factor = new Value(); 
		private final LowPass low_pass = new LowPass();
		private final Derivator deriv = new Derivator();
		private final Value deriv_factor = new Value();
		private org.hummingdroid.Communication.PID params = null;

		public synchronized void setParams(org.hummingdroid.Communication.PID params) {
			this.params = params;
			low_pass.setT(params.getTd());
		}

		public synchronized void pid(Value value) {
			if (params == null) {
				return;
			}
			// P
			propo.amplify(value, params.getKp());
			// I
			integ.integrate(value);
			float max = 1f / params.getKi();
			integ.constrain(-max, max);
			integ_factor.amplify(integ, params.getKi());
			// D
			low_pass.lowpass(value);
			deriv.derive(low_pass);
			deriv_factor.amplify(deriv, params.getKd());
			// Sum
			this.value = propo.value + integ_factor.value + deriv_factor.value + params.getKo();
			this.timestamp = value.timestamp;
		}

		public synchronized void reset() {
			integ.reset();
		}
	}
}
