void LCD_init() {


  //LCD INITIALIZATION
  if (EnableLCD == 1) {
    lcd.init();
    lcd.setBacklight(HIGH);
    set_brightness(BrightnessLevel);
    lcd.setCursor(0, 1);
    lcd.print("MPPT INITIALIZED");
    lcd.setCursor(0, 2);
    lcd.print("Version : 1.0 ");
    delay(1500);
    lcd.clear();
  }
  // Intiating characters
  lcd.createChar(0, NoBattery);
  lcd.createChar(1, Charging);
  lcd.createChar(2, Charged);
  lcd.createChar(3, IdLe);
  lcd.createChar(4, Check);
  lcd.createChar(5, Skull);

  return;
}

// DEALING WITH BACK TIME THE BACK LIGHT IS KEPT ON
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

  if (abs((int)(CurrentTime - PrevLcdBackMillis)) > backLightInterval) {
    PrevLcdBackMillis = CurrentTime;  //Store previous time
    lcd.setBacklight(LOW);
  }
  if (wake_opt()) {
    lcd.setBacklight(HIGH);
    PrevLcdBackMillis = CurrentTime;
  }
  return;
}

// SUCESS/ UNSUCESSFUL MESSAGE ON SAVING STATES
bool saved_config(short int STATE) {
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
      lcd.setCursor(2, 0), lcd.print("WANT TO PROCEED");
      lcd.setCursor(2, 1), lcd.print("WITHOUT SAVING?");
      while (true) {
        CurrentTime = millis();
        lcd.setCursor(1, 2), lcd.print("YES : PRESS SELECT");
        if (select_opt()) {
          lcd.clear();
          return true;
        }
        lcd.setCursor(1, 3), lcd.print("NO : PRESS WAKE");
        if (wake_opt()) {
          lcd.clear();
          return false;
        }
      }

    case 2:
      lcd.clear();
      lcd.setCursor(3, 1), lcd.print("SYSTEM RESET");
      lcd.setCursor(5, 2), lcd.print("SUCESSFUL");
      delay(FreezeTime);
      lcd.clear();
      break;
  }
  lcd.clear();
  return true;
}

// DISPLAY CONFIGURATION
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
    (BatteryLevel < 10) ? lcd.print("  "), lcd.print(BatteryLevel) : ((BatteryLevel == 100) ? BatteryLevel-- : lcd.print(" "), lcd.print(BatteryLevel));
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

// SETTING SCREEN BRIGHTNESS
void set_brightness(short int brightValue) {
  ledcWrite(LED_CHANNEL_BRIG, (int)(BrightnessLevel * 255 / 100));
  return;
}

