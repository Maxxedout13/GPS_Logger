
void setup() {
     Serial.begin(9600);
     while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
     }
     Serial.println("hello\n");
}



void loop () {


     Serial.println("world\n");
}
