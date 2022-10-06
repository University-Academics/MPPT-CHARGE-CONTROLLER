void wake_back_light() {
  lcd.setBacklight(HIGH);
  PrevLcdBackMillis = millis();
}

void lcd_back_light() {
  unsigned long backLightInterval = 10000;

  switch (BackLightSleepMode) {
    case 0:
      PrevLcdBackMillis = millis();
      break;
    case 1:
      backLightInterval = 10000;  // 10 seconds
      break;
    case 2:
      backLightInterval = 300000;  // 5 minutes
      break;
    case 3:
      backLightInterval = 3600000;  // 1 hour
      break;
    case 4:
      backLightInterval = 21600000;  // 6 hours
      break;
    case 5:
      backLightInterval = 43200000;  // 12 hours
      break;
    case 6:
      backLightInterval = 86400000;  // 1 days
      break;
    case 7:
      backLightInterval = 259200000;  // 3 days
      break;
    case 8:
      backLightInterval = 604800000;  // 1 week
      break;
    case 9:
      backLightInterval = 2419200000;  // 1 month
      break;
  }

  CurLcdBackMillis = millis();
  if (CurLcdBackMillis - PrevLcdBackMillis >= backLightInterval) {
    PrevLcdBackMillis = CurLcdBackMillis;  //Store previous time
    lcd.setBacklight(LOW);
  }
}

void saved_config(short int STATE) {
  switch (STATE) {
    case 0:
      lcd.clear();
      lcd.setCursor(3, 1), lcd.print("SETTING UPDATED");
      lcd.setCursor(5, 2), lcd.print("SUCESSFULLY");
      delay(FreezeTime);
      lcd.clear();
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(2, 1), lcd.print("WANT TO PROCEED");
      lcd.setCursor(2, 2), lcd.print("WITHOUT SAVING?");
      while (true) {
        CurrentTime = millis();
        lcd.setCursor(1, 3), lcd.print("YES");
        lcd.setCursor(15, 3), lcd.print("NO");
        break;
      }
      lcd.clear();
      break;
    case 2:
      break;
  }
}

void set_brightness(short int brightValue) {
  ledcWrite(LED_CHANNEL_BRIG, (int)(BrightnessLevel * 255 / 100));
  return;
}

void brightness_adj_config() {
  switch (slider(SLIDER_Y)) {
    case (-1): BrightnessLevel = constrain(BrightnessLevel - 5, 0, 100); break;
    case (1): BrightnessLevel = constrain(BrightnessLevel + 5, 0, 100); break;
  }
  set_brightness(BrightnessLevel);
  lcd.setCursor(1, 1), lcd.print("BRIGHTNESS LEVEL :");
  if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
    BlinkStartTime = CurrentTime;
    lcd.setCursor(5, 2);
    if (BlinkState) {
      BlinkState = false;
      (BrightnessLevel < 10) ? lcd.print("  "), lcd.print(BrightnessLevel) : ((BrightnessLevel == 0) ? lcd.print(BrightnessLevel) : lcd.print(" "), lcd.print(BrightnessLevel));
      lcd.print("%");
    } else {
      BlinkState = true;
      lcd.print("                ");
    }
  }
}

