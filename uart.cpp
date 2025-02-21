#include <SoftwareSerial.h>
#include <UbxGpsNavPosllh.h> //specifies packet type as UBX-NAV-POSLLH (0x01 0x02)

#define BAUDRATE 115200
#define GPS_RX 2 //modify as needed after sensor and actuation
#define GPS_TX 3 //modify as needed after sensor and actuation

SoftwareSerial ss(GPS_RX,GPS_TX);
UbxGpsNavPosllh<SoftwareSerial> gps(ss); 

void setup()
{
    Serial.begin(BAUDRATE);
    gps.begin(BAUDRATE);

}

void loop()
{
    if gps.ready())
    {
        Serial.print(gps.lat*10000000.0,7);
        Serial.print(gps.lon*10000000.0,7);
        Serial.print(gps.height*1000.0,3);
    }
}
