void System_Processes(){
  ///////////////// FAN COOLING /////////////////
  if(enableFan==true){
    if(enableDynamicCooling==false){                                   //STATIC PWM COOLING                        
      if(overrideFan==true){fanStatus=true;}                           //Force on fan
      else if(temperature>=temperatureFan){fanStatus=1;}               //Turn on fan when set fan temp reached
      else if(temperature<temperatureFan){fanStatus=0;}                //Turn off fan when set fan temp reached
      digitalWrite(FAN,fanStatus);                                     //Send a digital signal to the fan MOSFET
    }
    else{}                                                             //DYNAMIC PWM COOLING 
  }
  else{digitalWrite(FAN,LOW);}                                         //Fan Disabled
  
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

void loadSettings(){
  MPPT_Mode          = EEPROM.read(0);                       // Load saved charging mode setting
  voltageBatteryMax  = EEPROM.read(1)+(EEPROM.read(2)*.01);  // Load saved maximum battery voltage setting
  voltageBatteryMin  = EEPROM.read(3)+(EEPROM.read(4)*.01);  // Load saved minimum battery voltage setting
  currentCharging    = EEPROM.read(5)+(EEPROM.read(6)*.01);  // Load saved charging current setting
  enableFan          = EEPROM.read(7);                       // Load saved fan enable settings
  temperatureFan     = EEPROM.read(8);                       // Load saved fan temperature settings
  temperatureMax     = EEPROM.read(9);                       // Load saved shutdown temperature settings
  enableWiFi         = EEPROM.read(10);                      // Load saved WiFi enable settings  
  flashMemLoad       = EEPROM.read(11);                      // Load saved flash memory autoload feature  
}

void saveSettings(){
  int conv1, conv2;                    //Temporary float to integer converters
  EEPROM.write(0,MPPT_Mode);           //STORE: Charge Mode
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
  EEPROM.write(7,enableFan);           //STORE: Fan Enable
  EEPROM.write(8,temperatureFan);      //STORE: Fan Temp
  EEPROM.write(9,temperatureMax);      //STORE: Shutdown Temp
  EEPROM.write(10,enableWiFi);         //STORE: Enable WiFi
//EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload (must be excluded from bulk save, uncomment under discretion)
  EEPROM.commit();                     //Saves setting changes to flash memory
}
void saveAutoloadSettings(){
  EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload
  EEPROM.commit();                     //Saves setting changes to flash memory
}

void factoryReset(){
  EEPROM.write(0,1);    //STORE: MPPT Mode (bool)
  EEPROM.write(1,12);   //STORE: Max Battery Voltage (float whole)
  EEPROM.write(2,0);    //STORE: Max Battery Voltage (float decimal)
  EEPROM.write(3,10);   //STORE: Min Battery Voltage (float whole)
  EEPROM.write(4,0);    //STORE: Min Battery Voltage (float decimal) 
  EEPROM.write(5,30);   //STORE: Charging Current (float whole)
  EEPROM.write(6,0);    //STORE: Charging Current (float decimal)
  EEPROM.write(7,1);    //STORE: Fan Enable (Bool)
  EEPROM.write(8,60);   //STORE: Fan Temp (Integer)
  EEPROM.write(9,90);   //STORE: Shutdown Temp (Integer)
  EEPROM.write(10,1);   //STORE: Enable WiFi (Boolean)
  EEPROM.write(11,1);   //STORE: Enable autoload (on by default)
  EEPROM.commit();
  loadSettings();
}
void initializeFlashAutoload(){
  if(disableFlashAutoLoad==0){
    flashMemLoad = EEPROM.read(11);       //Load saved WiFi enable settings   
    if(flashMemLoad==1){loadSettings();}  //Load stored settings from flash memory  
  } 
}
