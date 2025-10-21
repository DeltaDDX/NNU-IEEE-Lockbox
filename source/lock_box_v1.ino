#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Install "LiquidCrystal I2C" by Frank de Brabander
#include <SPI.h>
#include <MFRC522.h>           // Install "MFRC522" by GithubCommunity
#include <Adafruit_NeoPixel.h> // Install "Adafruit Neopixel" by Adafruit

#include "low-level-functions.h"  // Helper functions in current directory

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

// ====================== MAIN LOOP ==============================

void loop() {
  
  // Reset loop if no card or cannot read card
  is_AvailableCard();

  // Print RFID code to screen and unlock box
  lcd_print(RFID_string);
  servo_unlock();

  // Halt PICC and stop encryption on RFID
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// ====================== ARDUINO SETUP ==========================

void setup() {
  //led_setup();
  lcd_setup();
  servo_setup();
  rfid_setup();
}
