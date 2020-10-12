#include <Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

// Ethernet A4 (SDA), A5 (SCL)

void setup() {
    Serial.begin(9600, SERIAL_8N1);
    Serial.print("Hello");
    
    Wire.begin(); 
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void loop() {
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

    
    Serial.print("AccX = "); Serial.print(AccX);
    Serial.print(" || AccY = "); Serial.print(AccY);
    Serial.print(" || AccZ = "); Serial.print(AccZ);
    Serial.print(" || Temp = "); Serial.print(Temp/340.00+36.53);
    Serial.print(" || GyroX = "); Serial.print(GyroX);
    Serial.print(" || GyroY = "); Serial.print(GyroY);
    Serial.print(" || GyroZ = "); Serial.println(GyroZ);
    //delay(1000);
}
