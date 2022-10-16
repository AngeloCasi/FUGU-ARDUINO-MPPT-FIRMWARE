//====================== ARDUINO LIBRARIES (ESP32 Compatible Libraries) ============================//
// You will have to download and install the following libraries below in order to program the MPPT //
// unit. Visit TechBuilder's YouTube channel for the "MPPT" tutorial.                               //
//============================================================================================= ====//
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <PubSubClient.h>
#include <EEPROM.h>                 //SYSTEM PARAMETER  - EEPROM Library (By: Arduino)
#include <Wire.h>                   //SYSTEM PARAMETER  - WIRE Library (By: Arduino)
#include <SPI.h>                    //SYSTEM PARAMETER  - SPI Library (By: Arduino)
#include <WiFi.h>                   //SYSTEM PARAMETER  - WiFi Library (By: Arduino)
#include <WiFiClient.h>             //SYSTEM PARAMETER  - WiFi Library (By: Arduino)
#include <BlynkSimpleEsp32.h>       //SYSTEM PARAMETER  - Blynk WiFi Library For Phone App 
#include <LiquidCrystal_I2C.h>      //SYSTEM PARAMETER  - ESP32 LCD Compatible Library (By: Robojax)
#include <Adafruit_ADS1X15.h>       //SYSTEM PARAMETER  - ADS1115/ADS1015 ADC Library (By: Adafruit)
LiquidCrystal_I2C lcd(0x27,16,2);   //SYSTEM PARAMETER  - Configure LCD RowCol Size and I2C Address
TaskHandle_t Core2;                 //SYSTEM PARAMETER  - Used for the ESP32 dual core operation
//Adafruit_ADS1015 ads;               //SYSTEM PARAMETER  - ADS1015 ADC Library (By: Adafruit) Kindly delete this line if you are using ADS1115
Adafruit_ADS1115 ads;             //SYSTEM PARAMETER  - ADS1115 ADC Library (By: Adafruit) Kindly uncomment this if you are using ADS1115

int a = 1;
int value;
int value1;

long lastMsg = 0;
char msg[50];

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);

WebServer server(80);
//const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>ESP32 Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

//================================ MPPT FIRMWARE LCD MENU INFO =====================================//
// The lines below are for the Firmware Version info displayed on the MPPT's LCD Menu Interface     //
//==================================================================================================//
String 
firmwareInfo      = "V2.0   ",
firmwareDate      = "14/10/22",
firmwareContactR1 = "www.youtube.com/",  
firmwareContactR2 = "TechBuilder     ";        
           
//====================================== USER PARAMETERS ===========================================//
// The parameters below are the default parameters used when the MPPT charger settings have not     //
// been set or saved through the LCD menu interface or mobile phone WiFi app. Some parameters here  //
// would allow you to override or unlock features for advanced users (settings not on the LCD menu) //
//==================================================================================================//
#define backflow_MOSFET 27          //SYSTEM PARAMETER - Backflow MOSFET
#define buck_IN         33          //SYSTEM PARAMETER - Buck MOSFET Driver PWM Pin
#define buck_EN         32          //SYSTEM PARAMETER - Buck MOSFET Driver Enable Pin
#define LED             2           //SYSTEM PARAMETER - LED Indicator GPIO Pin
#define FAN             16          //SYSTEM PARAMETER - Fan GPIO Pin
#define ADC_ALERT       34          //SYSTEM PARAMETER - Fan GPIO Pin
#define TempSensor      35          //SYSTEM PARAMETER - Temperature Sensor GPIO Pin
#define buttonLeft      18          //SYSTEM PARAMETER - 
#define buttonRight     17          //SYSTEM PARAMETER -
#define buttonBack      19          //SYSTEM PARAMETER - 
#define buttonSelect    23          //SYSTEM PARAMETER -

#define MY_MQTT_SERVER_HOST "192.168.2.6"
#define MY_MQTT_SERVER_PORT 1883
//#define MY_MQTT_USER ""
//#define MY_MQTT_PASSWORD ""
#define MY_MQTT_TOPIC "sensors/mppt"
//========================================= WiFi SSID ==============================================//
// This MPPT firmware uses the Blynk phone app and arduino library for controls and data telemetry  //
// Fill in your WiFi SSID and password. You will also have to get your own authentication token     //
// from email after registering from the Blynk platform.                                            //
//==================================================================================================//
char 
auth[] = "",   //   USER PARAMETER - Input Blynk Authentication Token (From email after registration)
ssid[] = "",                   //   USER PARAMETER - Enter Your WiFi SSID
pass[] = "";               //   USER PARAMETER - Enter Your WiFi Password

