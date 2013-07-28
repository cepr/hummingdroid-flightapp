Humming Droid Flight App
========================

Introduction
------------

This repository contains the source code of the Flight Application for the HummingDroid QuadCopter, a QuadCopter using a Nexus 4 phone as on-board controller.

The Nexus 4 contains a gyroscope, a magnetometer, an accelerometer, a barometer, a GPS and several full-duplex radios (Bluetooth, WiFi, UMTS), which makes it a good candidate to control an UAV.

Data Flows
----------

There are three different data flows running through the system:

* The configuration data flow
* The command and control data flow
* The telemetry data flow

### The configuration data flow ###

The vehicle can be configured before or during flight from the GCS. The configuration settings are:

* The PID gains of the Controller
* The telemetry server IP address
* The telemetry data to report

![Configuration Data Flow](https://raw.github.com/cepr/hummingdroid-flightapp/master/doc/data_flow_config.dot.png)

### The command and control data flow ###

This is the data flow which controls the vehicule attitude. It takes GCS commands as input and controls the attitude of the vehicule using closed-loop PID controllers.

![Command and Control Data flow](https://raw.github.com/cepr/hummingdroid-flightapp/master/doc/data_flow_control.dot.png)

### The telemetry data flow ###

The Telemetry class gathers all the data going through the vehicle and send this data to the GCS for monitoring purpose.

![Telemetry Data Flow](https://raw.github.com/cepr/hummingdroid-flightapp/master/doc/data_flow_telemetry.dot.png)

GCS communication protocol
--------------------------

The protocol for the communication between the GCS and the vehicle is specified by a [Google Protocol Buffers file](https://github.com/cepr/hummingdroid-flightapp/blob/master/src/org/hummingdroid/Communication.proto). All messages are transported by UDP packets on port 49152.

The configuration and command packet is defined by the `CommandPacket` message.

The telemetry packet is defined by the `TelemetryPacket` message.

