
uint32_t timer = millis();

/*

DISPLAY ONLY
Sketch uses 3068 bytes (9%) of program storage space. Maximum is 32256 bytes.
Global variables use 70 bytes (3%) of dynamic memory, leaving 1978 bytes for local variables. Maximum is 2048 bytes.

DISPLAY + Adafruit GPS
Sketch uses 15216 bytes (47%) of program storage space. Maximum is 32256 bytes.
Global variables use 1287 bytes (62%) of dynamic memory, leaving 761 bytes for local variables. Maximum is 2048 bytes.




*/

/* Display */
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiAvrI2c display;

/* GPS */
#include <NMEAGPS.h>
#define Serial1 Serial
#include <GPSport.h>
#include <GPSfix.h>
#include <Streamers.h>
static NMEAGPS  gps;
static gps_fix  fix;


/* SD Card */
#include <SPI.h>
#include "SdFat.h"
const uint8_t chipSelect = 4;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;


/* Acc */
#include <Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;




void writeHeader() {
    file.print(F("time,date,lat,lon,alt_m,speed_km_h,bearing,AccX,AccY,AccZ,GyroX,GyroY,GyroZ"));
    file.println();
}


void setup() {
    // ********************* LCD Setup ********************* //
    display.begin(&Adafruit128x32, I2C_ADDRESS);
    display.setFont(Adafruit5x7);

    display.clear();
    display.println("Initialised LCD...");
    delay(500);
    
    // ********************* GPS Setup ********************* //
    gpsPort.begin( 9600 );
    display.println("Initialised GPS...");
    delay(500);

    // ******************** SD CARD ************************ //
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[13] = FILE_BASE_NAME "00.csv";  
  
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
        display.println("sd.initErrorHalt...");
        sd.initErrorHalt();
    }
    // Find an unused file name.
    if (BASE_NAME_SIZE > 6) {
        display.println("FILE_BASE_NAME too long");
    }
    while (sd.exists(fileName)) {
        if (fileName[BASE_NAME_SIZE + 1] != '9') {
            fileName[BASE_NAME_SIZE + 1]++;
        } else if (fileName[BASE_NAME_SIZE] != '9') {
            fileName[BASE_NAME_SIZE + 1] = '0';
            fileName[BASE_NAME_SIZE]++;
        } else {
            display.println("Can't create file name");
        }
    }
    if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
        display.println("file.open");
    }
    display.println(fileName);

    /* Acc */
    Wire.begin(); 
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    
    // Write data header.
    writeHeader();
    delay(500);
}



void write_to_sd() {
    String data_string = "";
    // Hour
    if (fix.dateTime.hours < 10) {
        data_string += "0";
    }
    data_string += String(fix.dateTime.hours, DEC);
    data_string += ":";
    // minutes
    if (fix.dateTime.minutes < 10) {
        data_string += "0";
    }
    data_string += String(fix.dateTime.minutes, DEC);
    data_string += ":";
    // seconds
    if (fix.dateTime.seconds < 10) {
        data_string += "0";
    }
    data_string += String(fix.dateTime.seconds, DEC);
    data_string += ",";
    // day
    if (fix.dateTime.date < 10) {
        data_string += "0";
    }
    data_string += String(fix.dateTime.date, DEC);
    data_string += "/";
    // months
    if (fix.dateTime.month < 10) {
        data_string += "0";
    }
    data_string += String(fix.dateTime.month, DEC);
    // year
    data_string += "/20";
    data_string += String(fix.dateTime.year, DEC);
    
    data_string += ",";
    
    // Lat
    data_string += String(fix.latitude(), 8);
    data_string += ",";
    
    // Longitude            
    data_string += String(fix.longitude(), 8);
    data_string += ",";
    
    // Altitude
    data_string += String(fix.altitude());
    data_string += ",";
    
    // Speed & heading
    data_string += String(fix.speed_kph(), 4);
    data_string += ",";
    data_string += String(fix.heading());
    data_string += ",";

    // AccX,Y,Z
    data_string += String(AccX);
    data_string += ",";
    data_string += String(AccY);
    data_string += ",";
    data_string += String(AccZ);
    data_string += ",";

    // Gyros X,Y,Z
    data_string += String(GyroX);
    data_string += ",";
    data_string += String(GyroY);
    data_string += ",";
    data_string += String(GyroZ);
    
    file.print(data_string);
    file.println();

    if (!file.sync() || file.getWriteError()) {
        display.println("write error");
        delay(1000);
    }

}


void handle_fix() {
    display.clear();

    /*
    display.print(AccX, DEC);
    display.print("/");
    display.print(AccY, DEC);
    display.print("/");
    display.print(AccZ, DEC);
    display.println("");

    display.print(GyroX, DEC);
    display.print("/");
    display.print(GyroY, DEC);
    display.print("/");
    display.print(GyroZ, DEC);
    display.println("");
    */

    

    // Hour
    if (fix.dateTime.hours < 10) {
        display.print('0');
    }
    display.print(fix.dateTime.hours, DEC);
    display.print(':');
    // minutes
    if (fix.dateTime.minutes < 10) {
        display.print('0');
    }
    display.print(fix.dateTime.minutes, DEC);
    display.print(':');
    // seconds
    if (fix.dateTime.seconds < 10) {
        display.print('0');
    }
    display.print(fix.dateTime.seconds, DEC);
    display.print(' ');
    // day
    if (fix.dateTime.date < 10) {
        display.print('0');
    }
    display.print(fix.dateTime.date, DEC);
    display.print('/');
    // months
    if (fix.dateTime.month < 10) {
        display.print('0');
    }
    display.print(fix.dateTime.month, DEC);  
    // year
    display.print("/20");
    display.println(fix.dateTime.year, DEC);


    
    if (fix.valid.location) {
        // Latitude
        display.println(fix.latitude(), 8);
        // Longitude            
        display.println(fix.longitude(), 8);
        
        // Altitude
        display.print(fix.altitude());
        display.print("m ");

        // Speed & Heading
        display.print(fix.speed_kph(), 4);
        display.println("km/h ");
        //display.print(fix.heading());
        //display.print("o");
    }
    write_to_sd();
    delay(200);
}


void loop() {
    while (gps.available(gpsPort)) {
        fix = gps.read();
        switch (fix.status) {
        case fix.STATUS_NONE:
            //display.println("Fix... STATUS_NONE"); 
            break;
        case fix.STATUS_EST:
            //display.println("Fix... STATUS_EST"); 
            break;
        case fix.STATUS_TIME_ONLY:
            //display.println("Fix... STATUS_TIME_ONLY"); 
            break;
        case fix.STATUS_STD:
            handle_fix();
            break;
        case fix.STATUS_DGPS:
            //display.println("Fix... STATUS_DGPS"); 
            break;
        default:
            display.println("Fix... unknown"); 
        }
    } // gps available

    

    digitalWrite(12, LOW);
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    digitalWrite(12, HIGH);

    //handle_fix();
    
}


