short int slider(short int dirc) {
  unsigned int reading = 0;
  reading = (dirc) ? analogRead(CHANGE) : analogRead(SLIDE);
  short int result = 0;
  if (abs(CurrentTime - ButtonPreStart[dirc]) > BouncingTime) {
    if (reading < 1000) result = -1;
    else if (reading > 3000) result = 1;
    else result = 0;
    ButtonPreStart[dirc] = CurrentTime;
  }
  return result;
}

bool select_opt() {
  bool result = false;
  if (abs(CurrentTime - ButtonPreStart_sel) > BouncingTime) {
    result = digitalRead(SELECT) ? false : true;
    ButtonPreStart_sel = CurrentTime;
  }
  return result;
}


// void power_measurements() {

// InputCurrent = CurMeaOffset - analogRead(CURRENT_IN);}


void complete_measurements() {
  int avgCount = 150, totalCountCin = 0, totalCountCout = 0, totalCountVin = 0, totalCountVout = 0;
  float avgCountCin = 0, avgCountCout = 0, avgCountVin = 0, avgCountVout = 0;
  if (CurrentTime - RoutineStartTime > RoutineMidInterval) {
    for (int i = 0; i < avgCount; i++) {
      totalCountCin += analogRead(CURRENT_IN);
      totalCountCout += analogRead(CURRENT_OUT);
      totalCountVin += analogRead(VOLTAGE_IN);
      totalCountVout += analogRead(VOLTAGE_OUT);
    }
    RoutineStartTime = CurrentTime;
    avgCountCin = (float)totalCountCin / avgCount;
    avgCountCout = (float)totalCountCout / avgCount;
    avgCountVin = (float)totalCountVin / avgCount;
    avgCountVout = (float)totalCountVout / avgCount;

    InputCurrent = (CinOffsetVoltage - avgCountCin * 3.3 / 4096) * CinSensitivity;
    OutputCurrent = (CoutOffsetVoltage - avgCountCout * 3.3 / 4096) * CoutSensitivity;
    InputVoltage = avgCountVin * 3.3 * VinGain / 4096;
    BatteryVoltage = avgCountVout * 3.3 * VoutGain / 4096;
    Serial.print("Input Current : "), Serial.print(InputCurrent), Serial.print("    OutputCurrent: "), Serial.print(OutputCurrent), Serial.print("\n Input Voltage :"), Serial.print(InputVoltage), Serial.print("   BatteryVoltage :"), Serial.print(BatteryVoltage), Serial.print("\n");
  }
  return;
}