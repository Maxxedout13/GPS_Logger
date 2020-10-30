

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiAvrI2c display;


/* SD Card */
#include <SPI.h>
#include "SdFat.h"
const uint8_t chipSelect = 4;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;
const uint8_t ANALOG_COUNT = 4;





void setup() {
    // Open serial communications and wait for port to open:
    //Serial.begin(9600);
    //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
    //}
    display.begin(&Adafruit128x32, I2C_ADDRESS);
    display.setFont(Adafruit5x7);
    uint32_t m = micros();
    display.clear();

    display.println("initialising sd...");
    delay(1000);
    



    
    // ******************** SD CARD ************************ //
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[13] = FILE_BASE_NAME "00.csv";  
  
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
        display.println("sd.initErrorHalt");
        sd.initErrorHalt();
    }
    display.println("sd initialised");

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
    delay(2000);

}


void loop() {
    
    display.clear();
    String data_string = "";
    data_string += "hello there.";

    display.println(data_string);
        
    file.print(data_string);
    file.println();
    
    if (!file.sync() || file.getWriteError()) {
        display.println("write error");
        delay(1000);
    }
    delay(1000);
}