void main_display_config() {
  // LINE : 1
  lcd.setCursor(1, 0);
  lcd.write(ChargingMode);
  lcd.setCursor(4, 0);
  lcd.print("EFF:");
  (Efficiency < 10) ? lcd.print(" "), lcd.print(Efficiency) : lcd.print(Efficiency);
  lcd.print("%");
  lcd.setCursor(13, 0);
  (ControllerMode == MANU) ? lcd.print("MANU ") : lcd.print("AUTO ");
  lcd.setCursor(18, 0);
  (InputVoltage > 12) ? lcd.write(4) : lcd.write(5);

  // LINE : 2
  lcd.setCursor(0, 1);
  lcd.print("BAT LEVEL: ");
  if (calibParamCount < 2) lcd.print("ERROR");
  else {
    (BatteryLevel < 10) ? lcd.print("  "), lcd.print(BatteryLevel) : ((BatteryLevel == 100) ? lcd.print(BatteryLevel) : lcd.print(" "), lcd.print(BatteryLevel));
    lcd.print("%");
  }

  // LINE : 3
  lcd.setCursor(0, 2);
  lcd.print("   ENERGY: ");
  if (Wh < 10) lcd.print(Wh, 3), lcd.print("Wh ");                  //9.999Wh_
  else if (Wh < 100) lcd.print(Wh, 2), lcd.print("Wh ");            //99.99Wh_
  else if (Wh < 1000) lcd.print(Wh, 1), lcd.print("Wh ");           //999.9Wh_
  else if (Wh < 10000) lcd.print(kWh, 2), lcd.print("kWh ");        //9.99kWh_
  else if (Wh < 100000) lcd.print(kWh, 1), lcd.print("kWh ");       //99.9kWh_
  else if (Wh < 1000000) lcd.print(kWh, 0), lcd.print("kWh  ");     //999kWh__
  else if (Wh < 10000000) lcd.print(MWh, 2), lcd.print("MWh ");     //9.99MWh_
  else if (Wh < 100000000) lcd.print(MWh, 1), lcd.print("MWh ");    //99.9MWh_
  else if (Wh < 1000000000) lcd.print(MWh, 0), lcd.print("MWh  ");  //999MWh__

  // LINE : 4
  lcd.setCursor(0, 3);
  lcd.print("POW INPUT: ");
  lcd.print(InputPower, 2);
  return;
}

void device_mode_config() {
  const short int nMax = 1, nMin = 0;
  lcd.setCursor(1, 0), lcd.print("MODE OF OPERATION");

  switch (slider(SLIDER_Y)) {
    case (-1): TempControllerMode = constrain(TempControllerMode - 1, nMin, nMax); break;
    case (1): TempControllerMode = constrain(TempControllerMode + 1, nMin, nMax); break;
  }

  if (TempControllerMode == AUTO) {
    lcd.setCursor(1, 2), lcd.print("1. MANUAL");
    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      BlinkStartTime = CurrentTime;
      if (BlinkState) {
        BlinkState = false;
        lcd.setCursor(1, 3), lcd.print("2. AUTOMATIC");
      } else {
        BlinkState = true;
        lcd.setCursor(1, 3), lcd.print("             ");
      }
    }
  } else {
    lcd.setCursor(1, 3), lcd.print("2. AUTOMATIC");
    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      BlinkStartTime = CurrentTime;
      if (BlinkState) {
        BlinkState = false;
        lcd.setCursor(1, 2), lcd.print("1. MANUAL");
      } else {
        BlinkState = true;
        lcd.setCursor(1, 2), lcd.print("             ");
      }
    }
  }

  if (select_opt()) {
    Serial.println("WORKS");
    ControllerMode = TempControllerMode;
    saved_config(0);
  }

  return;
}