// ADJUSTMENT FUNCTION FOR ADJUSTING SCREEN BRIGHTNESS
void brig_adj_config() {
  switch (slider(SLIDER_Y)) {
    case (-1): BrightnessLevel = constrain(BrightnessLevel - 5, 0, 100); break;
    case (1): BrightnessLevel = constrain(BrightnessLevel + 5, 0, 100); break;
  }
  set_brightness(BrightnessLevel);
  lcd.setCursor(1, 1), lcd.print("BRIGHTNESS LEVEL :");
  if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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

// ADJUSTING DEVICE MODE
void device_mode_config() {
  const short int nMax = 1, nMin = 0;
  lcd.setCursor(1, 0), lcd.print("MODE OF OPERATION");

  switch (slider(SLIDER_Y)) {
    case (-1): TempControllerMode = constrain(TempControllerMode - 1, nMin, nMax); break;
    case (1): TempControllerMode = constrain(TempControllerMode + 1, nMin, nMax); break;
  }

  if (TempControllerMode == AUTO) {
    lcd.setCursor(1, 2), lcd.print("1. MANUAL");
    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
    ControllerMode = TempControllerMode;
    saved_config(0);
  }

  return;
}

// ADDING DATA TO THE BATTERY CALIBRATION MATRIX
void battery_calib_config() {
  lcd.setCursor(1, 0), lcd.print("BATTERY CALIBRATION");
  lcd.setCursor(1, 1), lcd.print("   VOLTAGE: ");
  lcd.setCursor(1, 2), lcd.print("PERCENTAGE: ");

  // VOLTAGE CALIBRATION
  if (TempBatteryCalib == 0) {
    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
    if (select_opt())
      while (!wake_opt()) {
        CurrentTime = millis();
        switch (slider(SLIDER_Y)) {
          case 1: TempBatteryVoltage = constrain(TempBatteryVoltage + 0.01, BatteryMinVoltage, BatteryMaxVoltage); break;
          case -1: TempBatteryVoltage = constrain(TempBatteryVoltage - 0.01, BatteryMinVoltage, BatteryMaxVoltage); break;
        }
        if (abs((int)(CurrentTime - BlinkStartTime)) > .6 * BlinkTime) {
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
  }


  // BATTERY PERCENTAGE CALIBRATION
  else if (TempBatteryCalib == 1) {
    lcd.setCursor(13, 1);
    (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage);
    lcd.print("V");
    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
    if (select_opt())
      while (!wake_opt()) {
        CurrentTime = millis();
        switch (slider(SLIDER_Y)) {
          case 1: TempBatteryLevel = constrain(TempBatteryLevel + 1, 0, 100); break;
          case -1: TempBatteryLevel = constrain(TempBatteryLevel - 1, 0, 100); break;
        }
        if (abs((int)(CurrentTime - BlinkStartTime)) > .6 * BlinkTime) {
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
  }


  // Save option
  else if (TempBatteryCalib == 2) {
    lcd.setCursor(13, 1);
    (TempBatteryVoltage < 10) ? lcd.print(" "), lcd.print(TempBatteryVoltage, 2) : lcd.print(TempBatteryVoltage);
    lcd.print("V");
    lcd.setCursor(13, 2);
    (TempBatteryLevel < 10) ? lcd.print("  "), lcd.print(TempBatteryLevel) : ((TempBatteryLevel == 100) ? lcd.print(TempBatteryLevel) : lcd.print(" "), lcd.print(TempBatteryLevel));
    lcd.print("%");

    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
    if (select_opt()) {
      delay(1.5 * BouncingTimeButt);
      if (select_opt()) {
        if (add_vol_per(TempBatteryVoltage, TempBatteryLevel))
          saved_config(0);
        reset_temp_battery();
      }
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

    if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
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
      show_battery_character();
    }
  }

  // switching between options
  switch (slider(SLIDER_Y)) {
    case 1:
      TempBatteryCalib = constrain(TempBatteryCalib - 1, 0, 3);
      lcd.clear();
      break;
    case -1:
      TempBatteryCalib = constrain(TempBatteryCalib + 1, 0, 3);
      lcd.clear();
      break;
  }
  return;
}

// ADJUSTING SLEEP TIME MODE
void sleep_time_config() {
  lcd.setCursor(1, 1), lcd.print("SELECT SLEEP MODE :");
  TempSleepMode = constrain(TempSleepMode, NEVER, MONTH_1);
  if (abs((int)(CurrentTime - BlinkStartTime)) > BlinkTime) {
    lcd.setCursor(2, 2);
    BlinkStartTime = CurrentTime;
    if (BlinkState) {
      switch (TempSleepMode) {
        case NEVER: lcd.print("NEVER"); break;
        case SEC_10: lcd.print("10 SECONDS"); break;
        case MIN_5: lcd.print("5 MINUTES"); break;
        case HOUR_1: lcd.print("1 HOUR"); break;
        case HOUR_6: lcd.print("6 HOURS"); break;
        case HOUR_12: lcd.print("12 HOURS"); break;
        case DAY_1: lcd.print("1 DAY"); break;
        case DAY_3: lcd.print("3 DAYS"); break;
        case WEEK_1: lcd.print("1 WEEK"); break;
        case MONTH_1: lcd.print("1 MONTH"); break;
      }
      BlinkState = false;
    } else {
      BlinkState = true;
      lcd.print("                 ");
    }
    if (select_opt()) {
      saved_config(0);
      BackLightSleepMode = TempSleepMode;
    }
    switch (slider(SLIDER_Y)) {
      case 1: TempSleepMode--; break;
      case -1: TempSleepMode++; break;
    }
  }

  return;
}

// FACTORY RESETTING
void factory_reset_config() {
  lcd.setCursor(1, 0), lcd.print("RESET THE DEVICE");
  lcd.setCursor(1, 2), lcd.print("PRESS WAKE IF YOU");
  lcd.setCursor(3, 3), lcd.print("TO PROCEED ?");
  if (wake_opt()) {
    saved_config(2);
    factory_reset();
  }
  return;
}

// RETURNING TO THE MAIN DISPLAY
void return_config() {
  lcd.setCursor(1, 1), lcd.print("EXIT MENU");
  lcd.setCursor(1, 2), lcd.print("PRESS SELECT IF YOU");
  lcd.setCursor(3, 3), lcd.print("WANT TO PROCEED ?");
  if (select_opt()) {
    lcd.clear();
    MenuMode = false;
  }
  return;
}

// DEALING WITH SWITCHING BETWEEN SUBMENUS
void change_submenu() {
  short int subMenuMax = RETURN;
  switch (slider(SLIDER_X)) {
    case 1:
      lcd.clear();
      if (SubMenu == BATTERY_CALIB && !saved_config(1) && TempBatteryVoltage != 12) break;  // DEALING WITH THE CASE OF SAVING WITHOUT PROCEED INCASE OF BATTERY CALIBRATION
      SubMenu = constrain(SubMenu - 1, 0, subMenuMax);
      break;
    case -1:
      lcd.clear();
      if (SubMenu == BATTERY_CALIB && !saved_config(1) && TempBatteryVoltage != 12) break;
      SubMenu = constrain(SubMenu + 1, 0, subMenuMax);
      break;
  }
  return;
}

// MAIN FUNCTION ON LCD DISPLAY
void LCD_MENU() {
  lcd_back_light();

  // Default Configuration
  if (!MenuMode) {
    // long press for entering menu
    if (select_opt()) {
      lcd.clear();
      MenuMode = true;
      return;
    }
    // updating display periodically
    if (abs((int)(CurrentTime - PrevDispUpdateTime)) > DispUpdateInterval) {
      lcd.clear();
      main_display_config();
      PrevDispUpdateTime = CurrentTime;
    }
    return;
  }

  // Entering Menu Mode
  switch (SubMenu) {
    case BRIG_ADJ:
      brig_adj_config();
      break;
    case DEVICE_MODE:
      device_mode_config();
      break;
    case BATTERY_CALIB:
      battery_calib_config();
      break;
    case SLEEP_TIME:
      sleep_time_config();
      break;
    case FACTORY_RESET:
      factory_reset_config();
    case RETURN:
      return_config();
      break;
  }

  change_submenu();

  return;
}