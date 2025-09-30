#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

const uint8_t SERVO_PIN = 2;
const int SERVO_LOCK_ANGLE = 0;
const int SERVO_UNLOCK_ANGLE = 90;
const unsigned long HOLD_MS = 2000;  // time to hold each position
const uint8_t RST_PIN = 9;       // 18
const uint8_t SS_PIN = 10;        // 19
const uint8_t buzzer = 6;
const uint8_t red_led = 4;
const uint8_t green_led = 5;

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lockServo;

void error() {
  int alarm[4] = { 75, 50, 100, 150 };
  
    for (int i = 0; i < 4; i++) {
      tone(buzzer, alarm[i], 500);
      delay(500);
    }
  
  noTone(buzzer);
}

// ====================== SERVO TEST CONFIG =============================

void servo_setup() {
  lockServo.attach(SERVO_PIN);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Test ready. Cycling LOCK/UNLOCK...");
}

void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  lcd_print("Unlocked");
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Locked");
}

// ====================== LCD TEST CONFIG ===============================

void lcd_setup() {
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void lcd_print(String output) {
   lcd.clear();
   lcd.setCursor(0, 0);

   if (output.length() >= 16) {
    lcd.print(output.substring(0,16));
    lcd.setCursor(0, 1);
    lcd.print(output.substring(16, output.length()));
  } else {
    lcd.setCursor(0, 0); lcd.print(output);
  }
}

// =====================================================================


// ====================== RFID TEST CONFIG ==============================

// Init array that will store new NUID
byte nuidPICC[4];

void rfid_setup() {
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	lcd_print("Scan PICC to see UID");
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  if (mfrc522.uid.uidByte[0] != nuidPICC[0] ||
    mfrc522.uid.uidByte[1] != nuidPICC[1] ||
    mfrc522.uid.uidByte[2] != nuidPICC[2] ||
    mfrc522.uid.uidByte[3] != nuidPICC[3] ) {

    // Store NUID into nuidPICC array and string
    String uidString = "";
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = mfrc522.uid.uidByte[i];
      uidString += String(mfrc522.uid.uidByte[i]);
    }

    lcd_print(uidString);
  } else {
    lcd_print("Card read previously");
    servo_unlock();
  }

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  
}

// ====================== ARDUINO ENTRY POINTS ==========================

void setup() {
  lcd_setup();
  servo_setup();
  rfid_setup();
}
