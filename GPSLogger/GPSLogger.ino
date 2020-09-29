
uint32_t timer = millis();

/*

DISPLAY ONLY
Sketch uses 3068 bytes (9%) of program storage space. Maximum is 32256 bytes.
Global variables use 70 bytes (3%) of dynamic memory, leaving 1978 bytes for local variables. Maximum is 2048 bytes.

DISPLAY + GPS
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
#include <Adafruit_GPS.h>
#define GPSSerial Serial
Adafruit_GPS GPS(&GPSSerial);

/* SD Card */
//#include <SPI.h>
//#include "SdFat.h"
//const uint8_t chipSelect = 4;
//#define FILE_BASE_NAME "Data"
//SdFat sd;
//SdFile file;
//const uint8_t ANALOG_COUNT = 4;


/*
void writeHeader() {
    file.print(F("micros"));
    for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
        file.print(F(",adc"));
        file.print(i, DEC);
    }
    file.println();
}

void logData() {
    uint16_t data[ANALOG_COUNT];
    
    // Read all channels to avoid SD write latency between readings.
    for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
        data[i] = analogRead(i);
    }
    // Write data to file.  Start with log time in micros.
    file.print(timer);
    
    // Write ADC data to CSV record.
    for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
        file.write(',');
        file.print(data[i]);
    }
    file.println();
}
*/


void setup() {
    // ********************* LCD Setup ********************* //
    display.begin(&Adafruit128x32, I2C_ADDRESS);
    display.setFont(Adafruit5x7);

    display.clear();
    display.println("Initialised LCD...");
    delay(500);
    
    // ********************* GPS Setup ********************* //
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    GPSSerial.println(PMTK_Q_RELEASE);

    display.println("Initialised GPS...");
    delay(500);

    /*
      if (!SD.begin(chipSelect)) {
      display.println("Card failed, or not present");
      while (1);
      }
    */

    /*
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[13] = FILE_BASE_NAME "00.csv";  
  
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
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
    */
    
    // Write data header.
    //writeHeader();

    // Start on a multiple of the sample interval.
    //logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
    //logTime *= 1000UL*SAMPLE_INTERVAL_MS;

    //display.println("Initialized SD card...");
    //delay(500);
}




void loop() {

    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
    }
    // approximately every 1 seconds or so, print out the current stats
    if (millis() - timer > 1000) {
        timer = millis(); // reset the timer
        display.clear();

        // Hour
        if (GPS.hour < 10) {
            display.print('0');
        }
        display.print(GPS.hour, DEC);
        display.print(':');
        
        // minutes
        if (GPS.minute < 10) {
            display.print('0');
        }
        display.print(GPS.minute, DEC);
        display.print(':');

        // seconds
        if (GPS.seconds < 10) {
            display.print('0');
        }
        display.print(GPS.seconds, DEC);

        display.print(' ');

        // day
        if (GPS.day < 10) {
            display.print('0');
        }
        display.print(GPS.day, DEC);
        display.print('/');

        // months
        if (GPS.month < 10) {
            display.print('0');
        }
        display.print(GPS.month, DEC);

        // year
        display.print("/20");
        display.println(GPS.year, DEC);
        
        // Location
        if (GPS.fix) {
            // Latitude
            display.print(GPS.latitude, 4);
            display.print(GPS.lat);
            display.print(", ");

            // Longitude            
            display.print(GPS.longitude, 4);
            display.println(GPS.lon);

            // Altitude
            display.print(GPS.altitude);
            display.print("m ");


            // Speed
            display.print(GPS.speed * 1.852, 4);
            display.println("km/h ");
                
            display.print(GPS.angle);
            display.print("o, ");
            
            //Serial.print("Satellites: ");
            display.print("s:");
            display.print((int)GPS.satellites);
            display.print(", q:");
            display.print((int)GPS.fixquality);
            // ******* Write to SD card ****************

            /*
            String data_string = "";
            // Hour
            if (GPS.hour < 10) {
                data_string += "0";
            }
            data_string += String(GPS.hour, DEC);
            data_string += ":";
            // minutes
            if (GPS.minute < 10) {
                data_string += "0";
            }
            data_string += String(GPS.minute, DEC);
            data_string += ":";
            // seconds
            if (GPS.seconds < 10) {
                data_string += "0";
            }
            data_string += String(GPS.seconds, DEC);
            data_string += ",";
            // day
            if (GPS.day < 10) {
                data_string += "0";
            }
            data_string += String(GPS.day, DEC);
            data_string += "/";
            // months
            if (GPS.month < 10) {
                data_string += "0";
            }
            data_string += String(GPS.month, DEC);
            // year
            data_string += "/20";
            data_string += String(GPS.year, DEC);

            data_string += ",";

            // Lat
            data_string += String(GPS.latitude, 4);
            data_string += GPS.lat;
            data_string += ",";

            // Longitude            
            data_string += String(GPS.longitude, 4);
            data_string += GPS.lon;
            data_string += ",";

            // Altitude
            data_string += String(GPS.altitude);
            data_string += ",";


            // Speed
            data_string += String(GPS.speed * 1.852, 4);
            data_string += ",";
                
            data_string += String(GPS.angle);
            data_string += ",";
            
            //Serial.print("Satellites: ");
            data_string += String((int)GPS.satellites);
            data_string += ",";
            data_string += String((int)GPS.fixquality);
            */
            
        } // fix found
    } // each timer slice
}


