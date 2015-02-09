
all: flight_software

-include libs/Adafruit-PWM-Servo-Driver-Library/Adafruit_PWMServoDriver.d
-include libs/LSM9DS0_Breakout/Libraries/Arduino/SFE_LSM9DS0/SFE_LSM9DS0.d
-include libs/SPI/SPI.d
-include libs/WiFi/WiFiClient.d
-include libs/WiFi/WiFi.d
-include libs/WiFi/Dhcp.d
-include libs/WiFi/Dns.d
-include libs/WiFi/WiFiUdp.d
-include libs/WiFi/WiFiServer.d
-include libs/Wire/Wire.d
-include Communication.pb.d
-include Motors.d
-include Telemetry.d
-include DatagramSocket.d
-include Object.d
-include Value.d
-include Receiver.d
-include FlightService.d
-include Telemetry.d
-include Sensors.d
-include Thread.d
-include Timestamp.d
-include Controller.d
-include libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_pci.d
-include libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_sc.d
-include libs/arduino-1.5.3-Intel.1.0.4/wiring_digital.d
-include libs/arduino-1.5.3-Intel.1.0.4/sysfs.d
-include libs/arduino-1.5.3-Intel.1.0.4/wiring_analog.d
-include libs/arduino-1.5.3-Intel.1.0.4/trace.d
-include libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_nc.d
-include libs/arduino-1.5.3-Intel.1.0.4/i2c.d
-include libs/arduino-1.5.3-Intel.1.0.4/mux.d
-include libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_common.d
-include libs/arduino-1.5.3-Intel.1.0.4/interrupt.d
-include libs/arduino-1.5.3-Intel.1.0.4/Stream.d
-include libs/arduino-1.5.3-Intel.1.0.4/WString.d
-include libs/arduino-1.5.3-Intel.1.0.4/Tone.d
-include libs/arduino-1.5.3-Intel.1.0.4/pulseIn.d
-include libs/arduino-1.5.3-Intel.1.0.4/UtilMisc.d
-include libs/arduino-1.5.3-Intel.1.0.4/TTYUART.d
-include libs/arduino-1.5.3-Intel.1.0.4/main.d
-include libs/arduino-1.5.3-Intel.1.0.4/UtilTime.d
-include libs/arduino-1.5.3-Intel.1.0.4/WMath.d
-include libs/arduino-1.5.3-Intel.1.0.4/Print.d
-include libs/arduino-1.5.3-Intel.1.0.4/IPAddress.d
-include libs/arduino-1.5.3-Intel.1.0.4/RingBuffer.d
-include libs/arduino-1.5.3-Intel.1.0.4/softwarepwm.d
-include libs/arduino-1.5.3-Intel.1.0.4/variant.d

# Compilation flags
CPPFLAGS+=\
	-DARDUINO=153 \
	-I "libs/arduino-1.5.3-Intel.1.0.4" \
	-I libs/Wire \
	-I libs/SPI \
	-I libs/WiFi \
	-I libs/LSM9DS0_Breakout/Libraries/Arduino/SFE_LSM9DS0 \
	-I libs/Adafruit-PWM-Servo-Driver-Library \
	-I libs/protobuf/src \
	-Wall \
	-O0 \
	-MD

# Make sure we link using g++ and not gcc
LINK.o:=$(LINK.cpp)

OBJS:=\
	libs/Adafruit-PWM-Servo-Driver-Library/Adafruit_PWMServoDriver.o \
	libs/LSM9DS0_Breakout/Libraries/Arduino/SFE_LSM9DS0/SFE_LSM9DS0.o \
	libs/SPI/SPI.o \
	libs/WiFi/WiFiClient.o \
	libs/WiFi/WiFi.o \
	libs/WiFi/Dhcp.o \
	libs/WiFi/Dns.o \
	libs/WiFi/WiFiUdp.o \
	libs/WiFi/WiFiServer.o \
	libs/Wire/Wire.o \
	Communication.pb.o \
	Motors.o \
	Telemetry.o \
	DatagramSocket.o \
	Object.o \
	Value.o \
	Receiver.o \
	FlightService.o \
	Telemetry.o \
	Sensors.o \
	Thread.o \
	Timestamp.o \
	Controller.o

ARDUINO_OBJS:=\
	libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_pci.o \
	libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_sc.o \
	libs/arduino-1.5.3-Intel.1.0.4/wiring_digital.o \
	libs/arduino-1.5.3-Intel.1.0.4/sysfs.o \
	libs/arduino-1.5.3-Intel.1.0.4/wiring_analog.o \
	libs/arduino-1.5.3-Intel.1.0.4/trace.o \
	libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_nc.o \
	libs/arduino-1.5.3-Intel.1.0.4/i2c.o \
	libs/arduino-1.5.3-Intel.1.0.4/mux.o \
	libs/arduino-1.5.3-Intel.1.0.4/fast_gpio_common.o \
	libs/arduino-1.5.3-Intel.1.0.4/interrupt.o \
	libs/arduino-1.5.3-Intel.1.0.4/Stream.o \
	libs/arduino-1.5.3-Intel.1.0.4/WString.o \
	libs/arduino-1.5.3-Intel.1.0.4/Tone.o \
	libs/arduino-1.5.3-Intel.1.0.4/pulseIn.o \
	libs/arduino-1.5.3-Intel.1.0.4/UtilMisc.o \
	libs/arduino-1.5.3-Intel.1.0.4/TTYUART.o \
	libs/arduino-1.5.3-Intel.1.0.4/main.o \
	libs/arduino-1.5.3-Intel.1.0.4/UtilTime.o \
	libs/arduino-1.5.3-Intel.1.0.4/WMath.o \
	libs/arduino-1.5.3-Intel.1.0.4/Print.o \
	libs/arduino-1.5.3-Intel.1.0.4/IPAddress.o \
	libs/arduino-1.5.3-Intel.1.0.4/RingBuffer.o \
	libs/arduino-1.5.3-Intel.1.0.4/softwarepwm.o \
	libs/arduino-1.5.3-Intel.1.0.4/variant.o 

arduino_core.a: $(ARDUINO_OBJS)
	ar r $@ $^

flight_software: LDLIBS+=-lpthread
flight_software: $(OBJS) arduino_core.a libs/protobuf/build/src/.libs/libprotobuf-lite.a
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: install
install: flight_software
	ssh root@quadcopter.local 'systemctl stop hummingdroid.service'
	scp $< root@quadcopter.local:/home/root
	ssh root@quadcopter.local 'sync'
	ssh root@quadcopter.local 'systemctl start hummingdroid.service'

libs/protobuf/build/src/.libs/libprotobuf-lite.a:
	mkdir -p libs/protobuf/build
	cd libs/protobuf/build && ../configure && make

Communication.pb.cc: Communication.proto libs/protobuf/build/src/.libs/libprotobuf-lite.a
	libs/protobuf/build/src/protoc --cpp_out=. Communication.proto

.PHONY: clean
clean:
	rm -rf \
		flight_software \
		arduino_core.a \
		$(OBJS) \
		$(ARDUINO_OBJS) \
		libs/protobuf/build \
		Communication.pb.h \
		Communication.pb.cc
