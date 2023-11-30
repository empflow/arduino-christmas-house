#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

const char* AUTHORIZED_UIDS[] = {
  "b3ea810e"
};

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  servo.attach(3);
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
  servo.write(0);
  printAuthorizedIds();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = getUid(rfid);
  Serial.println(uid);

  if (isAuthorized(uid)) {
    servo.write(150);
    delay(3000);
  }
  servo.write(0);
  
  delay(1500);
}

bool isAuthorized(String uid) {
  int uidsAmount = sizeof(AUTHORIZED_UIDS) / sizeof(AUTHORIZED_UIDS[0]);
  for (int i = 0; i < uidsAmount; i++) {
    if (strcmp(AUTHORIZED_UIDS[i], uid.c_str()) == 0) {
      Serial.println("Authorized");
      return true;
    }
  }

  Serial.println("Unauthorized");
  return false;
}

String getUid(MFRC522 rfid) {
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
  }
  return ID;
}

void printAuthorizedIds() {
  Serial.println("Authorized UIDs:");
  for (int i = 0; i < sizeof(AUTHORIZED_UIDS) / sizeof(AUTHORIZED_UIDS[0]); i++) {
    Serial.println(AUTHORIZED_UIDS[i]);
  }
}