//====================================== USER PARAMETERS ==========================================//
// The parameters below are the default parameters used when the MPPT charger settings have not    //
// been set or saved through the LCD menu interface or mobile phone WiFi app. Some parameters here //
// would allow you to override or unlock features for advanced users (settings not on the LCD menu)//
//=================================================================================================//
bool                                  
MPPT_Mode               = 1,           //   USER PARAMETER - Enable MPPT algorithm, when disabled charger uses CC-CV algorithm 
output_Mode             = 1,           //   USER PARAMETER - 0 = PSU MODE, 1 = Charger Mode  
disableFlashAutoLoad    = 0,           //   USER PARAMETER - Forces the MPPT to not use flash saved settings, enabling this "1" defaults to programmed firmware settings.
enablePPWM              = 1,           //   USER PARAMETER - Enables Predictive PWM, this accelerates regulation speed (only applicable for battery charging application)
enableWiFi              = 1,           //   USER PARAMETER - Enable WiFi Connection
enableFan               = 1,           //   USER PARAMETER - Enable Cooling Fan
enableBluetooth         = 1,           //   USER PARAMETER - Enable Bluetooth Connection
enableLCD               = 1,           //   USER PARAMETER - Enable LCD display
enableLCDBacklight      = 1,           //   USER PARAMETER - Enable LCD display's backlight
overrideFan             = 0,           //   USER PARAMETER - Fan always on
enableDynamicCooling    = 1;           //   USER PARAMETER - Enable for PWM cooling control 
int
serialTelemMode         = 0,           //  USER PARAMETER - Selects serial telemetry data feed (0 - Disable Serial, 1 - Display All Data, 2 - Display Essential, 3 - Number only)
pwmResolution           = 11,          //  USER PARAMETER - PWM Bit Resolution 
pwmFrequency            = 39000,       //  USER PARAMETER - PWM Switching Frequency - Hz (For Buck)
temperatureFan          = 60,          //  USER PARAMETER - Temperature threshold for fan to turn on
temperatureMax          = 90,          //  USER PARAMETER - Overtemperature, System Shudown When Exceeded (deg C)
telemCounterReset       = 0,           //  USER PARAMETER - Reset Telem Data Every (0 = Never, 1 = Day, 2 = Week, 3 = Month, 4 = Year) 
errorTimeLimit          = 1000,        //  USER PARAMETER - Time interval for reseting error counter (milliseconds)  
errorCountLimit         = 10,           //  USER PARAMETER - Maximum number of errors  
millisRoutineInterval   = 250,         //  USER PARAMETER - Time Interval Refresh Rate For Routine Functions (ms)
millisSerialInterval    = 1,           //  USER PARAMETER - Time Interval Refresh Rate For USB Serial Datafeed (ms)
millisLCDInterval       = 1000,        //  USER PARAMETER - Time Interval Refresh Rate For LCD Display (ms)
millisWiFiInterval      = 1000,        //  USER PARAMETER - Time Interval Refresh Rate For WiFi Telemetry (ms)
millisLCDBackLInterval  = 1000,        //  USER PARAMETER - Time Interval Refresh Rate For WiFi Telemetry (ms)
backlightSleepMode      = 0,           //  USER PARAMETER - 0 = Never, 1 = 10secs, 2 = 5mins, 3 = 1hr, 4 = 6 hrs, 5 = 12hrs, 6 = 1 day, 7 = 3 days, 8 = 1wk, 9 = 1month
baudRate                = 500000;      //  USER PARAMETER - USB Serial Baud Rate (bps)

float 
voltageBatteryMax       = 27.3000,     //   USER PARAMETER - Maximum Battery Charging Voltage (Output V)
voltageBatteryMin       = 0.0000,     //   USER PARAMETER - Minimum Battery Charging Voltage (Output V)
currentCharging         = 30.0000,     //   USER PARAMETER - Maximum Charging Current (A - Output)
electricalPrice         = 9.5000;      //   USER PARAMETER - Input electrical price per kWh (Dollar/kWh,Euro/kWh,Peso/kWh)


