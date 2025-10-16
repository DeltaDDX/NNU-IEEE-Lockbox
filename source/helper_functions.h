// Print to lcd screen
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

// Unlock box
void servo_unlock() {
  lockServo.write(SERVO_UNLOCK_ANGLE);
  lcd_print("Unlocked");
  delay(HOLD_MS);
  lockServo.write(SERVO_LOCK_ANGLE);
  lcd_print("Locked");
}

void awaitNewCard() { // poll the reader until new card appears, 
  while (1) {
    if (!mfrc522.PICC_IsNewCardPresent()) {
      continue;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
      continue;
    }
    else {
      return;
    }
  }
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
