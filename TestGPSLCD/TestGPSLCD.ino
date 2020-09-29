/*
Adafruit GPS
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <Adafruit_GPS.h>
#define GPSSerial Serial
Adafruit_GPS GPS(&GPSSerial);
uint32_t timer = millis();



void setup() {
    // ********************* LCD Setup ********************* //

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Initialised LCD...");
    display.display();
    delay(500);
    
    // ********************* GPS Setup ********************* //
    //Serial.begin(115200);
    //Serial.println("Adafruit GPS library basic test!");
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz
    // Request updates on antenna status, comment out to keep quiet
    GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);
    // Ask for firmware version
    GPSSerial.println(PMTK_Q_RELEASE);

    display.println("Initialised GPS...");
    display.display();
    delay(500);

}



void loop() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    //display.println("Loop...");
    //display.display();
    //delay(500);

    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    if (GPS.newNMEAreceived()) {
        //display.println("received:");
        //display.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        //display.display();
        //delay(100);
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
    } else {
        //display.println("None...");
        //display.display();
        //delay(100);
    }

    // approximately every 1 seconds or so, print out the current stats
    if (millis() - timer > 1000) {
        timer = millis(); // reset the timer

        display.clearDisplay();

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

        display.display();


        //Serial.print("Fix: "); Serial.print((int)GPS.fix);
        //Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
        //if (GPS.fix) {
        //    Serial.print("Location: ");
        //    Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
        //    Serial.print(", ");
        //    Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
        //    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
        //    Serial.print("Angle: "); Serial.println(GPS.angle);
        //    Serial.print("Altitude: "); Serial.println(GPS.altitude);
        //    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
        //}
    }






    
}