//================================== CALIBRATION PARAMETERS =======================================//
// The parameters below can be tweaked for designing your own MPPT charge controllers. Only modify //
// the values below if you know what you are doing. The values below have been pre-calibrated for  //
// MPPT charge controllers designed by TechBuilder (Angelo S. Casimiro)                            //
//=================================================================================================//
bool
ADS1015_Mode            = 0;          //  CALIB PARAMETER - Use 1 for ADS1015 ADC model use 0 for ADS1115 ADC model
int
ADC_GainSelect          = 2,          //  CALIB PARAMETER - ADC Gain Selection (0→±6.144V 3mV/bit, 1→±4.096V 2mV/bit, 2→±2.048V 1mV/bit)
avgCountVS              = 3,          //  CALIB PARAMETER - Voltage Sensor Average Sampling Count (Recommended: 3)
avgCountCS              = 4,          //  CALIB PARAMETER - Current Sensor Average Sampling Count (Recommended: 4)
avgCountTS              = 10;        //  CALIB PARAMETER - Temperature Sensor Average Sampling Count
float
inVoltageDivRatio       = 40.2156,    //  CALIB PARAMETER - Input voltage divider sensor ratio (change this value to calibrate voltage sensor)
outVoltageDivRatio      = 24.4200,    //  CALIB PARAMETER - Output voltage divider sensor ratio (change this value to calibrate voltage sensor)
vOutSystemMax           = 50.0000,    //  CALIB PARAMETER - 
cOutSystemMax           = 50.0000,    //  CALIB PARAMETER - 
ntcResistance           = 10000.00,   //  CALIB PARAMETER - NTC temp sensor's resistance. Change to 10000.00 if you are using a 10k NTC
voltageDropout          = 1.0000,     //  CALIB PARAMETER - Buck regulator's dropout voltage (DOV is present due to Max Duty Cycle Limit)
voltageBatteryThresh    = 1.5000,     //  CALIB PARAMETER - Power cuts-off when this voltage is reached (Output V)
currentInAbsolute       = 31.0000,    //  CALIB PARAMETER - Maximum Input Current The System Can Handle (A - Input)
currentOutAbsolute      = 50.0000,    //  CALIB PARAMETER - Maximum Output Current The System Can Handle (A - Input)
PPWM_margin             = 99.5000,    //  CALIB PARAMETER - Minimum Operating Duty Cycle for Predictive PWM (%)
PWM_MaxDC               = 97.0000,    //  CALIB PARAMETER - Maximum Operating Duty Cycle (%) 90%-97% is good
efficiencyRate          = 1.0000,     //  CALIB PARAMETER - Theroretical Buck Efficiency (% decimal)
currentMidPoint         = 2.5250,     //  CALIB PARAMETER - Current Sensor Midpoint (V)
currentSens             = 0.0000,     //  CALIB PARAMETER - Current Sensor Sensitivity (V/A)
currentSensV            = 0.0660,     //  CALIB PARAMETER - Current Sensor Sensitivity (mV/A)
vInSystemMin            = 10.000;     //  CALIB PARAMETER - 

