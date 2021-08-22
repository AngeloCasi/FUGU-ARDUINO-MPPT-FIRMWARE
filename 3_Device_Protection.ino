void Device_Protection(){
  //ERROR COUNTER RESET
  currentRoutineMillis = millis();
  if(currentErrorMillis-prevErrorMillis>=errorTimeLimit){              //Run routine every millisErrorInterval (ms)
    prevErrorMillis = currentErrorMillis;                              //Store previous time
    if(errorCount<errorCountLimit){errorCount=0;}                      //Reset error count if it is below the limit before x milliseconds       
  } 
  
  //////// PV BACKFLOW CONTROL (INPUT MOSFET) ////////
  if(voltageInput>voltageOutput+voltageDropout||MPPT_Mode==0){bypassEnable=1;}                              //Normal Condition   - Turn on Backflow MOSFET (on by default when not in MPPT charger mode)
  else{bypassEnable=0;}                                                                                     //Input Undervoltage - Turn off bypass MOSFET and prevent PV Backflow (SS)
  digitalWrite(backflow_MOSFET,bypassEnable);                                                               //Signal backflow MOSFET GPIO pin   
      
  //FAULT DETECTION
  ERR=0;                                                                                                    //Reset local error counter
  if(temperature>temperatureMax)                                   {OTE=1;ERR++;errorCount++;}else{OTE=0;}  //OTE - OVERTEMPERATURE: System overheat detected
  if(voltageOutput<vInSystemMin&&MPPT_Mode==1)                     {BNC=1;ERR++;}             else{BNC=0;}  //BNC - BATTERY NOT CONNECTED (Does not treat BNC as error when not under MPPT mode)                                                          //Do not treat BNC as error when not under PPWM mode
  if(currentInput>currentInAbsolute)                               {IOC=1;ERR++;errorCount++;}else{IOC=0;}  //IOC - INPUT  OVERCURRENT: Input current has reached absolute limit
  if(currentOutput>currentOutAbsolute)                             {OOC=1;ERR++;errorCount++;}else{OOC=0;}  //OOC - OUTPUT OVERCURRENT: Output current has reached absolute limit 
  if(voltageInput<voltageBatteryMax+voltageDropout&&MPPT_Mode==1)  {IUV=1;ERR++;REC=1;}       else{IUV=0;}  //IUV - INPUT UNDERVOLTAGE: Input voltage is below battery voltage (for MPPT mode only)   
  if(voltageOutput>voltageBatteryMax+voltageBatteryThresh)         {OOV=1;ERR++;errorCount++;}else{OOV=0;}  //OOV - OUTPUT OVERVOLTAGE: Output voltage has reached absolute limit                     
  if(voltageInput<vInSystemMin&&voltageOutput<vInSystemMin){FLV=1;ERR++;errorCount++;}else{FLV=0;}  //FLV - Fatally low system voltage (unable to resume operation)

}
