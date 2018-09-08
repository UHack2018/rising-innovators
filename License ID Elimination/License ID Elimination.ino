#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()  
{
Serial.begin(9600);
while (!Serial);  // For Yun/Leo/Micro/Zero/...i]
delay(100);
Serial.println("\n\nDelete Finger");
finger.begin(57600);
if (finger.verifyPassword()) {
Serial.println("Found fingerprint sensor!");
} else {
Serial.println("Did not find fingerprint sensor :(");
while (1);
}
}
uint8_t readnumber(void) {
uint8_t num = 0;
while (num == 0) {
while (! Serial.available());
num = Serial.parseInt();
}
return num;
}
void loop()                     
{
Serial.println("PLEASE TYPE YOUR LICENSE NUMBER TO DISABLE");
uint8_t id = readnumber();
if (id == 0) {
return;
  }
Serial.print("DISABLING..........");
Serial.println(id);
deleteFingerprint(id);
}
uint8_t deleteFingerprint(uint8_t id) {
uint8_t p = -1;
p = finger.deleteModel(id);
if (p == FINGERPRINT_OK) {
Serial.println("Deleted!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not delete in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
} else {
Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
return p;
}   
}