//===================================== SYSTEM PARAMETERS =========================================//
// Do not change parameter values in this section. The values below are variables used by system   //
// processes. Changing the values can damage the MPPT hardware. Kindly leave it as is! However,    //
// you can access these variables to acquire data needed for your mods.                            //
//=================================================================================================//
bool
buckEnable            = 0,           // SYSTEM PARAMETER - Buck Enable Status
fanStatus             = 0,           // SYSTEM PARAMETER - Fan activity status (1 = On, 0 = Off)
bypassEnable          = 0,           // SYSTEM PARAMETER - 
chargingPause         = 0,           // SYSTEM PARAMETER - 
lowPowerMode          = 0,           // SYSTEM PARAMETER - 
buttonRightStatus     = 0,           // SYSTEM PARAMETER -
buttonLeftStatus      = 0,           // SYSTEM PARAMETER - 
buttonBackStatus      = 0,           // SYSTEM PARAMETER - 
buttonSelectStatus    = 0,           // SYSTEM PARAMETER -
buttonRightCommand    = 0,           // SYSTEM PARAMETER - 
buttonLeftCommand     = 0,           // SYSTEM PARAMETER - 
buttonBackCommand     = 0,           // SYSTEM PARAMETER - 
buttonSelectCommand   = 0,           // SYSTEM PARAMETER -
settingMode           = 0,           // SYSTEM PARAMETER -
setMenuPage           = 0,           // SYSTEM PARAMETER -
boolTemp              = 0,           // SYSTEM PARAMETER -
flashMemLoad          = 0,           // SYSTEM PARAMETER -  
confirmationMenu      = 0,           // SYSTEM PARAMETER -      
WIFI                  = 0,           // SYSTEM PARAMETER - 
BNC                   = 0,           // SYSTEM PARAMETER -  
REC                   = 0,           // SYSTEM PARAMETER - 
FLV                   = 0,           // SYSTEM PARAMETER - 
IUV                   = 0,           // SYSTEM PARAMETER - 
IOV                   = 0,           // SYSTEM PARAMETER - 
IOC                   = 0,           // SYSTEM PARAMETER - 
OUV                   = 0,           // SYSTEM PARAMETER - 
OOV                   = 0,           // SYSTEM PARAMETER - 
OOC                   = 0,           // SYSTEM PARAMETER - 
OTE                   = 0;           // SYSTEM PARAMETER - 
int
inputSource           = 0,           // SYSTEM PARAMETER - 0 = MPPT has no power source, 1 = MPPT is using solar as source, 2 = MPPTis using battery as power source
avgStoreTS            = 0,           // SYSTEM PARAMETER - Temperature Sensor uses non invasive averaging, this is used an accumulator for mean averaging
temperature           = 0,           // SYSTEM PARAMETER -
sampleStoreTS         = 0,           // SYSTEM PARAMETER - TS AVG nth Sample
pwmMax                = 0,           // SYSTEM PARAMETER -
pwmMaxLimited         = 0,           // SYSTEM PARAMETER -
PWM                   = 0,           // SYSTEM PARAMETER -
PPWM                  = 0,           // SYSTEM PARAMETER -
pwmChannel            = 0,           // SYSTEM PARAMETER -
batteryPercent        = 0,           // SYSTEM PARAMETER -
errorCount            = 0,           // SYSTEM PARAMETER -
menuPage              = 0,           // SYSTEM PARAMETER -
subMenuPage           = 0,           // SYSTEM PARAMETER -
ERR                   = 0,           // SYSTEM PARAMETER - 
conv1                 = 0,           // SYSTEM PARAMETER -
conv2                 = 0,           // SYSTEM PARAMETER -
intTemp               = 0;           // SYSTEM PARAMETER -
float
VSI                   = 0.0000,      // SYSTEM PARAMETER - Raw input voltage sensor ADC voltage
VSO                   = 0.0000,      // SYSTEM PARAMETER - Raw output voltage sensor ADC voltage
CSI                   = 0.0000,      // SYSTEM PARAMETER - Raw current sensor ADC voltage
CSI_converted         = 0.0000,      // SYSTEM PARAMETER - Actual current sensor ADC voltage 
TS                    = 0.0000,      // SYSTEM PARAMETER - Raw temperature sensor ADC value
powerInput            = 0.0000,      // SYSTEM PARAMETER - Input power (solar power) in Watts
powerInputPrev        = 0.0000,      // SYSTEM PARAMETER - Previously stored input power variable for MPPT algorithm (Watts)
powerOutput           = 0.0000,      // SYSTEM PARAMETER - Output power (battery or charing power in Watts)
energySavings         = 0.0000,      // SYSTEM PARAMETER - Energy savings in fiat currency (Peso, USD, Euros or etc...)
voltageInput          = 0.0000,      // SYSTEM PARAMETER - Input voltage (solar voltage)
voltageInputPrev      = 0.0000,      // SYSTEM PARAMETER - Previously stored input voltage variable for MPPT algorithm
voltageOutput         = 0.0000,      // SYSTEM PARAMETER - Input voltage (battery voltage)
currentInput          = 0.0000,      // SYSTEM PARAMETER - Output power (battery or charing voltage)
currentOutput         = 0.0000,      // SYSTEM PARAMETER - Output current (battery or charing current in Amperes)
TSlog                 = 0.0000,      // SYSTEM PARAMETER - Part of NTC thermistor thermal sensing code
ADC_BitReso           = 0.0000,      // SYSTEM PARAMETER - System detects the approriate bit resolution factor for ADS1015/ADS1115 ADC
daysRunning           = 0.0000,      // SYSTEM PARAMETER - Stores the total number of days the MPPT device has been running since last powered
Wh                    = 0.0000,      // SYSTEM PARAMETER - Stores the accumulated energy harvested (Watt-Hours)
kWh                   = 0.0000,      // SYSTEM PARAMETER - Stores the accumulated energy harvested (Kiliowatt-Hours)
MWh                   = 0.0000,      // SYSTEM PARAMETER - Stores the accumulated energy harvested (Megawatt-Hours)
loopTime              = 0.0000,      // SYSTEM PARAMETER -
outputDeviation       = 0.0000,      // SYSTEM PARAMETER - Output Voltage Deviation (%)
buckEfficiency        = 0.0000,      // SYSTEM PARAMETER - Measure buck converter power conversion efficiency (only applicable to my dual current sensor version)
floatTemp             = 0.0000,
vOutSystemMin         = 0.0000;     //  CALIB PARAMETER - 
unsigned long 
currentErrorMillis    = 0,           //SYSTEM PARAMETER -
currentButtonMillis   = 0,           //SYSTEM PARAMETER -
currentSerialMillis   = 0,           //SYSTEM PARAMETER -
currentRoutineMillis  = 0,           //SYSTEM PARAMETER -
currentLCDMillis      = 0,           //SYSTEM PARAMETER - 
currentLCDBackLMillis = 0,           //SYSTEM PARAMETER - 
currentWiFiMillis     = 0,           //SYSTEM PARAMETER - 
currentMenuSetMillis  = 0,           //SYSTEM PARAMETER - 
prevButtonMillis      = 0,           //SYSTEM PARAMETER -
prevSerialMillis      = 0,           //SYSTEM PARAMETER -
prevRoutineMillis     = 0,           //SYSTEM PARAMETER -
prevErrorMillis       = 0,           //SYSTEM PARAMETER -
prevWiFiMillis        = 0,           //SYSTEM PARAMETER -
prevLCDMillis         = 0,           //SYSTEM PARAMETER -
prevLCDBackLMillis    = 0,           //SYSTEM PARAMETER -
timeOn                = 0,           //SYSTEM PARAMETER -
loopTimeStart         = 0,           //SYSTEM PARAMETER - Used for the loop cycle stop watch, records the loop start time
loopTimeEnd           = 0,           //SYSTEM PARAMETER - Used for the loop cycle stop watch, records the loop end time
secondsElapsed        = 0;           //SYSTEM PARAMETER - 

