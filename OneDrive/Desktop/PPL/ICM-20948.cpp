/****************************************************************
 * Example1_Basics.ino
 * ICM 20948 Arduino Library Demo
 * Use the default configuration to stream 9-axis IMU data
 * Owen Lyke @ SparkFun Electronics
 * Original Creation Date: April 17 2019
 *
 * Please see License.md for the license information.
 *
 * Distributed as-is; no warranty is given.
 ***************************************************************/
#include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU

//#define USE_SPI       // Uncomment this to use SPI

#define SERIAL_PORT Serial

#define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
#define CS_PIN 2     // Which pin you connect CS to. Used only when "USE_SPI" is defined

#define WIRE_PORT Wire // Your desired Wire port.      Used when "USE_SPI" is not defined
// The value of the last bit of the I2C address.
// On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
#define AD0_VAL 1

//never sends each sesor data one by one. different bytes for different readings
// Define structures to hold sensor data
struct AccelerometerData {
  float x;
  float y;
  float z;
};

struct GyroscopeData {
  float x;
  float y;
  float z;
};

struct MagnetometerData {
  float x;
  float y;
  float z;
};

// Functions to get sensor data
AccelerometerData getAccelerometerData(ICM_20948_I2C *sensor) {
  AccelerometerData data;
  data.x = sensor->accX();
  data.y = sensor->accY();
  data.z = sensor->accZ();
  return data;
}

GyroscopeData getGyroscopeData(ICM_20948_I2C *sensor) {
  GyroscopeData data;
  data.x = sensor->gyrX();
  data.y = sensor->gyrY();
  data.z = sensor->gyrZ();
  return data;
}

MagnetometerData getMagnetometerData(ICM_20948_I2C *sensor) {
  MagnetometerData data;
  data.x = sensor->magX();
  data.y = sensor->magY();
  data.z = sensor->magZ();
  return data;
}

// O B J E C T  C R E A T I O N /////////////////////////////////////////////////////

#ifdef USE_SPI
ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object
#else
ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
#endif

// S E T U P ///////////////////////////////////////////////////////////////////
// initializes the serial port, communication port and the sensor
void setup() {
  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT) { }

#ifdef USE_SPI
  SPI_PORT.begin();
#else
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
#endif

  //myICM.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  bool initialized = false;
  while (!initialized) {
#ifdef USE_SPI
    myICM.begin(CS_PIN, SPI_PORT);
#else
    myICM.begin(WIRE_PORT, AD0_VAL);
#endif

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok) {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }
}

// L O O P /////////////////////////////////////////////////////////////////////
// Continuously reads the sensor and prints the data
void loop() {
  if (myICM.dataReady()) {
    myICM.getAGMT(); // The values are only updated when you call 'getAGMT'
    
    // Get sensor data
    AccelerometerData accData = getAccelerometerData(&myICM);
    GyroscopeData gyroData = getGyroscopeData(&myICM);
    MagnetometerData magData = getMagnetometerData(&myICM);
    
    // Print sensor data
    SERIAL_PORT.print("Acc (mg): X = ");
    SERIAL_PORT.print(accData.x);
    SERIAL_PORT.print(", Y = ");
    SERIAL_PORT.print(accData.y);
    SERIAL_PORT.print(", Z = ");
    SERIAL_PORT.println(accData.z);
    
    SERIAL_PORT.print("Gyr (DPS): X = ");
    SERIAL_PORT.print(gyroData.x);
    SERIAL_PORT.print(", Y = ");
    SERIAL_PORT.print(gyroData.y);
    SERIAL_PORT.print(", Z = ");
    SERIAL_PORT.println(gyroData.z);
    
    SERIAL_PORT.print("Mag (uT): X = ");
    SERIAL_PORT.print(magData.x);
    SERIAL_PORT.print(", Y = ");
    SERIAL_PORT.print(magData.y);
    SERIAL_PORT.print(", Z = ");
    SERIAL_PORT.println(magData.z);
    
    delay(30);
  } else {
    SERIAL_PORT.println("Waiting for data");
    delay(500);
  }
}