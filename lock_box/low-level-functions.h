// ========================= Functions =========================

// Compare RFID -> Unlock Box
String RFID_string() {
  String uidString;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      uidString += ":";
    }
  }
  return uidString;
}

// Print to LCD Screen
void lcd_print(String output) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (output.length() >= 16) {
    lcd.print(output.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(output.substring(16, output.length()));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(output);
  }
}

// Unlock Box
void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  lcd_print("Unlocked");
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Locked");
}

// Buzzer for Error
void error() {
  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, 50 * i, 500);
    delay(500);
  }
  noTone(BUZZER_PIN);
}

// Check if card
bool is_AvailableCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    return true;
  }
  return false;
}

void sleepLCD() {  // Clear the screen and turn off both display and backlight
  lcd.clear();  // Clear screen
  lcd.noDisplay();  // turn off text (reduces consumption)
  lcd.noBacklight();  // turn off backlight (biggest power saver)
  lcd_sleeping = true;
}

void wakeLCD() {
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd_print("LCD Awake");
  lcd_sleeping = false;
  delay(350); // tiny pause so user sees it
  lcd.clear();
}