//====================================== MAIN PROGRAM =============================================//
// The codes below contain all the system processes for the MPPT firmware. Most of them are called //
// from the 8 .ino tabs. The codes are too long, Arduino tabs helped me a lot in organizing them.  //
// The firmware runs on two cores of the Arduino ESP32 as seen on the two separate pairs of void   //
// setups and loops. The xTaskCreatePinnedToCore() freeRTOS function allows you to access the      //
// unused ESP32 core through Arduino. Yes it does multicore processes simultaneously!              // 
//=================================================================================================//



//================= CORE0: SETUP (DUAL CORE MODE) =====================//
void coreTwo(void * pvParameters){
  
 setupWiFi();                                              //TAB#7 - WiFi Initialization

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
//================= CORE0: LOOP (DUAL CORE MODE) ======================//
  while(1){
    Wireless_Telemetry();                                   //TAB#7 - Wireless telemetry (WiFi & Bluetooth)
    
}}

//================== CORE1: SETUP (DUAL CORE MODE) ====================//
void setup() { 
  
  //SERIAL INITIALIZATION          
  Serial.begin(baudRate);                                   //Set serial baud rate
  Serial.println("> Serial Initialized");                   //Startup message
  
  //GPIO PIN INITIALIZATION
  pinMode(backflow_MOSFET,OUTPUT);                          
  pinMode(buck_EN,OUTPUT);
  pinMode(LED,OUTPUT); 
  pinMode(FAN,OUTPUT);
  pinMode(TS,INPUT); 
  pinMode(ADC_ALERT,INPUT);
  pinMode(buttonLeft,INPUT); 
  pinMode(buttonRight,INPUT); 
  pinMode(buttonBack,INPUT); 
  pinMode(buttonSelect,INPUT); 
  
  //PWM INITIALIZATION
  ledcSetup(pwmChannel,pwmFrequency,pwmResolution);          //Set PWM Parameters
  ledcAttachPin(buck_IN, pwmChannel);                        //Set pin as PWM
  ledcWrite(pwmChannel,PWM);                                 //Write PWM value at startup (duty = 0)
  pwmMax = pow(2,pwmResolution)-1;                           //Get PWM Max Bit Ceiling
  pwmMaxLimited = (PWM_MaxDC*pwmMax)/100.000;                //Get maximum PWM Duty Cycle (pwm limiting protection)
  
  //ADC INITIALIZATION
  ADC_SetGain();                                             //Sets ADC Gain & Range
  ads.begin();                                               //Initialize ADC

  //GPIO INITIALIZATION                          
  buck_Disable();

  //ENABLE DUAL CORE MULTITASKING
  xTaskCreatePinnedToCore(coreTwo,"coreTwo",10000,NULL,0,&Core2,0);
  
  //INITIALIZE AND LIOAD FLASH MEMORY DATA
  EEPROM.begin(512);
  Serial.println("> FLASH MEMORY: STORAGE INITIALIZED");  //Startup message 
  initializeFlashAutoload();                              //Load stored settings from flash memory       
  Serial.println("> FLASH MEMORY: SAVED DATA LOADED");    //Startup message 

  //LCD INITIALIZATION
  if(enableLCD==1){
    lcd.begin();
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("MPPT INITIALIZED");
    lcd.setCursor(0,1);
    lcd.print("FIRMWARE ");
    lcd.print(firmwareInfo);    
    delay(1500);
    lcd.clear();
  }

      client.setServer(MY_MQTT_SERVER_HOST, MY_MQTT_SERVER_PORT);
      client.setCallback(callback);
      client.subscribe(MY_MQTT_TOPIC"/voltageBatteryMax1");
      client.subscribe(MY_MQTT_TOPIC"/MPPT_Mode1");
      client.subscribe(MY_MQTT_TOPIC"/output_Mode1");
      client.subscribe(MY_MQTT_TOPIC"/voltageBatteryMin1");
      client.subscribe(MY_MQTT_TOPIC"/currentCharging1");
      client.subscribe(MY_MQTT_TOPIC"/enableFan1");
      client.subscribe(MY_MQTT_TOPIC"/temperatureFan1");
      client.subscribe(MY_MQTT_TOPIC"/temperatureMax1");
      client.subscribe(MY_MQTT_TOPIC"/enableWiFi1");
      client.subscribe(MY_MQTT_TOPIC"/backlightSleepMode1");
  //SETUP FINISHED
  Serial.println("> MPPT HAS INITIALIZED");                //Startup message

}

