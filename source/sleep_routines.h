void wakeUp();

void goToSleep() {
#ifdef SERIAL_DEBUG
  Serial.println("Going to sleep...");
#endif

  sleep_enable();
  noInterrupts();
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), wakeUp, FALLING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(2000); // Do we need this?
  interrupts();
  sleep_cpu(); // stays on this line until interrupted ^
  
#ifdef SERIAL_DEBUG
  Serial.println("just woke up!");
#endif
}

void wakeUp() {
  noInterrupts();
  detachInterrupt(digitalPinToInterrupt(IRQ_PIN));
  interrupts();

#ifdef SERIAL_DEBUG
  Serial.println("Interrupt Fired");
#endif

  sleep_disable();
}
