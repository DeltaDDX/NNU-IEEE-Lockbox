#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Install "LiquidCrystal I2C" by Frank de Brabander
#include <SPI.h>
#include <MFRC522.h> // Install "MFRC522" by GithubCommunity
#include <Adafruit_NeoPixel.h> // Install "Adafruit Neopixel" by Adafruit

const uint8_t SERVO_PIN = 2;
const int SERVO_LOCK_ANGLE = 0;
const int SERVO_UNLOCK_ANGLE = 90;
const unsigned long HOLD_MS = 2000;  // time to hold each position
const uint8_t RST_PIN = 9;
const uint8_t SS_PIN = 10;
const uint8_t BUZZER_PIN = 6;
const uint8_t ERROR_PIN = 4; // Red LED PIN
const uint8_t VALID_PIN = 5; // Green LED PIN
const uint8_t LOW_BATTERY_PIN = 3; // Yellow LED PIN
const uint8_t LED_TOGGLE_PIN = 7; // Toggles power for led strip
const uint8_t LED_DATA_PIN = 8;
const uint8_t LED_COUNT = 23;


MFRC522 mfrc522(SS_PIN, RST_PIN); // Instantiate RFID Object w/ slave select and reset pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // Instantiate LCD Screen Object
Servo lockServo; // Instantiate Servo Object
Adafruit_NeoPixel strip(LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800); //Instantiate LED Strip Object

// Buzzer for error
void error() {
  
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, 50 * i, 500);
      delay(500);
    }
  
  noTone(buzzer);
}

// ====================== SERVO CONFIG =============================

void servo_setup() {
  lockServo.attach(SERVO_PIN);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Test ready. Cycling LOCK/UNLOCK...");
}

// Unlock box
void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  lcd_print("Unlocked");
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Locked");
}

// ====================== LCD CONFIG ===============================

void lcd_setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

// Print to lcd screen
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

// ====================== LED STRIP CONFIG ========================

void led_setup() {
  strip.begin(); // Initialize the strip
  strip.show();	// Turn off all LEDs at start
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


// ====================== RFID CONFIG ==============================

// Init array that will store new NUID
byte nuidPICC[4];

void rfid_setup() {
	SPI.begin(); // Initialize SPI bus
	mfrc522.PCD_Init(); // Initialize MFRC522
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
    	servo_unlock();
    	rainbowCycle(10);
    	strip.clear();
    	strip.show();
  	}

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  
}

// ====================== ARDUINO ENTRY POINTS ==========================

void setup() {
  pinMode(LED_TOGGLE_PIN, OUTPUT);
  led_setup();
  lcd_setup();
  servo_setup();
  rfid_setup();
}
