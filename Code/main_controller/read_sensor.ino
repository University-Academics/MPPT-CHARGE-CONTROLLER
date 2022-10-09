// INTIALIZATION OF PIN PARAMETERS
void PARAM_init() {
  // configure LED PWM functionalitites
  ledcSetup(LED_CHANNEL_BRIG, FREQ2, RESOLU2);
  ledcAttachPin(BRIGHTNESS_CONTROLLER, LED_CHANNEL_BRIG);
  ledcSetup(BUCK_PWM_CHANNEL, FREQ1, RESOLU1);
  ledcAttachPin(BUCK_IN, BUCK_PWM_CHANNEL);

  // PIN INTIALIZATIONS
  pinMode(CHANGE, INPUT);
  pinMode(SLIDE, INPUT);
  pinMode(CURRENT_IN, INPUT);
  pinMode(CURRENT_OUT, INPUT);
  pinMode(VOLTAGE_IN, INPUT);
  pinMode(VOLTAGE_OUT, INPUT);
  pinMode(SELECT, INPUT_PULLUP);
  pinMode(WAKE_BACK_LIGHT, INPUT_PULLUP);

  pinMode(INDICATOR_LED, OUTPUT);
  pinMode(BUCK_EN, OUTPUT);


  // Intiating characters
  lcd.createChar(0, NoBattery);
  lcd.createChar(1, Charging);
  lcd.createChar(2, Charged);
  lcd.createChar(3, IdLe);
  lcd.createChar(4, Check);
  lcd.createChar(5, Skull);

  return;
}

// DETECTING STATE OF SLIDER
short int slider(short int dirc) {
  unsigned int reading = 0;
  reading = (dirc) ? analogRead(CHANGE) : analogRead(SLIDE);
  short int result = 0;
  if (abs((int)(CurrentTime - ButtonPreStart[dirc])) > BouncingTime) {
    if (reading < 1000) result = -1;
    else if (reading > 3000) result = 1;
    else result = 0;
    ButtonPreStart[dirc] = CurrentTime;
  }
  return result;
}

// DETECTING STATE OF SELECT OPTION
bool select_opt() {
  bool result = false;
  if (abs((int)(CurrentTime - ButtonPreStart_sel)) > BouncingTimeButt) {
    result = digitalRead(SELECT) ? false : true;
    ButtonPreStart_sel = CurrentTime;
  }
  return result;
}

// DETECTING STATE OF WAKE OPTION
bool wake_opt() {
  bool result = false;
  if (abs((int)(CurrentTime - ButtonPreStart_wake)) > BouncingTimeButt) {
    result = digitalRead(WAKE_BACK_LIGHT) ? false : true;
    ButtonPreStart_wake = CurrentTime;
  }
  return result;
}

// SELECTION OF NEARER SENSITIVITIES TO MAKE CLOSER APPROXIMATION
void correct_sensitivities() {
  // INPUT VOLTAGE CALIBRATION ---> 0 : 24V , 1 : 28V, 2 : 32V, 3: 36V
  if (InputVoltage < 26) VinGain = VinGainList[0];
  else if (InputVoltage < 30) VinGain = VinGainList[1];
  else if (InputVoltage < 34) VinGain = VinGainList[2];
  else VinGain = VinGainList[3];

  // BATTERY VOLTAGE CALIBRATION ---> 0 : 10V, 1 : 11.5V, 2 : 13V, 3 : 15V
  if (BatteryVoltage < 10.75) VoutGain = VoutGainList[0];
  else if (BatteryVoltage < 12.25) VoutGain = VoutGainList[1];
  else if (BatteryVoltage < 14) VoutGain = VoutGainList[2];
  else VoutGain = VoutGainList[4];
  return;
}

// COMPLETING ALL THE MEASUREMENTS PERIODICALLY
void COMPLETE_MEASUREMENTS() {
  int avgCount = 150, totalCountCin = 0, totalCountCout = 0, totalCountVin = 0, totalCountVout = 0;
  float avgCountCin = 0, avgCountCout = 0, avgCountVin = 0, avgCountVout = 0;

  // TIME UPDATA
  CurrentTime = millis();

  // PERIODIC PARAMETER UPDATE
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

    // APPROXIMATE VALUES
    InputCurrent = (CinOffsetVoltage - avgCountCin * 3.3 / 4096) * CinSensitivity;
    OutputCurrent = (CoutOffsetVoltage - avgCountCout * 3.3 / 4096) * CoutSensitivity;
    InputVoltage = avgCountVin * 3.3 * VinGain / 4096;
    BatteryVoltage = avgCountVout * 3.3 * VoutGain / 4096;

    // INCREASING ACCURACY
    correct_sensitivities();
    InputVoltage = avgCountVin * 3.3 * VinGain / 4096;
    BatteryVoltage = avgCountVout * 3.3 * VoutGain / 4096;

    // POWER AND EFFICIECY MEASUREMENT
    InputPower = InputCurrent * InputVoltage;
    OutputPower = OutputCurrent * BatteryVoltage;
    Efficiency = (InputPower != 0) ? OutputPower * 100 / InputPower : 0;

    // Energy Calculation
    Wh += OutputPower * RoutineMidInterval / 3600000;
    kWh = Wh / 1000;
    MWh = Wh / 1000000;

    // Serial.print("Input Current : "), Serial.print(InputCurrent), Serial.print("    OutputCurrent: "), Serial.print(OutputCurrent);
    // Serial.print("\n Input Voltage :"), Serial.print(InputVoltage), Serial.print("   BatteryVoltage :"), Serial.print(BatteryVoltage), Serial.print("\n\n");

    // char BatteryVoltageString[8];
    // dtostrf(BatteryVoltage, 4, 2, BatteryVoltageString);
    // client.publish("mppt/battery_voltage", BatteryVoltageString);
    ParamUpdateFlag = true;
  }
  return;
}