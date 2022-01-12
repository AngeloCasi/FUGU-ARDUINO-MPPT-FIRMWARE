void System_Processes(){
  
  //////////// LOOP TIME STOPWATCH ////////////
  loopTimeStart = micros();                                            //Record Start Time
  loopTime = (loopTimeStart-loopTimeEnd)/1000.000;                     //Compute Loop Cycle Speed (mS)
  loopTimeEnd = micros();                                              //Record End Time

  ///////////// AUTO DATA RESET /////////////
  if(telemCounterReset==0){}                                           //Never Reset
  else if(telemCounterReset==1 && daysRunning>1)  {resetVariables();}  //Daily Reset
  else if(telemCounterReset==2 && daysRunning>7)  {resetVariables();}  //Weekly Reset
  else if(telemCounterReset==3 && daysRunning>30) {resetVariables();}  //Monthly Reset
  else if(telemCounterReset==4 && daysRunning>365){resetVariables();}  //Yearly Reset 

  ///////////// LOW POWER MODE /////////////
  if(lowPowerMode==1){}   
  else{}      
}

void factoryReset(){
  EEPROM.write(0,1);  //STORE: Charging Algorithm (1 = MPPT Mode)
  EEPROM.write(12,1); //STORE: Charger/PSU Mode Selection (1 = Charger Mode)
  EEPROM.write(1,12); //STORE: Max Battery Voltage (whole)
  EEPROM.write(2,0);  //STORE: Max Battery Voltage (decimal)
  EEPROM.write(3,9);  //STORE: Min Battery Voltage (whole)
  EEPROM.write(4,0);  //STORE: Min Battery Voltage (decimal) 
  EEPROM.write(5,30); //STORE: Charging Current (whole)
  EEPROM.write(6,0);  //STORE: Charging Current (decimal)
  EEPROM.write(EEPROM_FANMODE,3);                //STORE: Fan mode (decimal)
  EEPROM.write(EEPROM_FANTRIGGERTEMPERATURE,60); //STORE: Fan trigger temperature (Integer)
  EEPROM.write(EEPROM_MAXBOARDTEMPERATURE,90);   //STORE: Shutdown Temp (Integer)
  EEPROM.write(10,1); //STORE: Enable WiFi (Boolean)
  EEPROM.write(11,1); //STORE: Enable autoload (on by default)
  EEPROM.write(13,0); //STORE: LCD backlight sleep timer (default: 0 = never)
  EEPROM.commit();
  loadSettings();
}

void loadSettings(){ 
  MPPT_Mode          = EEPROM.read(0);                       // Load saved charging mode setting
  output_Mode        = EEPROM.read(12);                      // Load saved charging mode setting
  voltageBatteryMax  = EEPROM.read(1)+(EEPROM.read(2)*.01);  // Load saved maximum battery voltage setting
  voltageBatteryMin  = EEPROM.read(3)+(EEPROM.read(4)*.01);  // Load saved minimum battery voltage setting
  currentCharging    = EEPROM.read(5)+(EEPROM.read(6)*.01);  // Load saved charging current setting
  fanMode               = EEPROM.read(EEPROM_FANMODE);               // Load saved fan mode setting
  fanTriggerTemperature = EEPROM.read(EEPROM_FANTRIGGERTEMPERATURE); // Load saved fan temperature settings
  temperatureMax        = EEPROM.read(EEPROM_MAXBOARDTEMPERATURE);   // Load saved shutdown temperature settings
  enableWiFi         = EEPROM.read(10);                      // Load saved WiFi enable settings  
  flashMemLoad       = EEPROM.read(11);                      // Load saved flash memory autoload feature
  backlightSleepMode = EEPROM.read(13);                      // Load saved lcd backlight sleep timer
}

void saveSettings(){
  EEPROM.write(0,MPPT_Mode);           //STORE: Algorithm 
  EEPROM.write(12,output_Mode);        //STORE: Charge/PSU Mode Selection 
  conv1 = voltageBatteryMax*100;       //STORE: Maximum Battery Voltage (gets whole number)
  conv2 = conv1%100;                   //STORE: Maximum Battery Voltage (gets decimal number and converts to a whole number)
  EEPROM.write(1,voltageBatteryMax);
  EEPROM.write(2,conv2);
  conv1 = voltageBatteryMin*100;       //STORE: Minimum Battery Voltage (gets whole number)
  conv2 = conv1%100;                   //STORE: Minimum Battery Voltage (gets decimal number and converts to a whole number)
  EEPROM.write(3,voltageBatteryMin);
  EEPROM.write(4,conv2);
  conv1 = currentCharging*100;         //STORE: Charging Current
  conv2 = conv1%100;
  EEPROM.write(5,currentCharging);
  EEPROM.write(6,conv2);
  EEPROM.write(EEPROM_FANMODE,fanMode);                              //STORE: Fan mode
  EEPROM.write(EEPROM_FANTRIGGERTEMPERATURE, fanTriggerTemperature); //STORE: Fan Temp
  EEPROM.write(EEPROM_MAXBOARDTEMPERATURE, temperatureMax);          //STORE: Shutdown Temp
  EEPROM.write(10,enableWiFi);         //STORE: Enable WiFi
//EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload (must be excluded from bulk save, uncomment under discretion)
  EEPROM.write(13,backlightSleepMode); //STORE: LCD backlight sleep timer 
  EEPROM.commit();                     //Saves setting changes to flash memory
}
void saveAutoloadSettings(){
  EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload
  EEPROM.commit();                     //Saves setting changes to flash memory
}
void initializeFlashAutoload(){
  if(disableFlashAutoLoad==0){
    flashMemLoad = EEPROM.read(11);       //Load saved autoload (must be excluded from bulk save, uncomment under discretion) 
    if(flashMemLoad==1){loadSettings();}  //Load stored settings from flash memory  
  } 
}