void callback(char* topic, byte* payload, unsigned int length) {
char buffer[128];
memcpy(buffer, payload, length);
buffer[length] = '\0';
char *end = nullptr;
long value = strtol(buffer, &end, 10);

// Check for conversion errors
if (end == buffer || errno == ERANGE)
  ; // Conversion error occurred
else
  Serial.println(value);

      if (String(topic) == MY_MQTT_TOPIC"/voltageBatteryMax1")
      {
    conv1 = value/100;      
    conv2 = value%100;                   
    EEPROM.write(1,conv1);
    EEPROM.write(2,conv2);
    EEPROM.commit();
    delay(5000);
    ESP.restart();
}
    if (String(topic) == MY_MQTT_TOPIC"/voltageBatteryMin1")
    {
    conv1 = value/100;       
    conv2 = value%100;                   
    EEPROM.write(3,conv1);
    EEPROM.write(4,conv2);
    EEPROM.commit();
    delay(5000);
    ESP.restart();
  }
if (String(topic) == MY_MQTT_TOPIC"/MPPT_Mode1")
    {
    conv1 = value;
    EEPROM.write(0,conv1);  
    EEPROM.commit();
    delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/output_Mode1")
    {
    conv1 = value;       
    EEPROM.write(12,conv1); 
    EEPROM.commit();
    delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/currentCharging1")
    {
    conv1 = value/100;       
    conv2 = value%100;                 
    EEPROM.write(5,conv1);
    EEPROM.write(6,conv2);
    EEPROM.commit();
        delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/enableFan1")
    {
    conv1 = value;       
    EEPROM.write(7,conv1);
    EEPROM.commit();
        delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/temperatureFan1")
    {
    conv1 = value;       
    EEPROM.write(8,conv1);
    EEPROM.commit();
        delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/temperatureMax1")
    {
    conv1 = value;       
    EEPROM.write(9,conv1);
    EEPROM.commit();
        delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/enableWiFi1")
    {
    conv1 = value;       
    EEPROM.write(10,conv1);
    EEPROM.commit();
        delay(5000);
    ESP.restart();
  }
  if (String(topic) == MY_MQTT_TOPIC"/backlightSleepMode1")
    {
    conv1 = value;       
    EEPROM.write(13,conv1);
    EEPROM.commit(); 
        delay(5000);
    ESP.restart();   
  }

}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("ESP32Client")) {
     // Serial.println("connected");
      // Subscribe
      client.subscribe(MY_MQTT_TOPIC"/voltageBatteryMax1");
      client.subscribe(MY_MQTT_TOPIC"/MPPT_Mode1");
      client.subscribe(MY_MQTT_TOPIC"/output_Mode1");
      client.subscribe(MY_MQTT_TOPIC"/voltageBatteryMin1");
      client.subscribe(MY_MQTT_TOPIC"/currentCharging1");
      client.subscribe(MY_MQTT_TOPIC"/enableFan1");
      client.subscribe(MY_MQTT_TOPIC"/temperatureFan1");
      client.subscribe(MY_MQTT_TOPIC"/temperatureMax1");
      client.subscribe(MY_MQTT_TOPIC"/enableWiFi1");
      client.subscribe(MY_MQTT_TOPIC"/backlightSleepMode1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//================== CORE1: LOOP (DUAL CORE MODE) ======================//
void loop() {
  Read_Sensors();         //TAB#2 - Sensor data measurement and computation
  Device_Protection();    //TAB#3 - Fault detection algorithm  
  System_Processes();     //TAB#4 - Routine system processes 
  Charging_Algorithm();   //TAB#5 - Battery Charging Algorithm                    
  Onboard_Telemetry();    //TAB#6 - Onboard telemetry (USB & Serial Telemetry)
  LCD_Menu();             //TAB#8 - Low Power Algorithm
  server.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
  char msg1[10];
  char msg2[10];
  char msg3[10];   
  char msg4[10];
  char msg5[10];
  char msg6[10];
  char msg7[10];
  char msg8[10];
  char msg9[10];
  char msg10[10];
  char msg11[10];
  char msg12[10];
  char msg13[10];
  char msg14[10];
  char msg15[10];
  char msg16[10];
  char msg17[10];
  char msg18[10];
  char msg19[10];
  char msg20[10];
  char msg21[10];
  char msg22[10];
  char msg23[10];
  char msg24[10];
  char msg25[10];
  char msg26[10];
  char msg27[10];
  char msg28[10];
  char msg29[10];
  char msg30[10];
  char msg31[10];
  char msg32[10];
  char msg33[10];
  char msg34[10];
  char msg35[10];
  char msg36[10];
  char msg37[10];
  char msg38[10];
  char msg39[10];
  char msg40[10];
  char msg41[10];
  char msg42[10];
  char msg43[10];
  char msg44[10];

  dtostrf(voltageInput,7, 3, msg1);
  client.publish(MY_MQTT_TOPIC"/voltagein", msg1);
  
  dtostrf(temperature,7, 3, msg2);
  client.publish(MY_MQTT_TOPIC"/temperature", msg2);
  
  dtostrf(voltageBatteryMax,7, 3, msg3);
  client.publish(MY_MQTT_TOPIC"/voltageBatteryMax", msg3);
  
  dtostrf(powerInput,7, 3, msg4);
  client.publish(MY_MQTT_TOPIC"/powerInput", msg4);
  
  dtostrf(batteryPercent,7, 3, msg5);
  client.publish(MY_MQTT_TOPIC"/batteryPercent", msg5);
  
  dtostrf(currentInput,7, 3, msg6);
  client.publish(MY_MQTT_TOPIC"/currentInput", msg6);
  
  dtostrf(voltageOutput,7, 3, msg7);
  client.publish(MY_MQTT_TOPIC"/voltageOutput", msg7);
  
  dtostrf(currentOutput,7, 3, msg8);
  client.publish(MY_MQTT_TOPIC"/currentOutput", msg8);
  
  dtostrf(Wh/1000,7, 3, msg9);
  client.publish(MY_MQTT_TOPIC"/Wh", msg9);

  dtostrf(energySavings,7, 3, msg10);
  client.publish(MY_MQTT_TOPIC"/energySavings", msg10);
  
  dtostrf(voltageBatteryMin,7, 3, msg11);
  client.publish(MY_MQTT_TOPIC"/voltageBatteryMin", msg11);
  
  dtostrf(currentCharging,7, 3, msg12);
  client.publish(MY_MQTT_TOPIC"/currentCharging", msg12);
  
  dtostrf(ESP.getHeapSize()/1024,7, 0, msg13);
  client.publish(MY_MQTT_TOPIC"/ESP.getHeapSize", msg13);
  
  dtostrf(ESP.getFlashChipMode(),7, 0, msg14);
  client.publish(MY_MQTT_TOPIC"/ESP.getFlashChipMode", msg14);
  
  dtostrf(ESP.getCpuFreqMHz() ,7, 0, msg15);
  client.publish(MY_MQTT_TOPIC"/ESP.getCpuFreqMHz", msg15);
  
  dtostrf(ESP.getChipRevision(),7, 0, msg16);
  client.publish(MY_MQTT_TOPIC"/ESP.getChipRevision", msg16);
  
  dtostrf(ESP.getFlashChipSize()/1024,7, 0, msg17);
  client.publish(MY_MQTT_TOPIC"/ESP.getFlashChipSize", msg17);
  
  dtostrf(ESP.getFlashChipSpeed()/1000000,7, 0, msg18);
  client.publish(MY_MQTT_TOPIC"/ESP.getFlashChipSpeed", msg18);
  
  dtostrf(ESP.getFreeHeap()/1024,7, 0, msg19);
  client.publish(MY_MQTT_TOPIC"/ESP.getFreeHeap", msg19);

  dtostrf(ESP.getFreePsram()/1024,7, 0, msg20);
  client.publish(MY_MQTT_TOPIC"/ESP.getFreePsram", msg20);
  
  dtostrf(ESP.getPsramSize()/1024,7, 0, msg21);
  client.publish(MY_MQTT_TOPIC"/ESP.getPsramSize", msg21);
  
  dtostrf(ESP.getMinFreeHeap()/1024,7, 0, msg22);
  client.publish(MY_MQTT_TOPIC"/ESP.getMinFreeHeap", msg22);
  
  client.publish(MY_MQTT_TOPIC"/ESP.getSdkVersion", ESP.getSdkVersion());
  
  dtostrf(ESP.getSketchSize()/1024,7, 0, msg24);
  client.publish(MY_MQTT_TOPIC"/ESP.getSketchSize", msg24);
  
  dtostrf(ERR,7, 0, msg25);
  client.publish(MY_MQTT_TOPIC"/ERR", msg25);
  
  dtostrf(FLV,7, 0, msg26);
  client.publish(MY_MQTT_TOPIC"/FLV", msg26);
  
  dtostrf(BNC,7, 0, msg27);
  client.publish(MY_MQTT_TOPIC"/BNC", msg27);
  
  dtostrf(IUV,7, 0, msg28);
  client.publish(MY_MQTT_TOPIC"/IUV", msg28);
  
  dtostrf(IOC,7, 0, msg29);
  client.publish(MY_MQTT_TOPIC"/IOC", msg29);
  
  dtostrf(OOV,7, 0, msg30);
  client.publish(MY_MQTT_TOPIC"/OOV", msg30);
  
  dtostrf(OOC,7, 0, msg31);
  client.publish(MY_MQTT_TOPIC"/OOC", msg31);
  
  dtostrf(OTE,7, 0, msg32);
  client.publish(MY_MQTT_TOPIC"/OTE", msg32);
  
  dtostrf(REC,7, 0, msg33);
  client.publish(MY_MQTT_TOPIC"/REC", msg33);
  
  dtostrf(MPPT_Mode,7, 0, msg34);
  client.publish(MY_MQTT_TOPIC"/MPPT_Mode", msg34);
  
  dtostrf(output_Mode,7, 0, msg35);
  client.publish(MY_MQTT_TOPIC"/output_Mode", msg35);
  
  dtostrf(bypassEnable,7, 0, msg36);
  client.publish(MY_MQTT_TOPIC"/bypassEnable", msg36);
  
  dtostrf(buckEnable,7, 0, msg37);
  client.publish(MY_MQTT_TOPIC"/buckEnable", msg37);
  
  dtostrf(fanStatus,7, 0, msg38);
  client.publish(MY_MQTT_TOPIC"/fanStatus", msg38);
  
  dtostrf(WIFI,7, 0, msg39);
  client.publish(MY_MQTT_TOPIC"/WIFI", msg39);
  
  dtostrf(PWM,7, 0, msg40);
  client.publish(MY_MQTT_TOPIC"/PWM", msg40);
  
  dtostrf(PPWM,7, 0, msg41);
  client.publish(MY_MQTT_TOPIC"/PPWM", msg41);
  
  dtostrf(currentMidPoint,7, 0, msg42);
  client.publish(MY_MQTT_TOPIC"/currentMidPoint", msg42);
  
  dtostrf(CSI_converted,7, 0, msg43);
  client.publish(MY_MQTT_TOPIC"/CSI_converted", msg43);
  
  dtostrf(outputDeviation,7, 0, msg44);
  client.publish(MY_MQTT_TOPIC"/outputDeviation", msg44);
  }
}
