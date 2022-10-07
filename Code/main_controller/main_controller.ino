/////////////////////////////// MPPT CHARGE CONTROLLER //////////////////////////////////////////

////////////////////////////// DEFINITIONS /////////////////////////////////////////////////////
// GPIO : DEFININTIONS //
#define SLIDE 32   // Change between Menu Options
#define CHANGE 35  // Change the Value
#define SELECT 18
#define INDICATOR_LED 2
#define BUCK_IN 13
#define BUCK_EN 14
#define BRIGHTNESS_CONTROLLER 33
#define CURRENT_IN 33
#define CURRENT_OUT 34
#define VOLTAGE_IN 39
#define VOLTAGE_OUT 36

const int BAUD_RATE = 115200;


// DEVICE STATE
const unsigned short int
  SLEEP = 0,
  DISP = 1,

  // CONTROLLER MODES
  AUTO = 0,
  MANU = 1,

  // CHARGING MODES
  NO_BATTERY = 0,
  DEEP_CHARGING = 1,
  FLOATING = 2,
  IDLE = 3,

  // MENUS
  MAIN_DISP = 0,
  DEVICE_MODE = 1,
  BATTERY_CALIB = 2,
  SLEEP_TIME = 3,
  RETURN = 4,

  // SLIDERS
  SLIDER_X = 0,
  SLIDER_Y = 1,

  // PWM PARAMETERS
  LED_CHANNEL_BRIG = 2,
  BUCK_PWM_CHANNEL = 0,
  FREQ1 = 37000,
  RESOLU1 = 8,

  FREQ2 = 5000,
  RESOLU2 = 8,

  // BACK_LIGHT SLEEP MODE
  NEVER = 0,
  SEC_10 = 1,
  MIN_5 = 2,
  HOUR_1 = 3,
  HOUR_6 = 4,
  HOUR_12 = 5,
  DAY_1 = 6,
  DAY_3 = 7,
  WEEK_1 = 8,
  MONTH_1 = 9;



//////////////////////////////// CHARACTER DEFINITION //////////////////////////////////////////

byte NoBattery[8] = {
  0b00000,
  0b01111,
  0b01001,
  0b00011,
  0b00100,
  0b00100,
  0b00000,
  0b00100
};

byte Charging[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b11111,
  0b01110,
  0b01100,
  0b01000
};


byte Charged[8] = {
  0b00100,
  0b01110,
  0b01010,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b00000
};


byte IdLe[8] = {
  0b00001,
  0b00011,
  0b00101,
  0b01001,
  0b01001,
  0b01011,
  0b11011,
  0b11000
};

byte Check[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000
};


byte Skull[8] = {
  0b00000,
  0b01110,
  0b10101,
  0b11011,
  0b01110,
  0b01110,
  0b00000,
  0b00000
};


///////////////////////////////// LIBRARIES ////////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <EEPROM.h>



//////////////////////////////// DEVICE INTIALIZATIONS /////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20, 4);




/////////////////////////////// SYSTEM PARAMETERS //////////////////////////////////////////////
bool
  enableLCD = true,
  OutputMode = true,
  BlinkState = false,
  BuckEnable = false,
  Error = false;


unsigned short int
  BackLightSleepMode = NEVER,
  ChargingMode = DEEP_CHARGING,
  ControllerMode = MANU,
  TempControllerMode = MANU,
  Menu_Mode = MAIN_DISP,
  Efficiency = 0,
  BatteryLevel = 0,
  TempBatteryLevel = 0,
  BrightnessLevel = 30,
  TempBatteryCalib = 0,  // 0: Voltage Calibration, 1: percentage Calibration, 3 : Save Values 4 : Show Values
  calibParamCount = 3,
  ErrorCount = 0,
  PPwmMax = 95,
  PPwmMin = 20,
  PPWM = 40,
  PWM = 0;



float
  TempBatteryVoltage = 12,
  BatteryVoltage = 0,
  InputVoltage = 12.0000,
  PreInputVoltage = 12.4000,
  BatteryMaxVoltage = 15.000,
  BatteryMinVoltage = 10.0000,
  InputCurrent = 0.0000,
  OutputCurrent = 0.0000,
  InputPower = 0.000,
  PreInputPower = 0.000,
  Wh = 0.0000,
  kWh = 0.0000,
  MWh = 0.0000,
  CinOffsetVoltage = 2.5102,
  CoutOffsetVoltage = 2.5058,
  CinSensitivity = 0.03000,
  CoutSensitivity = 0.0300,
  VinGain = 15.609321,
  VoutGain = 17.8592297;


unsigned long
  PrevLcdBackMillis = 0.0000,  //Time of backlight started
  CurLcdBackMillis = 0.0000,   //Current Time to track ON time
  BlinkStartTime = 0.0000,
  ButtonPreStart[2] = { 0.0000, 0.0000 },
  ButtonPreStart_sel = 0.0000,
  BouncingTime = 200,
  BlinkTime = 600,
  FreezeTime = 2000,
  CurrentTime = 0.0000,
  RoutineStartTime = 0.0000,
  RoutineMidInterval = 500;

float BatteryLevelMatrix[6][2] = { { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } };



////////////////////////////////////////////// BODY ///////////////////////////////////////////

void setup() {

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
  pinMode(12,INPUT_PULLUP);

  pinMode(INDICATOR_LED, OUTPUT);
  pinMode(BUCK_EN, OUTPUT);

  //LCD INITIALIZATION
  if (enableLCD == 1) {
    lcd.init();
    lcd.setBacklight(HIGH);
    // set_brightness(BrightnessLevel);
    // lcd.setCursor(0, 1);
    // lcd.print("MPPT INITIALIZED");
    // lcd.setCursor(0, 2);
    // lcd.print("Version : 1.0 ");
    // delay(1500);
    lcd.clear();
  }

  // Intiating characters
  lcd.createChar(0, NoBattery);
  lcd.createChar(1, Charging);
  lcd.createChar(2, Charged);
  lcd.createChar(3, IdLe);
  lcd.createChar(4, Check);
  lcd.createChar(5, Skull);

  // saved_config(1);
  Serial.begin(BAUD_RATE);
  Serial.print("Serial Monitor Intialized.... !");
  buck_enable();
  ChargingMode = DEEP_CHARGING;
}


void loop() {
  MPPT_CONTROLLER_ALGO();
  CurrentTime = millis();
  // lcd_menu();
  complete_measurements();
}