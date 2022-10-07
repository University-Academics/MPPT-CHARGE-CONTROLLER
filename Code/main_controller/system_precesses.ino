///////////////////////////////// FLASHING ORDER ////////////////////////////////////////
/*
VALUES TO SAVE AND RELOAD ON REBOOT
------------------------------------------
1. CONTROLLER MODE = MANUAL / AUTOMATIC
2. BATTERY CALIBRATION MATRIX[0][0] WHOLE
3. BATTERY CALIBRATION MATRIX[0][0] DECIMAL
4. BATTERY CALIBRATION MATRIX[0][1]
5. BATTERY CALIBRATION MATRIX[1][0] WHOLE
6. BATTERY CALIBRATION MATRIX[1][0] DECIMAL
7. BATTERY CALIBRATION MATRIX[1][1]
8. BATTERY CALIBRATION MATRIX[2][0] WHOLE
9. BATTERY CALIBRATION MATRIX[2][0] DECIMAL
10. BATTERY CALIBRATION MATRIX[2][1]
11. BATTERY CALIBRATION MATRIX[3][0] WHOLE
12. BATTERY CALIBRATION MATRIX[3][0] DECIMAL
13. BATTERY CALIBRATION MATRIX[3][1]
14. BATTERY CALIBRATION MATRIX[4][0] WHOLE
15. BATTERY CALIBRATION MATRIX[4][0] DECIMAL
16. BATTERY CALIBRATION MATRIX[4][1]
17. BATTERY CALIBRATION MATRIX[5][0] WHOLE
18. BATTERY CALIBRATION MATRIX[5][0] DECIMAL
19. BATTERY CALIBRATION MATRIX[5][1]
20. CALIBRATION PARAMETER COUNT
21. BACK LIGHT SLEEP MODE
22. BRIGHTNESS LEVEL
*/

int decimal_extractor(float value) {
  int result = 0;
  result = (value < 0) ? ((int)(value * (-100))) % 100 : ((int)(value * 100)) % 100;
  return result;
}


// ON THE EVENT OF FACTORY RESET
void factor_reset() {
  EEPROM.write(0, AUTO);

  EEPROM.write(1, (int8_t)-1);
  EEPROM.write(2, decimal_extractor(-1);
  EEPROM.write(3, (int8_t)-1);
  EEPROM.write(4,  (int8_t)-1);
  EEPROM.write(5, decimal_extractor(-1);
  EEPROM.write(6,  (int8_t)-1);
  EEPROM.write(7,  (int8_t)-1);
  EEPROM.write(8, decimal_extractor(-1);
  EEPROM.write(9,  (int8_t)-1);
  EEPROM.write(10,  (int8_t)-1);
  EEPROM.write(11, decimal_extractor(-1);
  EEPROM.write(12,  (int8_t)-1);
  EEPROM.write(13,  (int8_t)-1);
  EEPROM.write(14, decimal_extractor(-1);
  EEPROM.write(15,  (int8_t)-1);
  EEPROM.write(16,  (int8_t)-1);
  EEPROM.write(17, decimal_extractor(-1);
  EEPROM.write(18,  (int8_t)-1);

  EEPROM.write(19, 0);
  EEPROM.write(20, NEVER);
  EEPROM.write(21, 40);
}


//LOAD CURRENT VALUES
void load_settings() {
  int8_t temp = 0;
  ControllerMode = EEPROM.read(0);

  temp = int8_t(EEPROM.read(1));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(2) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(3));
  temp = int8_t(EEPROM.read(4));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(5) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(6));
  temp = int8_t(EEPROM.read(7));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(8) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(9));
  temp = int8_t(EEPROM.read(10));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(11) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(12));
  temp = int8_t(EEPROM.read(13));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(14) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(15));
  temp = int8_t(EEPROM.read(16));
  BatteryLevelMatrix[0][0] = temp + EEPROM.read(17) * 0.01;
  BatteryLevelMatrix[0][1] = int8_t(EEPROM.read(18));

  CalibParamCount = EEPROM.read(19);
  BackLightSleepMode = EEPROM.read(20);
  BrightnessLevel = EEPROM.read(21);
  return;
}

// SAVE CURRENT PARAMS INTO ROM
void save_settings() {
  EEPROM.write(0, ControllerMode);

  EEPROM.write(1, (int8_t)BatteryLevelMatrix[0][0]);
  EEPROM.write(2, decimal_extractor(BatteryLevelMatrix[0][0]));
  EEPROM.write(3, (int8_t)BatteryLevelMatrix[0][1]);
  EEPROM.write(4, (int8_t)BatteryLevelMatrix[1][0]);
  EEPROM.write(5, decimal_extractor(BatteryLevelMatrix[1][0]));
  EEPROM.write(6, (int8_t)BatteryLevelMatrix[1][1]);
  EEPROM.write(7, (int8_t)BatteryLevelMatrix[2][0]);
  EEPROM.write(8, decimal_extractor(BatteryLevelMatrix[2][0]));
  EEPROM.write(9, (int8_t)BatteryLevelMatrix[2][1]);
  EEPROM.write(10, (int8_t)BatteryLevelMatrix[3][0]);
  EEPROM.write(11, decimal_extractor(BatteryLevelMatrix[3][0]));
  EEPROM.write(12, (int8_t)BatteryLevelMatrix[3][1]);
  EEPROM.write(13, (int8_t)BatteryLevelMatrix[4][0]);
  EEPROM.write(14, decimal_extractor(BatteryLevelMatrix[4][0]));
  EEPROM.write(15, (int8_t)BatteryLevelMatrix[4][1]);
  EEPROM.write(16, (int8_t)BatteryLevelMatrix[5][0]);
  EEPROM.write(17, decimal_extractor(BatteryLevelMatrix[5][0]));
  EEPROM.write(18, (int8_t)BatteryLevelMatrix[5][1]);

  EEPROM.write(19, calibParamCount);
  EEPROM.write(20, BackLightSleepMode);
  EEPROM.write(21, BrightnessLevel);
}