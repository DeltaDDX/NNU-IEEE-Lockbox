// ====================== LCD CONFIG ===============================

void lcd_setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

// ====================== SERVO CONFIG =============================

void servo_setup() {
  lockServo.attach(SERVO_PIN);
  lockServo.write(SERVO_LOCK_ANGLE);
}

// ====================== RFID CONFIG ==============================

void rfid_setup() {
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  lcd_print("Scan PICC to see UID");
}

void error() {

  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, 50 * i, 500);
    delay(500);
  }

  noTone(BUZZER_PIN);
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

// ========================= Functions =========================

// Compare RFID -> unlock box
string RFID_string() {
  String uidString;
  
  for (byte i = 0; i < 4; i++) {
    uidString += String(mfrc522.uid.uidByte[i]);
  }
  return uidString;
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
    lcd.setCursor(0, 0);
    lcd.print(output);
  }
}

// Unlock box
void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  lcd_print("Unlocked");
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Locked");
}

void error() {
  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, 50 * i, 500);
    delay(500);
  }
  noTone(BUZZER_PIN);
}
