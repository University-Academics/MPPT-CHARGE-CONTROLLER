// CALIBRATION OF MAXIMUM AND MINIMUM VOLTAGES USING ENTERED VALUES
void battery_calib_limits() {
  float temVol = 0;
  int temPer = 0;
  for (int i = 0; i++; i < 5) {
    temVol = BatteryLevelMatrix[i][0], temPer = BatteryLevelMatrix[i][1];
    if (temVol == -1) break;
    else if (temPer == 0) BatteryMinVoltage = temVol;
    else if (temPer == 100) BatteryMaxVoltage = temVol;
  }
  return;
}

// FINDING BATTERY PERCENTAGE USING AVAILABLE VALUES
void find_battery_level() {
  float temVol = 0, supVol = BatteryMaxVoltage, infVol = BatteryMinVoltage;
  int temPer = 0, supPer = 100, infPer = 0;
  for (int i = 0; i < 5; i++) {
    temVol = BatteryLevelMatrix[i][0], temPer = BatteryLevelMatrix[i][1];
    if (temVol == -1) break;
    if (temVol < BatteryVoltage && infVol <= temVol)
      infVol = temVol, infPer = max(temPer, infPer);
    if (temVol > BatteryVoltage && supVol >= temVol)
      supVol = temVol, supPer = min(temPer, supPer);
  }

  temVol = constrain(BatteryVoltage, BatteryMinVoltage, BatteryMaxVoltage);
  BatteryLevel = constrain(infPer + ((float)(supPer - infPer)) * (BatteryVoltage - infVol) / (supVol - infVol),0,100) ;
  return;
}

// ADDING A VOLTAGE AND PERCENTAGE VALUE INTO CALIBRATION MATRIX
bool add_vol_per(float vol, int per) {
  if (calibParamCount == 6) {
    lcd.clear();
    lcd.setCursor(2, 1), lcd.print("MAXIMUM COUNT");
    lcd.setCursor(4, 2), lcd.print("REACHED.");
    delay(FreezeTime);
    lcd.clear();
    return false;
  }
  for (int i = 0; i < 5; i++) {
    if (BatteryLevelMatrix[i][0] == vol) {
      lcd.setCursor(2, 1), lcd.print("INVALID INPUT");
      lcd.setCursor(1, 2), lcd.print("PLZ ENTER VALID ONE");
      return false;
    } else if (BatteryLevelMatrix[i][0] == -1) break;
  }
  calibParamCount = calibParamCount + 1;
  BatteryLevelMatrix[calibParamCount - 1][0] = vol, BatteryLevelMatrix[calibParamCount - 1][1] = per;
  battery_calib_limits();
  return true;
}

void reset_temp_battery() {
  TempBatteryVoltage = 12, TempBatteryLevel = 0;
  return;
}

void show_battery_character() {
  int rangeMin = 0, rangeMax = min(4, (int)calibParamCount);
  lcd.clear();
  while (!wake_opt()) {
    CurrentTime = millis();
    for (int i = rangeMin; i < rangeMax; i++) {
      TempBatteryVoltage = BatteryLevelMatrix[i][0], TempBatteryLevel = BatteryLevelMatrix[i][1];
      lcd.setCursor(1, i), lcd.print(i), lcd.print(". ");
      (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage, 2);
      lcd.print("V   ");
      (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
      lcd.print("%");
    }
    switch (slider(SLIDER_Y)) {
      case 1:
        if (rangeMin > 0) rangeMin = rangeMin - 1, rangeMax = rangeMax - 1;
        break;
      case -1:
        if (calibParamCount > 4 && rangeMax < calibParamCount - 1) rangeMax = rangeMax + 1, rangeMin = rangeMin + 1;
        break;
    }
  }
}