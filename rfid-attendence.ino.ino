#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

#define BUZZER 8
#define GREEN_LED A0
#define RED_LED A1

MFRC522 rfid(SS_PIN, RST_PIN);

// LCD pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// UID
String saurabhUID = "499b3612";
String devUID     = "b75fed9e";

// IN/OUT status
bool saurabhIn = false;
bool devIn = false;

// 🔥 Scrolling function
void scrollText(String text1, String text2) {
  for (int i = 0; i < 16; i++) {
    lcd.clear();
    lcd.setCursor(16 - i, 0);
    lcd.print(text1);

    lcd.setCursor(16 - i, 1);
    lcd.print(text2);

    delay(200);
  }
}

// 🔥 Double Beep Function
void doubleBeep() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(150);
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lcd.begin(16, 2);

  // Startup scrolling
  scrollText("RFID Attendance", "Scan Your Card");
}

void loop() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  lcd.clear();

  // 🟢 SAURABH
  if (uid == saurabhUID) {

    saurabhIn = !saurabhIn;

    if (saurabhIn) {
      Serial.println("Saurabh Sharma,202410101150013,Entered");
      lcd.print("Saurabh IN");
    } else {
      Serial.println("Saurabh Sharma,202410101150013,Exited");
      lcd.print("Saurabh OUT");
    }

    lcd.setCursor(0,1);
    lcd.print("202410101150013");

    digitalWrite(GREEN_LED, HIGH);
    doubleBeep();   // 🔥 Double beep
  }

  // 🟢 DEV
  else if (uid == devUID) {

    devIn = !devIn;

    if (devIn) {
      Serial.println("Dev Verma,202410101150017,Entered");
      lcd.print("Dev IN");
    } else {
      Serial.println("Dev Verma,202410101150017,Exited");
      lcd.print("Dev OUT");
    }

    lcd.setCursor(0,1);
    lcd.print("202410101150017");

    digitalWrite(GREEN_LED, HIGH);
    doubleBeep();   // 🔥 Double beep
  }

  // 🔴 UNKNOWN
  else {
    Serial.println("Unknown,000000000000000,Unknown");

    lcd.print("Unknown Card");

    digitalWrite(RED_LED, HIGH);
    doubleBeep();   // 🔥 Double beep
  }

  delay(4000);

  // Back to scrolling
  scrollText("RFID Attendance", "Scan Your Card");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}