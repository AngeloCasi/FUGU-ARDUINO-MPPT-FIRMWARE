void setupWiFi(){
  if(enableWiFi==1){
    Blynk.begin(auth,ssid,pass);
    WIFI = 1;
  }
}

void Wireless_Telemetry(){
  
  ////////// WIFI TELEMETRY ////////// 
  if(enableWiFi==1){
    int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
    if(batteryPercent<=10) {LED3=200;}else{LED3=0;}  //LOW BATTERY STATUS
    if(IUV==0)             {LED4=200;}else{LED4=0;}  //PV INPUT PRESENCE STATUS

    Blynk.run();  
    Blynk.virtualWrite(1,powerInput); 
    Blynk.virtualWrite(2,batteryPercent);
    Blynk.virtualWrite(3,voltageInput);    
    Blynk.virtualWrite(4,currentInput);   
    Blynk.virtualWrite(5,voltageOutput); 
    Blynk.virtualWrite(6,currentOutput); 
    Blynk.virtualWrite(7,temperature); 
    Blynk.virtualWrite(8,Wh/1000); 
    Blynk.virtualWrite(9,energySavings);       
    Blynk.virtualWrite(10,LED1);               //LED - Battery Charging Status
    Blynk.virtualWrite(11,LED2);               //LED - Full Battery Charge Status
    Blynk.virtualWrite(12,LED3);               //LED - Low Battery Charge Status
    Blynk.virtualWrite(13,LED4);               //LED - PV Harvesting
    
    Blynk.virtualWrite(14,voltageBatteryMin);  //Minimum Battery Voltage (Read & Write)
    Blynk.virtualWrite(15,voltageBatteryMax);  //Maximum Battery Voltage (Read & Write)
    Blynk.virtualWrite(16,currentCharging);    //Charging Current  (Read & Write)
    Blynk.virtualWrite(17,electricalPrice);    //Electrical Price  (Write)
  }
  ////////// WIFI TELEMETRY ////////// 
  if(enableBluetooth==1){
    //ADD BLUETOOTH CODE
  }
  
} 
