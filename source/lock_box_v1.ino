#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Install "LiquidCrystal I2C" by Frank de Brabander
#include <SPI.h>
#include <MFRC522.h>           // Install "MFRC522" by GithubCommunity
#include <Adafruit_NeoPixel.h> // Install "Adafruit Neopixel" by Adafruit

//#define SERIAL_DEBUG

const uint8_t SERVO_PIN = 7; //2;
const int SERVO_LOCK_ANGLE = 15;
const int SERVO_UNLOCK_ANGLE = 85;
const unsigned long HOLD_MS = 2000;  // time to hold each position
const uint8_t RST_PIN = 9;
const uint8_t SS_PIN = 10;
const uint8_t BUZZER_PIN = 6;
const uint8_t ERROR_PIN = 4; // Red LED PIN
const uint8_t VALID_PIN = 3; //5; // Green LED PIN
const uint8_t LOW_BATTERY_PIN = 5; //3; // Yellow LED PIN
//const uint8_t LED_DATA_PIN = 8;
//const uint8_t LED_COUNT = 23;


MFRC522 mfrc522(SS_PIN, RST_PIN); // Instantiate RFID Object w/ slave select and reset pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // Instantiate LCD Screen Object
Servo lockServo; // Instantiate Servo Object
//Adafruit_NeoPixel strip(LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800); //Instantiate LED Strip Object

#include "helper_functions.h"  // Helper functions in current directory

// Buzzer for error
void error() {

  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, 50 * i, 500);
    delay(500);
  }

  noTone(BUZZER_PIN);
}

// ====================== SERVO CONFIG =============================

void servo_setup() {
  lockServo.attach(SERVO_PIN);
  lockServo.write(SERVO_LOCK_ANGLE);
}

// ====================== LCD CONFIG ===============================

void lcd_setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

// ====================== LED STRIP CONFIG ========================
/*
  void led_setup() {
  strip.begin(); // Initialize the strip
  strip.show();  // Turn off all LEDs at start
  strip.setBrightness(100); // 0-255 brightness scale
  }

  // Rainbow Cycle
  void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  }

  // Helper function to map values to colors
  uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
*/

// ====================== RFID CONFIG ==============================

// Init array that will store new NUID
byte nuidPICC[4];

void rfid_setup() {
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  lcd_print("Scan PICC to see UID");
}

// ====================== MAIN LOOP ==============================

void checkRFID() {
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

#ifdef SERIAL_DEBUG
    Serial.print(uidString);
#endif

  } 

    servo_unlock();
    /*
    rainbowCycle(10);
    strip.clear();
    strip.show();
    */
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

  checkRFID();

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on RFID
  mfrc522.PCD_StopCrypto1();
}

// ====================== ARDUINO ENTRY POINTS ==========================

void setup() {
  
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif

  //led_setup();
  lcd_setup();
  servo_setup();
  rfid_setup();
}
