/////////////////////////////// MPPT CHARGE CONTROLLER //////////////////////////////////////////

////////////////////////////// DEFINITIONS /////////////////////////////////////////////////////
// GPIO : DEFININTIONS //
#define SLIDE 32   // Change between Menu Options
#define CHANGE 35  // Change the Value
#define SELECT 18
#define WAKE_BACK_LIGHT 12
#define INDICATOR_LED 2
#define BUCK_IN 13
#define BUCK_EN 14
#define BRIGHTNESS_CONTROLLER 33
#define CURRENT_IN 33
#define CURRENT_OUT 34
#define VOLTAGE_IN 39
#define VOLTAGE_OUT 36

const int BAUD_RATE = 115200;


const unsigned short int
  // DEVICE STATE
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
  BRIG_ADJ = 0,
  DEVICE_MODE = 1,
  BATTERY_CALIB = 2,
  SLEEP_TIME = 3,
  FACTORY_RESET = 4,
  RETURN = 5,

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

const char
  *ssid = "DT4G",          //change here to nearby wifi ssid
  *password = "iuytrewq",  //wifi password
    *mqtt_server = "13.76.34.183";


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
#include <WiFi.h>
#include <PubSubClient.h>



//////////////////////////////// DEVICE INTIALIZATIONS /////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20, 4);
WiFiClient espClient;
PubSubClient client(espClient);


/////////////////////////////// SYSTEM PARAMETERS //////////////////////////////////////////////
bool
  EnableLCD = true,
  OutputMode = true,
  BlinkState = false,
  BuckEnable = false,
  Error = false,
  MenuMode = false,
  ParamUpdateFlag = false,
  PrevConnectionStatus = false;

unsigned short int
  BackLightSleepMode = NEVER,
  TempSleepMode = NEVER,
  ChargingMode = DEEP_CHARGING,
  ControllerMode = MANU,
  TempControllerMode = MANU,
  SubMenu = BRIG_ADJ,
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
  InputVoltage = 12.0000,
  InputThresholdVoltage = 15.0000,
  PreInputVoltage = 12.4000,
  BatteryVoltage = 0,
  Delta = 0.3,
  BatteryMaxVoltage = 15.000,
  BatteryMinVoltage = 10.0000,
  BatteryThresholdVoltage = 9.0000,
  InputCurrent = 0.0000,
  OutputCurrent = 0.0000,
  InputPower = 0.000,
  OutputPower = 0.000,
  PreInputPower = 0.000,
  Wh = 0.0000,
  kWh = 0.0000,
  MWh = 0.0000,
  CinOffsetVoltage = 2.5102,
  CoutOffsetVoltage = 2.5058,
  CinSensitivity = 0.03000,
  CoutSensitivity = 0.0300,
  VinGain = 15.609321,
  VinGainList[] = { 15.609321, 15.609321, 15.609321, 15.609321 },  //  0 : 25V , 1 : 28V, 2 : 32V, 3: 36V
  VoutGain = 17.8592297,
  VoutGainList[] = { 17.8592297, 17.8592297, 17.8592297, 17.8592297 };  // 0 : 10V, 1 : 11.5V, 2 : 13V, 3 : 15V



unsigned long
  PrevLcdBackMillis = 0.0000,  //Time of backlight started
  PrevDispUpdateTime = 0.0000,
  PrevBackupTime = 0.0000,
  BlinkStartTime = 0.0000,
  DisconnectedTime = 0.0000,
  ButtonPreStart[2] = { 0.0000, 0.0000 },
  ButtonPreStart_sel = 0.0000,
  ButtonPreStart_wake = 0.0000,
  BouncingTime = 400,
  BouncingTimeButt = 600,
  BlinkTime = 600,
  FreezeTime = 2000,
  CurrentTime = 0.0000,
  RoutineStartTime = 0.0000,
  RoutineMidInterval = 200,
  DispUpdateInterval = 1000,
  WifiReconnectionInterval = 5000,
  BackupInterval = 60000;  // ONE MINUTES IDEAL

float BatteryLevelMatrix[6][2] = { { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } };



////////////////////////////////////////////// BODY ///////////////////////////////////////////
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.print("Serial Monitor Intialized.... !");

  PARAM_init();
  WIFI_init();
  LCD_init();
  client.setServer(mqtt_server, 1883);
  load_settings();  //NEED TO RUN SAVE SETTINGS ON THE OVERALL FIRST RUN TO SAVE PARAMETERS INTO FLASH
}


void loop() {
  COMPLETE_MEASUREMENTS();
  LCD_MENU();
  MPPT_CONTROLLER_ALGO();
  UPDATE_CONNECTED_DEVICE();
  BACKUP_PARAMETERS();
}