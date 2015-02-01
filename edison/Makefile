
# Compilation flags
CPPFLAGS+=\
	-DARDUINO=153 \
	-I "libs/arduino-1.5.3-Intel.1.0.4" \
	-I libs/Wire \
	-I libs/SPI \
	-I libs/WiFi

# Make sure we link using g++ and not gcc
LINK.o:=$(LINK.cpp)

all: flight_software

OBJS:=\
	flight_software.o \
	libs/LSM9DS0_Breakout/Libraries/Arduino/SFE_LSM9DS0/SFE_LSM9DS0.o \
	libs/SPI/SPI.o \
	libs/WiFi/WiFiClient.o \
	libs/WiFi/WiFi.o \
	libs/WiFi/Dhcp.o \
	libs/WiFi/Dns.o \
	libs/WiFi/WiFiUdp.o \
	libs/WiFi/WiFiServer.o \
	libs/Wire/Wire.o \
	flight_software.o

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
flight_software: $(OBJS) arduino_core.a

clean:
	$(RM) flight_software arduino_core.a $(OBJS) $(ARDUINO_OBJS)

#/home/cpriscal/bin/arduino-1.5.3-Intel.1.0.4/hardware/tools/edison/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-g++ -m32 -march=i586 --sysroot=/home/cpriscal/bin/arduino-1.5.3-Intel.1.0.4/hardware/tools/edison/sysroots/core2-32-poky-linux -Os -Wl,--gc-sections -march=i686 -o /tmp/build7073233215406790678.tmp/flight_software.cpp.elf /tmp/build7073233215406790678.tmp/flight_software.cpp.o /tmp/build7073233215406790678.tmp/SPI/SPI.cpp.o /tmp/build7073233215406790678.tmp/Wire/Wire.cpp.o /tmp/build7073233215406790678.tmp/SFE_LSM9DS0/SFE_LSM9DS0.cpp.o /tmp/build7073233215406790678.tmp/core.a -L/tmp/build7073233215406790678.tmp -lm -lpthread 
#/home/cpriscal/bin/arduino-1.5.3-Intel.1.0.4/hardware/tools/edison/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-strip /tmp/build7073233215406790678.tmp/flight_software.cpp.elf 
