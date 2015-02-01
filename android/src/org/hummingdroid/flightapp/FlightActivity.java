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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.Switch;

public class FlightActivity extends Activity implements
		OnCheckedChangeListener, Runnable, OnTouchListener {

	private Handler handler;
	private Switch switchService;
	private ImageView imageMotors;
	private ImageView imageRC;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_flight_server);
		switchService = (Switch) findViewById(R.id.switchService);
		switchService.setOnTouchListener(this);
		imageMotors = (ImageView) findViewById(R.id.imageMotors);
		imageRC = (ImageView) findViewById(R.id.imageRC);

		switchService.setOnCheckedChangeListener(this);
		handler = new Handler();
		run();
	}

	@Override
	public void onCheckedChanged(CompoundButton arg0, boolean arg1) {
		handler.removeCallbacks(this);
		Intent i = new Intent(this, FlightService.class);
		if (arg1) {
			startService(i);
			handler.postDelayed(this, 500);
		} else {
			switchService.setChecked(false);
			imageMotors.setImageDrawable(getResources().getDrawable(
					R.drawable.unknown));
			imageRC.setImageDrawable(getResources().getDrawable(
					R.drawable.unknown));
			stopService(i);
		}
	}

	@Override
	public void run() {
		FlightService service = FlightService.getInstance(); 
		if (service != null) {
			switchService.setChecked(true);
			imageMotors.setImageDrawable(getResources().getDrawable(
					service.getTeensy().isConnected() ? R.drawable.yes : R.drawable.no));
			imageRC.setImageDrawable(getResources().getDrawable(
					service.getReceiver().isConnected() ? R.drawable.yes : R.drawable.no));
			handler.postDelayed(this, 500);
		} else {
			switchService.setChecked(false);
			imageMotors.setImageDrawable(getResources().getDrawable(
					R.drawable.unknown));
			imageRC.setImageDrawable(getResources().getDrawable(
					R.drawable.unknown));
		}
	}

	@Override
	public boolean onTouch(View arg0, MotionEvent arg1) {
		handler.removeCallbacks(this);
		int action = arg1.getAction();
		if (action == MotionEvent.ACTION_UP) {
			// Restart refresh
			handler.postDelayed(this, 500);
		}
		return false;
	}
}
