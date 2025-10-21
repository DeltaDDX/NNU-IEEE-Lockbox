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

// Init array that will store new NUID
byte nuidPICC[4];

void rfid_setup() {
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  lcd_print("Scan PICC to see UID");
}

// ========================= Functions =========================

// Compare RFID -> unlock box
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


// future idea
/*
struct uid {
  byte uidBytes[4];
};

bool compare_uid(uid uid1, uid uid2) {
  for (byte i = 0; i < len(uid1.uidBytes); ++i) {
    if (uid1.uidBytes[i] != uid2.uidBytes[i]) {
      return 0;
    }
  }

  return 1;
}

uid make_uid() {
  
}
*/