void battery_calib_config() {
  lcd.setCursor(1, 0), lcd.print("BATTERY CALIBRATION");
  lcd.setCursor(1, 1), lcd.print("   VOLTAGE: ");
  lcd.setCursor(1, 2), lcd.print("PERCENTAGE: ");
  // VOLTAGE CALIBRATION
  if (TempBatteryCalib == 0) {
    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      lcd.setCursor(13, 1);
      BlinkStartTime = CurrentTime;
      if (BlinkState) {
        (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage, 2);
        lcd.print("V");
        BlinkState = false;
      } else {
        BlinkState = true;
        lcd.print("        ");
      }
    }
    lcd.setCursor(13, 2);
    (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
    lcd.print("%");
    lcd.setCursor(1, 3), lcd.print("SAVE");
    lcd.setCursor(13, 3), lcd.print("SHOW");
    if (select_opt()) {
      delay(2 * BouncingTime);
      while (!select_opt()) {
        CurrentTime = millis();
        switch (slider(SLIDER_Y)) {
          case 1: TempBatteryVoltage = constrain(TempBatteryVoltage + 0.01, BatteryMinVoltage, BatteryMaxVoltage); break;
          case -1: TempBatteryVoltage = constrain(TempBatteryVoltage - 0.01, BatteryMinVoltage, BatteryMaxVoltage); break;
        }
        if (abs(CurrentTime - BlinkStartTime) > .6 * BlinkTime) {
          lcd.setCursor(13, 1);
          BlinkStartTime = CurrentTime;
          if (BlinkState) {
            (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage, 2);
            lcd.print("V");
            BlinkState = false;
          } else {
            BlinkState = true;
            lcd.print("          ");
          }
        }
      }
    } else if (slider(SLIDER_Y) == -1) {
      TempBatteryCalib = 1;
      delay(BouncingTime);
    }
  }


  // BATTERY PERCENTAGE CALIBRATION
  else if (TempBatteryCalib == 1) {
    lcd.setCursor(13, 1);
    (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage);
    lcd.print("V");
    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      lcd.setCursor(13, 2);
      BlinkStartTime = CurrentTime;
      if (BlinkState) {
        BlinkState = false;
        (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
        lcd.print("%");
      } else {
        BlinkState = true;
        lcd.print("                ");
      }
    }
    lcd.setCursor(1, 3), lcd.print("SAVE");
    lcd.setCursor(13, 3), lcd.print("SHOW");
    if (select_opt()) {
      delay(2 * BouncingTime);
      while (!select_opt()) {
        CurrentTime = millis();
        switch (slider(SLIDER_Y)) {
          case 1: TempBatteryLevel = constrain(TempBatteryLevel + 1, 0, 100); break;
          case -1: TempBatteryLevel = constrain(TempBatteryLevel - 1, 0, 100); break;
        }
        if (abs(CurrentTime - BlinkStartTime) > .6 * BlinkTime) {
          lcd.setCursor(13, 2);
          BlinkStartTime = CurrentTime;
          if (BlinkState) {
            BlinkState = false;
            (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
            lcd.print("%");
          } else {
            BlinkState = true;
            lcd.print("                ");
          }
        }
      }
    } else if (slider(SLIDER_Y) == -1) {
      TempBatteryCalib = 2;
      delay(BouncingTime);
    } else if (slider(SLIDER_Y) == 1) {
      TempBatteryCalib = 0;
      delay(BouncingTime);
    }
  }


  // Save option
  else if (TempBatteryCalib == 2) {
    lcd.setCursor(13, 1);
    (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage);
    lcd.print("V");
    lcd.setCursor(13, 2);
    (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
    lcd.print("%");

    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      BlinkStartTime = CurrentTime;
      lcd.setCursor(1, 3);
      if (BlinkState) {
        BlinkState = false;
        lcd.print("SAVE");
      } else {
        BlinkState = true;
        lcd.print("         ");
      }
    }
    if (select_opt() && add_vol_per(TempBatteryVoltage, TempBatteryLevel)) saved_config(0);
    else if (slider(SLIDER_Y) == 1) {
      TempBatteryCalib = 1;
      delay(BouncingTime);
    } else if (slider(SLIDER_Y) == -1) {
      TempBatteryCalib = 3;
      delay(BouncingTime);
    }
    lcd.setCursor(13, 3), lcd.print("SHOW");
  }

  // SHOW OPTION
  else if (TempBatteryCalib == 3) {

    lcd.setCursor(13, 1);
    (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage);
    lcd.print("V");
    lcd.setCursor(13, 2);
    (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
    lcd.print("%");
    lcd.setCursor(1, 3), lcd.print("SAVE");

    if (abs(CurrentTime - BlinkStartTime) > BlinkTime) {
      BlinkStartTime = CurrentTime;
      lcd.setCursor(13, 3);
      if (BlinkState) {
        BlinkState = false;
        lcd.print("SHOW");
      } else {
        BlinkState = true;
        lcd.print("         ");
      }
    }
    if (select_opt()) {
      delay(2 * BouncingTime);
      show_battery_character();
    } else if (slider(SLIDER_Y) == 1) {
      TempBatteryCalib = 2;
      delay(BouncingTime);
    }
  }
  return;
}




void lcd_menu() {
  battery_calib_config();
  return;
}