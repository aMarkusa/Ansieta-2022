// Vibration motor module.
// Used to notify the user.

unsigned long vibrateOnTime = 0;

void vibrateDevice() {
  // Start the vibration period.
  Serial.println("HAHAHAHAHHAHA");
  vibrateOnTime = millis();
  digitalWrite(VIBRATE_PIN, HIGH);

}

void handleVibrate() {
  // Stops the vibrate after the period has ended.
  if (vibrateOnTime && millis()-vibrateOnTime>VIBRATE_PERIOD) {
    digitalWrite(VIBRATE_PIN, LOW);
    vibrateOnTime = 0;
  }
}