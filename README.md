# GPS_Logger
Arduino code for GPS logger


# Components
## Arduino Ethernet
* pins: https://i.stack.imgur.com/9mk4A.jpg

## LCD screen
* Product: https://www.amazon.co.uk/128x32-Display-Module-SSD1306-Arduino/dp/B0759VFHSG
* Arduino code: https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/

## GPS device
* Product: https://www.amazon.co.uk/gp/product/B01H1R8BK0/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1
* Doc: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf
* Direct connection to computer: https://learn.adafruit.com/adafruit-ultimate-gps/direct-computer-wiring
* Arduino wiring: https://learn.adafruit.com/adafruit-ultimate-gps/arduino-wiring
* Parsing the data with arduino: https://learn.adafruit.com/adafruit-ultimate-gps/parsed-data-output

# Proposed plan
1. [ ] Direct computer connection for GPS module
2. [ ] Breadboard LCD module + Arduino as debug/logging facility [TestLCD.ino]
3. [ ] Breadboard GPS module + Arduino + LCD(debug) [TestGPSLCD.ino]
4. [ ] SDCard test [TestSD/TestSD.ino] - we've got this one, let's check it still works
5. [ ] Solder the GPS module and LCD module on the veroboard and wire them up
6. [ ] Test each facility again separately (LCD [ ] , GPS [ ], SD-card [ ])
7. [ ] Put it all together [GPSLogger/GPSLogger.ino]



# Questions:
1. How often can/will we poll? 
2. How much data will that generate? Is text file still a good idea? How long is a work session? What size SD card do we need?
3. What do we show on the LCD? I suggest GPS time, and maybe number of measurements or some such counter. 
4. Do we do any signal processing? E.g. get 15 measurements (within 1 second) and average them for location while doing a multi-point derivative for speed?















