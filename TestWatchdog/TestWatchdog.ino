#include <avr/wdt.h>

void setup() {
    /* Setup watchdog 
       3 2 1 0 WDP
       0 1 1 0 1000ms
       0 1 1 1 2000ms <-----
       1 0 0 0 4000ms
    */
    cli();
    wdt_reset(); 
    WDTCSR |= (1<<WDCE) | (1<<WDE);  // Enter Watchdog Configuration mode:
    WDTCSR = (1<<WDE) |
        (0<<WDP3) | (1<<WDP2) | (1<<WDP1) |
        (1<<WDP0);
    sei();


    
    Serial.begin(9600, SERIAL_8N1);
    Serial.println("Hello");


    
}



void loop() {
    
    Serial.println("Loop 1");
    delay(1000);
    wdt_reset(); // feed the dog

    Serial.println("Loop 2");
    delay(1800);
    wdt_reset(); // feed the dog

    Serial.println("Loop 3");
    delay(3000);
    wdt_reset(); // feed the dog late

    Serial.println("Loop 4 - shouldnt happen");
}
