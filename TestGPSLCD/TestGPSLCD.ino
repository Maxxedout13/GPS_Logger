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
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);
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

    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
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

        
        // Location
        if (GPS.fix) {
            // Latitude
            
            display.print(GPS.latitude, 4);
            display.print(GPS.lat);
            display.print(", ");

            // Longitude            
            display.print(GPS.longitude, 4);
            display.print(GPS.lon);
            display.print(", ");

            // Altitude
            display.print(GPS.altitude);
            display.print("m, ");


            // Speed
            display.print(GPS.speed * 1.852, 4);
            display.print("km/h, ");
                
            display.print(GPS.angle);
            display.print("o, ");
            
            //Serial.print("Satellites: ");
            display.print("s:");
            display.print((int)GPS.satellites);
            display.print(", q:");
            display.print((int)GPS.fixquality);

        }
        display.display();

    }






    
}


