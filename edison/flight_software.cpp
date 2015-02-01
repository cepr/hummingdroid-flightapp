
// The SFE_LSM9DS0 requires both the SPI and Wire libraries.
// Unfortunately, you'll need to include both in the Arduino
// sketch, before including the SFE_LSM9DS0 library.
#include <Arduino.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include <SFE_LSM9DS0.h>
#include <Adafruit_PWMServoDriver.h>

// SDO_XM and SDO_G are both grounded, so our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
// Create an instance of the LSM9DS0 library called `dof` the
// parameters for this constructor are:
// [SPI or I2C Mode declaration],[gyro I2C address],[xm I2C add.]
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// Do you want to print calculated values or raw ADC ticks read
// from the sensor? Comment out ONE of the two #defines below
// to pick:
#define PRINT_CALCULATED
//#define PRINT_RAW

#define PRINT_SPEED 500 // 500 ms between prints

static void printGyro();
static void printAccel();
static void printMag();
static void printHeading(float hx, float hy);
static void printOrientation(float x, float y, float z);

int main()
{
    // Use the begin() function to initialize the LSM9DS0 library.
    // You can either call it with no parameters (the easy way):
    uint16_t status = dof.begin();
    // Or call it with declarations for sensor scales and data rates:
    //uint16_t status = dof.begin(dof.G_SCALE_2000DPS,
    //                            dof.A_SCALE_6G, dof.M_SCALE_2GS);

    // begin() returns a 16-bit value which includes both the gyro
    // and accelerometers WHO_AM_I response. You can check this to
    // make sure communication was successful.
    printf("LSM9DS0 WHO_AM_I's returned: 0x%04d\n"
           "Should be 0x49D4\n"
           "\n", (int)status);

    pwm.begin();
    pwm.setPWMFreq(400);
    pwm.setPWM(0, 0, 50);
    pwm.setPWM(1, 0, 50);
    pwm.setPWM(2, 0, 50);
    pwm.setPWM(3, 0, 50);

    //loop
    while (true){
      printGyro();  // Print "G: gx, gy, gz"
      printAccel(); // Print "A: ax, ay, az"
      printMag();   // Print "M: mx, my, mz"

      // Print the heading and orientation for fun!
      printHeading((float) dof.mx, (float) dof.my);
      printOrientation(dof.calcAccel(dof.ax), dof.calcAccel(dof.ay),
                       dof.calcAccel(dof.az));
      putchar('\n');

      delay(PRINT_SPEED);
    }

    return 0;
}

static void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  dof.readGyro();

  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  printf("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  printf("%.2f, %.2f, %.2f\n", dof.calcGyro(dof.gx), dof.calcGyro(dof.gy), dof.calcGyro(dof.gz));
#elif defined PRINT_RAW
  printf("%d, %d, %d\n", dof.gx, dof.gy, dof.gz);
#endif
}

static void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  dof.readAccel();

  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  printf("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  printf("%.2f, %.2f, %.2f\n", dof.calcAccel(dof.ax), dof.calcAccel(dof.ay), dof.calcAccel(dof.az));
#elif defined PRINT_RAW
  printf("%d, %d, %d\n", dof.ax, dof.ay, dof.az);
#endif

}

static void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  dof.readMag();

  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  printf("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  printf("%.2f, %.2f, %.2f\n", dof.calcMag(dof.mx), dof.calcMag(dof.my), dof.calcMag(dof.mz));
#elif defined PRINT_RAW
  printf("%d, %d, %d\n", dof.mx, dof.my, dof.mz);
#endif
}

// Here's a fun function to calculate your heading, using Earth's
// magnetic field.
// It only works if the sensor is flat (z-axis normal to Earth).
// Additionally, you may need to add or subtract a declination
// angle to get the heading normalized to your location.
// See: http://www.ngdc.noaa.gov/geomag/declination.shtml
static void printHeading(float hx, float hy)
{
  float heading;

  if (hy > 0)
  {
    heading = 90 - (atan(hx / hy) * (180 / PI));
  }
  else if (hy < 0)
  {
    heading = - (atan(hx / hy) * (180 / PI));
  }
  else // hy = 0
  {
    if (hx < 0) heading = 180;
    else heading = 0;
  }

  printf("Heading: %f\n", heading);
}

// Another fun function that does calculations based on the
// acclerometer data. This function will print your LSM9DS0's
// orientation -- it's roll and pitch angles.
static void printOrientation(float x, float y, float z)
{
  float pitch, roll;

  pitch = atan2(x, sqrt(y * y) + (z * z));
  roll = atan2(y, sqrt(x * x) + (z * z));
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;

  printf("Pitch, Roll: %.2f, %.2f\n", pitch, roll);
}
