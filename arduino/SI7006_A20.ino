// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SI7006-A20
// This code is designed to work with the SI7006-A20_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SI7006-A20_I2CS#tabs-0-product_tabset-2

#include <Wire.h>

// SI7006-A20 I2C address is 0x40(64)
#define Addr 0x40

byte crc8(byte *addr, byte len)
{
 byte crc = 0;
 while (len--) {
   byte inbyte = *addr++;
   for (byte i = 8; i; i--)
   {
     byte carry = (crc ^ inbyte) & 0x80;
     crc <<= 1;
     if (carry)
       crc ^= 0x31;
     inbyte <<= 1;
   }
 }
 return crc;
}  // end of crc8

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
    uint8_t data[2] = {0};
  
    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Send humidity measurement command, NO HOLD MASTER
    Wire.write(0xF5);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data
    Wire.requestFrom(Addr, 2);

    // Read 2 bytes of data
    // humidity msb, humidity lsb 
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }
    
    // Convert the data
    float humidity  = ((data[0] * 256.0) + data[1]);
    humidity = ((125 * humidity) / 65536.0) - 6;

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Send temperature measurement command, NO HOLD MASTER
    Wire.write(0xF3);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data
    Wire.requestFrom(Addr, 2);

    // Read 2 bytes of data
    // temp msb, temp lsb
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }

    // Convert the data
    float temp  = ((data[0] * 256.0) + data[1]);
    float ctemp = ((175.72 * temp) / 65536.0) - 46.85;
    float ftemp = ctemp * 1.8 + 32;
    
    // Output data to serial monitor
    Serial.print("Relative humidity : ");
    Serial.print(humidity);
    Serial.println(" % RH");
    Serial.print("Temperature in Celsius : ");
    Serial.print(ctemp);
    Serial.println(" C");
    Serial.print("Temperature in Fahrenheit : ");
    Serial.print(ftemp);
    Serial.println(" F");
    delay(500);

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Send temperature measurement command, NO HOLD MASTER
    Wire.write(0xFA);
    Wire.write(0x0F);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(500);

    // Request 8 bytes of data
    Wire.requestFrom(Addr, 8);
    
    // Read 8 bytes of data
    // SNA_3, CRC, SNA_2, CRC, SNA_1, CRC, SNA_0, CRC
    // Ignoring all but last CRC, as it's comprised of all four SNAs
    // Ignore SNB_X data, as it's always 0x6FFFFFF
    if (Wire.available() == 8); {
      data[0] = Wire.read();
      data[1] = Wire.read();
      data[2] = Wire.read();
      data[3] = Wire.read();
      data[4] = Wire.read();
      data[5] = Wire.read();
      data[6] = Wire.read();
      data[7] = Wire.read();
    }

    // Print raw data, followed by received and calculated checksums
    Serial.print("Electronic ID : ");
    Serial.print(data[0], HEX);
    Serial.print(" ");
    Serial.print(data[2], HEX);
    Serial.print(" ");
    Serial.print(data[4], HEX);
    Serial.print(" ");
    Serial.println(data[6], HEX);
    Serial.print("Received checksum : ");
    Serial.println(data[7], HEX);
    Serial.print("Calculated checksum : ");
    byte buf [] = { data[0], data[2], data[4], data[6] };
    byte crc = crc8 (buf, sizeof buf);
    Serial.println(crc, HEX);
    Serial.println();

    delay(2500);
}
