void ALGO_init() {
  battery_calib_limits();
  buck_disable();
  COMPLETE_MEASUREMENTS();
  detect_mode();
  return;
}


void buck_enable() {  //Enable MPPT Buck Converter
  BuckEnable = 1;
  digitalWrite(BUCK_EN, HIGH);
  digitalWrite(INDICATOR_LED, HIGH);
}

void buck_disable() {  //Disable MPPT Buck Converter
  BuckEnable = 0;
  digitalWrite(BUCK_EN, LOW);
  digitalWrite(INDICATOR_LED, LOW);
}

void PPWM_constrain() {
  PPWM = constrain(PPWM, PPwmMin, PPwmMax);
  return;
}

int pwm_avalue(int ppwm, int resolution) {
  return pow(2, resolution) * ppwm / 100;
}


void detect_mode() {
  buck_disable();
  delay(500);
  Serial.println(BatteryVoltage);
  if (InputVoltage < InputThresholdVoltage) ChargingMode = IDLE;
  else if (BatteryVoltage < BatteryThresholdVoltage) ChargingMode = NO_BATTERY;
  else if (BatteryVoltage > BatteryMaxVoltage - Delta) ChargingMode = FLOATING;
  else ChargingMode = DEEP_CHARGING;
  return;
}

void run_algorithm() {
  switch (ChargingMode) {
    case IDLE:  // NO INPUT DETECTED STATE
      PPWM = 50;
      break;
    case NO_BATTERY:  // NO BATTERY DETECTED STATE
      PPWM = 50;
      break;
    case DEEP_CHARGING:                                                               // MPPT ALGORITHM COME INTO ACTION
      if (InputPower > PreInputPower && InputVoltage > PreInputVoltage) PPWM--;       //  ↑P ↑V ; →MPP  //D--
      else if (InputPower > PreInputPower && InputVoltage < PreInputVoltage) PPWM++;  //↑P ↓V ; MPP←  //D++
      else if (InputPower < PreInputPower && InputVoltage > PreInputVoltage) PPWM++;  //↓P ↑V ; MPP→  //D++
      else if (InputPower < PreInputPower && InputVoltage < PreInputVoltage) PPWM--;  //  ↓P ↓V ; ←MPP  //D--
      else
        PPWM++;  // MPP REACHED
      break;
    case FLOATING:  // KEEPING BATTERY AT MAXIMUM VOLTAGE WITHOUT DAMAGING
      if (BatteryVoltage > BatteryMaxVoltage - 0.5 * Delta) PPWM--;
      else PPWM++;
      break;
  }
  // Serial.println(PPWM);

  PPWM_constrain();
  PWM = pwm_avalue(PPWM, RESOLU1);
  ledcWrite(BUCK_PWM_CHANNEL, PWM);
  if (ChargingMode = DEEP_CHARGING || ChargingMode == FLOATING) buck_enable();
  else {
    buck_disable();
    InputPower = 0.000;
    OutputPower = 0;
  }
  // STORING PREVIOUS VALUES
  PreInputPower = InputPower;
  PreInputVoltage = InputVoltage;
  // Serial.println(InputPower);     ////////////////////////////////////////////// VISUALIZATION POINT ////////////////////////////////////////////
  return;
}

void MPPT_CONTROLLER_ALGO() {
  if (abs((int)CurrentTime - DetectedTime) > DetectionInterval) {
    detect_mode();
    DetectedTime = CurrentTime;
  }
  if (ParamUpdateFlag) {
    run_algorithm();
    ParamUpdateFlag = false;
  }
  return;
}