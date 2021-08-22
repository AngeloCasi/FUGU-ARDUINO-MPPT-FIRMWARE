void padding100(int padVar){
  if(padVar<10){lcd.print("  ");}
  else if(padVar<100){lcd.print(" ");}
}
void padding10(int padVar){
  if(padVar<10){lcd.print(" ");}
}

void displayConfig1(){
  lcd.setCursor(0,0);lcd.print(powerInput,0);lcd.print("W");padding100(powerInput);      
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,3);lcd.print("Wh");}                 //9.999Wh
  else if(Wh<100){lcd.print(Wh,2);lcd.print("Wh");}           //99.99Wh
  else if(Wh<1000){lcd.print(Wh,1);lcd.print("Wh");}          //999.9Wh
  else if(Wh<10000){lcd.print(kWh,2);lcd.print("kWh");}       //9.99kWh
  else if(Wh<100000){lcd.print(kWh,1);lcd.print("kWh");}      //99.9kWh
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh ");}    //999kWh_
  else if(Wh<10000000){lcd.print(MWh,2);lcd.print("MWh");}    //9.99MWh
  else if(Wh<100000000){lcd.print(MWh,1);lcd.print("MWh");}   //99.9MWh 
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh ");} //999MWh_
  lcd.setCursor(13,0);lcd.print(daysRunning,3); 
  lcd.setCursor(0,1);lcd.print(batteryPercent); lcd.print("%");padding100(batteryPercent);
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT");}          
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);     
}
void displayConfig2(){
  lcd.setCursor(0,0); lcd.print(powerInput,0);  lcd.print("W");padding100(powerInput);    
  lcd.setCursor(5,0); lcd.print(voltageInput,1);lcd.print("V");padding10(voltageInput);            
  lcd.setCursor(11,0);lcd.print(currentInput,1);lcd.print("A");padding10(currentInput);    
  lcd.setCursor(0,1); lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent); 
  if(BNC==0){lcd.setCursor(5,1); lcd.print(voltageOutput,1);lcd.print("V");padding10(voltageOutput);}
  else{lcd.setCursor(5,1);lcd.print("NOBAT");}
  lcd.setCursor(11,1);lcd.print(currentOutput,1);lcd.print("A");padding10(currentOutput);    
}
void displayConfig3(){
  lcd.setCursor(0,0);  lcd.print(powerInput,0);lcd.print("W");padding100(powerInput); 
  lcd.setCursor(5,0);
  if(Wh<10){lcd.print(Wh,2);lcd.print("Wh");}                 //9.99Wh
  else if(Wh<100){lcd.print(Wh,1);lcd.print("Wh");}           //99.9Wh
  else if(Wh<1000){lcd.print(Wh,0);lcd.print("Wh ");}         //999Wh_
  else if(Wh<10000){lcd.print(kWh,1);lcd.print("kWh");}       //9.9kWh
  else if(Wh<100000){lcd.print(kWh,0);lcd.print("kWh ");}     //99kWh_
  else if(Wh<1000000){lcd.print(kWh,0);lcd.print("kWh");}     //999kWh
  else if(Wh<10000000){lcd.print(MWh,1);lcd.print("MWh");}    //9.9MWh
  else if(Wh<100000000){lcd.print(MWh,0);lcd.print("MWh ");}  //99MWh_
  else if(Wh<1000000000){lcd.print(MWh,0);lcd.print("MWh");}  //999Mwh
  lcd.setCursor(12,0);lcd.print(batteryPercent);lcd.print("%");padding100(batteryPercent);
  int batteryPercentBars;
  batteryPercentBars = batteryPercent/6.18; //6.25 proper value
  lcd.setCursor(0,1);
  for(int i=0;i<batteryPercentBars;i++){lcd.print((char)255);} //Battery Bar Blocks    
  for(int i=0;i<16-batteryPercentBars;i++){lcd.print(" ");}    //Battery Blanks
}
void displayConfig4(){
  lcd.setCursor(0,0);lcd.print("TEMPERATURE STAT");
  lcd.setCursor(0,1);lcd.print(temperature);lcd.print((char)223);lcd.print("C");padding100(temperature);
  lcd.setCursor(8,1);lcd.print("FAN");
  lcd.setCursor(12,1);
  if(fanStatus==1){lcd.print("ON ");}
  else{lcd.print("OFF");}
}
void displayConfig5(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS MENU ");
  lcd.setCursor(0,1);lcd.print("--PRESS SELECT--");
}
void savedMessageLCD(){
  lcd.setCursor(0,0);lcd.print(" SETTINGS SAVED ");
  lcd.setCursor(0,1);lcd.print(" SUCCESSFULLY   ");
  delay(500);
}
void cancelledMessageLCD(){
//  lcd.setCursor(0,0);lcd.print(" SETTINGS       ");
//  lcd.setCursor(0,1);lcd.print(" CANCELLED      ");
//  delay(500);
}
void LCD_Menu(){
  int 
  menuPages          = 4,
  subMenuPages       = 10,
  longPressTime      = 3000,
  longPressInterval  = 500,
  shortPressInterval = 100;

  //SETTINGS MENU
  if(settingMode==1){
    chargingPause = 1;

    //BUTTON KEYPRESS
    if(setMenuPage==0){
      if(digitalRead(buttonRight)==1){subMenuPage++;}
      if(digitalRead(buttonLeft)==1) {subMenuPage--;}
      if(digitalRead(buttonBack)==1) {settingMode=0;subMenuPage=0;}  //bool engage, main menu int page
      if(digitalRead(buttonSelect)==1){setMenuPage=1;} //enter sub menu settings - bool engage 
      while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1    
      ||digitalRead(buttonBack)==1||digitalRead(buttonSelect)==1){}
    } 
    //SUB MENU PAGE CYCLING
    if(subMenuPage>subMenuPages){subMenuPage=0;}                     
    else if(subMenuPage<0){subMenuPage=subMenuPages;}  
    //--------------------------- SETTINGS MENU PAGES: ---------------------------// 
    ///// SETTINGS MENU ITEM: CHARGING MODE /////
    if(subMenuPage==0){
      lcd.setCursor(0,0);lcd.print("CHARGING MODE   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(MPPT_Mode==1){lcd.print("MPPT CHARGER  ");}
      else{lcd.print("CC-CV BUCK PSU");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = MPPT_Mode;}
      else{
        if(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){
          while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){}
          if(MPPT_Mode==1){MPPT_Mode=0;}else{MPPT_Mode=1;}
        }
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}MPPT_Mode = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}
      }     
    }
    ///// SETTINGS MENU ITEM: MAX BATTERY V /////
    else if(subMenuPage==1){
      lcd.setCursor(0,0);lcd.print("MAX BATTERY V   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);lcd.print("V");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = voltageBatteryMax;}
      else{
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}voltageBatteryMax = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(digitalRead(buttonRight)==1){                                                    //Right button press (increments setting values)
          while(digitalRead(buttonRight)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMax += 1.00;                                                    //Increment by 1
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                           //Short Press  
              voltageBatteryMax += 0.01;                                                    //Increment by 0.01
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("V   ");                                                              //Display unit
          } 
        }
        else if(digitalRead(buttonLeft)==1){                                                //Left button press (decrements setting values)
          while(digitalRead(buttonLeft)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMax -= 1.00;                                                    //Increment by 1
              voltageBatteryMax = constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                            //Short Press  
              voltageBatteryMax -= 0.01;                                                     //Increment by 0.01
              voltageBatteryMax =  constrain(voltageBatteryMax,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMax,2);delay(shortPressInterval);   //Display settings data                                      
            } 
            lcd.print("V   ");                                                               //Display unit
          } 
        }
      }    
    }
    ///// SETTINGS MENU ITEM: MIN BATTERY V /////
    else if(subMenuPage==2){
      lcd.setCursor(0,0);lcd.print("MIN BATTERY V   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);lcd.print("V");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = voltageBatteryMin;}
      else{
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}voltageBatteryMin = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(digitalRead(buttonRight)==1){                                                    //Right button press (increments setting values)
          while(digitalRead(buttonRight)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMin += 1.00;                                                    //Increment by 1
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                           //Short Press  
              voltageBatteryMin += 0.01;                                                    //Increment by 0.01
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("V   ");                                                              //Display unit
          } 
        }
        else if(digitalRead(buttonLeft)==1){                                                //Left button press (decrements setting values)
          while(digitalRead(buttonLeft)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                                //Long Press     
              voltageBatteryMin -= 1.00;                                                    //Increment by 1
              voltageBatteryMin = constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                            //Short Press  
              voltageBatteryMin -= 0.01;                                                     //Increment by 0.01
              voltageBatteryMin =  constrain(voltageBatteryMin,vOutSystemMin,vOutSystemMax); //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(voltageBatteryMin,2);delay(shortPressInterval);   //Display settings data                                      
            } 
            lcd.print("V   ");                                                               //Display unit
          } 
        }
      }    
    }
    ///// SETTINGS MENU ITEM: CHARGING CURRENT /////
    else if(subMenuPage==3){
      lcd.setCursor(0,0);lcd.print("CHARGING CURRENT");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(currentCharging,2);lcd.print("A");  
      lcd.print("                ");  

      //SET MENU - FLOATTYPE
      if(setMenuPage==0){floatTemp = currentCharging;}
      else{
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}currentCharging = floatTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        currentMenuSetMillis = millis();
        if(digitalRead(buttonRight)==1){                                                  //Right button press (increments setting values)
          while(digitalRead(buttonRight)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                              //Long Press     
              currentCharging += 1.00;                                                    //Increment by 1
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                         //Short Press  
              currentCharging += 0.01;                                                    //Increment by 0.01
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(shortPressInterval);  //Display settings data                            
            }  
            lcd.print("A   ");                                                            //Display unit
          } 
        }
        else if(digitalRead(buttonLeft)==1){                                              //Left button press (decrements setting values)
          while(digitalRead(buttonLeft)==1){
            if(millis()-currentMenuSetMillis>longPressTime){                              //Long Press     
              currentCharging -= 1.00;                                                    //Increment by 1
              currentCharging = constrain(currentCharging,0.0,cOutSystemMax);             //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(longPressInterval);   //Display settings data                               
            }
            else{                                                                         //Short Press  
              currentCharging -= 0.01;                                                    //Increment by 0.01
              currentCharging =  constrain(currentCharging,0.0,cOutSystemMax);            //Limit settings values to a range
              lcd.setCursor(2,1);lcd.print(currentCharging,2);delay(shortPressInterval);  //Display settings data                                      
            } 
            lcd.print("A   ");                                                            //Display unit
          } 
        }
      } 
    }
    ///// SETTINGS MENU ITEM: COOLING FAN /////
    else if(subMenuPage==4){
      lcd.setCursor(0,0);lcd.print("COOLING FAN     ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(enableFan==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLE         ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = enableFan;}
      else{
        if(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){
          while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){}
          if(enableFan==1){enableFan=0;}else{enableFan=1;}
        }
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}enableFan = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}
      } 
    }
    ///// SETTINGS MENU ITEM: FAN TRIG TEMP /////
    else if(subMenuPage==5){
      lcd.setCursor(0,0);lcd.print("FAN TRIG TEMP   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(temperatureFan);
      lcd.print((char)223);lcd.print("C");lcd.print("                ");  

      //SET MENU - INTTYPE
      if(setMenuPage==0){intTemp = temperatureFan;}
      else{
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}temperatureFan = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(digitalRead(buttonRight)==1){                                              //Right button press (increments setting values)
          while(digitalRead(buttonRight)==1){   
            temperatureFan++;                                                       //Increment by 1
            temperatureFan = constrain(temperatureFan,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFan);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
        else if(digitalRead(buttonLeft)==1){                                        //Left button press (decrements setting values)
          while(digitalRead(buttonLeft)==1){ 
            temperatureFan--;                                                       //Increment by 1
            temperatureFan = constrain(temperatureFan,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureFan);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
      }         
    }
    ///// SETTINGS MENU ITEM: SHUTDOWN TEMP /////
    else if(subMenuPage==6){
      lcd.setCursor(0,0);lcd.print("SHUTDOWN TEMP   ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      lcd.setCursor(2,1);lcd.print(temperatureMax);
      lcd.print((char)223);lcd.print("C");lcd.print("                ");  

      //SET MENU - INTTYPE
      if(setMenuPage==0){intTemp = temperatureMax;}
      else{
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}temperatureMax = intTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}     
        if(digitalRead(buttonRight)==1){                                              //Right button press (increments setting values)
          while(digitalRead(buttonRight)==1){   
            temperatureMax++;                                                       //Increment by 1
            temperatureMax = constrain(temperatureMax,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
        else if(digitalRead(buttonLeft)==1){                                        //Left button press (decrements setting values)
          while(digitalRead(buttonLeft)==1){ 
            temperatureMax--;                                                       //Increment by 1
            temperatureMax = constrain(temperatureMax,0,100);                       //Limit settings values to a range
            lcd.setCursor(2,1);lcd.print(temperatureMax);delay(shortPressInterval); //Display settings data                               
            lcd.print((char)223);lcd.print("C    ");                                //Display unit
          } 
        }
      }       
    }
    ///// SETTINGS MENU ITEM: WIFI FEATURE /////
    else if(subMenuPage==7){
      lcd.setCursor(0,0);lcd.print("WIFI FEATURE    ");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(enableWiFi==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLED      ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = enableWiFi;}
      else{
        if(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){
          while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){}
          if(enableWiFi==1){enableWiFi=0;}else{enableWiFi=1;}
        }
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}enableWiFi = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveSettings();setMenuPage=0;savedMessageLCD();}
      }       
    }

    ///// SETTINGS MENU ITEM: AUTOLOAD /////
    else if(subMenuPage==8){
      lcd.setCursor(0,0);lcd.print("AUTOLOAD FEATURE");
      if(setMenuPage==1){lcd.setCursor(0,1);lcd.print(" >");}
      else{lcd.setCursor(0,1);lcd.print("= ");}
      if(flashMemLoad==1){lcd.print("ENABLED       ");}
      else{lcd.print("DISABLED      ");}

      //SET MENU - BOOLTYPE
      if(setMenuPage==0){boolTemp = flashMemLoad;}
      else{
        if(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){
          while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){}
          if(flashMemLoad==1){flashMemLoad=0;}else{flashMemLoad=1;}
        }
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}flashMemLoad = boolTemp;cancelledMessageLCD();setMenuPage=0;} 
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}saveAutoloadSettings();setMenuPage=0;savedMessageLCD();}
      }       
    }
    
    ///// SETTINGS MENU ITEM: FACTORY RESET /////
    else if(subMenuPage==9){
      if(setMenuPage==0){
        lcd.setCursor(0,0);lcd.print("FACTORY RESET   ");
        lcd.setCursor(0,1);lcd.print("> PRESS SELECT  ");  
      }
      else{
        if(confirmationMenu==0){lcd.setCursor(0,0);lcd.print(" ARE YOU SURE?  ");lcd.setCursor(0,1);lcd.print("  >NO      YES  ");}  // Display ">No"
        else{lcd.setCursor(0,0);lcd.print(" ARE YOU SURE?  ");lcd.setCursor(0,1);lcd.print("   NO     >YES  ");}                     // Display ">YES"
        if(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){while(digitalRead(buttonRight)==1||digitalRead(buttonLeft)==1){}if(confirmationMenu==0){confirmationMenu=1;}else{confirmationMenu=0;}}  //Cycle Yes NO
        if(digitalRead(buttonBack)==1){while(digitalRead(buttonBack)==1){}cancelledMessageLCD();setMenuPage=0;confirmationMenu=0;} //Cancel
        if(digitalRead(buttonSelect)==1){while(digitalRead(buttonSelect)==1){}if(confirmationMenu==1){factoryReset();savedMessageLCD();}setMenuPage=0;confirmationMenu=0;}
      } 
    }  
    ///// SETTINGS MENU ITEM: FIRMWARE VERSION /////
    else if(subMenuPage==10){
      if(setMenuPage==0){
        lcd.setCursor(0,0);lcd.print("FIRMWARE VERSION");
        lcd.setCursor(0,1);lcd.print(firmwareInfo);    
      }
      else{
        lcd.setCursor(0,0);lcd.print(firmwareContactR1);
        lcd.setCursor(0,1);lcd.print(firmwareContactR1);          
        if(digitalRead(buttonBack)==1||digitalRead(buttonSelect)==1){while(digitalRead(buttonBack)==1||digitalRead(buttonSelect)==1){}setMenuPage=0;} //Cancel
      } 
    }  
  }
  //MAIN MENU
  else if(settingMode==0){
    chargingPause = 0;

    //BUTTON KEYPRESS
    if(digitalRead(buttonRight)==1) {buttonRightCommand=1;}
    if(digitalRead(buttonLeft)==1)  {buttonLeftCommand=1;}
    if(digitalRead(buttonBack)==1)  {buttonBackCommand=1;}
    if(digitalRead(buttonSelect)==1){buttonSelectCommand=1;}
    
    currentLCDMillis = millis();
    if(currentLCDMillis-prevLCDMillis>=millisLCDInterval&&enableLCD==1){   //Run routine every millisLCDInterval (ms)
      prevLCDMillis = currentLCDMillis;     

      //MENU PAGE BUTTON ACTION
      if(buttonRightCommand==1)      {buttonRightCommand=0; lcd.clear();menuPage++;}
      else if(buttonLeftCommand==1)  {buttonLeftCommand=0;  lcd.clear();menuPage--;}
      else if(buttonBackCommand==1)  {buttonBackCommand=0;  lcd.clear();menuPage=0;}
      else if(buttonSelectCommand==1&&menuPage==4){buttonSelectCommand=0;lcd.clear();settingMode=1;}
      if(menuPage>menuPages){menuPage=0;}
      else if(menuPage<0){menuPage=menuPages;}  

      if(menuPage==0)     {displayConfig1();}
      else if(menuPage==1){displayConfig2();}
      else if(menuPage==2){displayConfig3();}
      else if(menuPage==3){displayConfig4();}
      else if(menuPage==4){displayConfig5();}
    }    
  }
}
