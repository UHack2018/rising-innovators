#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
const int busser=5;
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
   pinMode(busser,OUTPUT);
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  Serial.println("\n\n***************LICENSE REGISTRATION***************");
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor :)");
  } else {
    Serial.println("sorry we can't find fingerprint sensor :(");
    while (1) { delay(1); }
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
  Serial.println("READY TO REGISTER YOUR FINGERPRINTS WITH YOUR LICENSE");
  Serial.println("ENTER YOUR LICENSE NUMBER WHICH IS ALLOCATED TO YOU BY THE GOVERNMENT");
  id = readnumber();
  if (id == 0) {
     return;
  }
  Serial.print("Enrolling you *****************");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("KEEP YOUR PRINTS ON THE SCANNER FOR THE ID - "); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("model captured");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Network error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Image sensing error,try again");
      break;
    default:
      Serial.println("Unexpectedly error had occured please try again");
      break;
    }
  }
p = finger.image2Tz(1);
switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Fingerprint Model converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(" too messy,please try again");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Network error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint Image features");
      return p;
    default:
      Serial.println("Unexpectedly error had occurred,please try again");
      return p;
  }
  Serial.println("PRINTS CAPTURED,KINDLY REMOVE YOUR PRINTS");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("PLACE THE SAME FINGER AGAIN");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Network error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Network error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("image features missing");
      return p;
    default:
      Serial.println("Unexpectedly error had occurred");
      return p;
  }
  
  
  Serial.print("CREATING MODEL FOR ID -  #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Network error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("STORED ! .. READY TO DRIVE");
    digitalWrite(busser,HIGH);
  delay(1000);
  digitalWrite(busser,LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
