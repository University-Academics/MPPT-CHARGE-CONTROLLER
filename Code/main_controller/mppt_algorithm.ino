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
  if (InputVoltage < InputThresholdVoltage) ControllerMode = IDLE;
  else if (BatteryVoltage < BatteryThresholdVoltage) ControllerMode = NO_BATTERY;
  else if (BatteryVoltage > BatteryMaxVoltage - Delta) ControllerMode = FLOATING;
  else ControllerMode = DEEP_CHARGING;
  return;
}

void run_algorithm() {
  switch (ControllerMode) {
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

  PPWM_constrain();
  PWM = pwm_avalue(PPWM, RESOLU1);
  ledcWrite(BUCK_PWM_CHANNEL, PWM);
  if (ControllerMode = DEEP_CHARGING || ControllerMode == FLOATING) buck_enable();
  else buck_disable();
  // STORING PREVIOUS VALUES
  PreInputPower = InputPower;
  PreInputVoltage = InputVoltage;
  // Serial.println(InputPower);     ////////////////////////////////////////////// VISUALIZATION POINT ////////////////////////////////////////////
  return;
}

void MPPT_CONTROLLER_ALGO() {
  detect_mode();
  if (ParamUpdateFlag) {
    run_algorithm();
    ParamUpdateFlag = false;
  }
  return;
}