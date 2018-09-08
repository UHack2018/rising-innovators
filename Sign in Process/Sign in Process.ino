
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
const int motor=5;
const int busser=4;
const int led=6;
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()  
{
pinMode(busser,OUTPUT);
pinMode(motor,OUTPUT);
pinMode(led,OUTPUT);
Serial.begin(9600);
while (!Serial); 
delay(100);
Serial.println("\n\n DRIVING LICENSE LOGIN");
finger.begin(57600);
if (finger.verifyPassword()) {
Serial.println("Found fingerprint sensor!");
} else {
Serial.println("Did not find fingerprint sensor :(");
while (1) { delay(1); }
}
Serial.println("PLACE YOUR PRINTS TO START YOUR VEHICLE");
}
void loop()                     
{
getFingerprintIDez();
delay(50);            
}
uint8_t getFingerprintID() {
uint8_t p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println("No finger detected");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
return p;
default:
Serial.println("Unknown error");
return p;
}
p = finger.image2Tz();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
return p;
}
p = finger.fingerFastSearch();
if (p == FINGERPRINT_OK) {
Serial.println("Found a print match!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_NOTFOUND) {
Serial.println("Did not find a match");
return p;
} else {
Serial.println("Unknown error");
return p;
}   
Serial.print("FOUND LICENSE NO: #"); Serial.print(finger.fingerID); 
Serial.print(" with confidence of "); Serial.println(finger.confidence); 
return finger.fingerID;
}
int getFingerprintIDez() {
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK)  return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK)  return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)  return -1;
Serial.print("FOUND LICENSE NO: #"); Serial.print(finger.fingerID); 
digitalWrite(motor,HIGH);
digitalWrite(led,HIGH);
digitalWrite(busser,HIGH);
delay(1000);
digitalWrite(busser,LOW);
return finger.fingerID; 
}
