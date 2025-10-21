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

