#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "";
const char* AUTHORIZED_UIDS[] = {
  "b3ea810e"
};
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  servo.write(70);
  servo.attach(3);
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
  for (int i = 0; i < sizeof(AUTHORIZED_UIDS) / sizeof(AUTHORIZED_UIDS[0]); i++) {
    Serial.println("Authorized UIDs:");
    Serial.print(AUTHORIZED_UIDS[i]);
    Serial.print("");
  }
  Serial.println("");
}

void loop() {
  
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = getUid(rfid);

  Serial.println(uid);
  isAuthorized(rfid);
  
  delay(1500);
}

bool isAuthorized(MFRC522 rfid) {
  String providedUid = getUid(rfid);

  int uidsAmount = sizeof(AUTHORIZED_UIDS) / sizeof(AUTHORIZED_UIDS[0]);
  for (int i = 0; i < uidsAmount; i++) {
    if (strcmp(AUTHORIZED_UIDS[i], providedUid.c_str()) == 0) {
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
