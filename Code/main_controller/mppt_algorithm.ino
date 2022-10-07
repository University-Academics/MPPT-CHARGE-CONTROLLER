void buck_enable() {  //Enable MPPT Buck Converter
  BuckEnable = 1;
  digitalWrite(BUCK_EN, HIGH);
  digitalWrite(INDICATOR_LED, HIGH);
}

void buck_disable() {  //Disable MPPT Buck Converter
  BuckEnable = 0;
  digitalWrite(BUCK_EN, LOW);
  digitalWrite(INDICATOR_LED, LOW);
  // PWM = 0;
}

void PPWM_constrain() {
  PPWM = constrain(PPWM, PPwmMin, PPwmMax);
  return;
}

int pwm_avalue(int ppwm, int resolution) {
  return pow(2,resolution) * ppwm / 100;
}


void detect_mode() {}

void MPPT_CONTROLLER_ALGO() {
  switch (ControllerMode) {
    case IDLE:  // NO INPUT DETECTED STATE
      break;
    case NO_BATTERY:  // NO BATTERY DETECTED STATE
      break;
    case DEEP_CHARGING:                                                              // MPPT ALGORITHM COME INTO ACTION
      if (InputPower > PreInputPower && InputVoltage > PreInputVoltage) PWM--;       //  ↑P ↑V ; →MPP  //D--
      else if (InputPower > PreInputPower && InputVoltage < PreInputVoltage) PWM++;  //↑P ↓V ; MPP←  //D++
      else if (InputPower < PreInputPower && InputVoltage > PreInputVoltage) PWM++;  //↓P ↑V ; MPP→  //D++
      else if (InputPower < PreInputPower && InputVoltage < PreInputVoltage) PWM--;  //  ↓P ↓V ; ←MPP  //D--
      else
        PPWM++;  // MPP REACHED
      break;
    case FLOATING:  // KEEPING BATTERY AT MAXIMUM VOLTAGE WITHOUT DAMAGING
      break;
  }
  if (ControllerMode == DEEP_CHARGING || ControllerMode == FLOATING) {
    PPWM_constrain();
    PWM = pwm_avalue(PPWM,RESOLU1);
    ledcWrite(BUCK_PWM_CHANNEL, PWM);
    buck_enable();
  }

  // STORING PREVIOUS VALUES
  PreInputPower = InputPower;
  PreInputVoltage = InputVoltage;
}