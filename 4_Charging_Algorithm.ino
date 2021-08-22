void buck_Enable(){                                          //Enable MPPT Buck Converter
  buckEnable = 1;
  digitalWrite(LED,HIGH);
  digitalWrite(buck_EN,HIGH);
}
void buck_Disable(){                                         //Disable MPPT Buck Converter
  PWM = 0;
  buckEnable = 0;
  digitalWrite(LED,LOW);
  digitalWrite(buck_EN,LOW);
}   
void predictivePWM(){                                                                  //PREDICTIVE PWM ALGORITHM 
  if(enablePPWM==0){PPWM=0;}                                                           //PPWM Disabled - PPWM is set to the corresponding PPWM value for the voltage slightly beneath output voltage

  else if(MPPT_Mode==0){                                                               //PPWM Enabled (CC-CV PSU Mode) - PPWM is set to 0 (maximum PWM floor will be set to 0)
    if(voltageInput<=0){PPWM=0;}                                                       //Prevents Indefinite Answer when voltageInput is zero
    else{PPWM=(PPWM_margin*pwmMax*voltageBatteryMax)/(100.00*voltageInput);}               //Compute for predictive PWM Floor and store in variable
    if(PPWM>pwmMaxLimited){PPWM=pwmMaxLimited;}                                        //PPWM Exceeded allowable PWM ceiling; Limit PPWM to maximum allowable PWM value
    else if(PPWM<0){PPWM=0;}                                                           //PPWM Exceeded allowable PWM floor value; Limit PPWM to 0  
  }
  
  else{                                                                                //PPWM Enabled (MPPT Mode) - PPWM is set to 0 (maximum PWM floor will be set to 0)
    if(voltageInput<=0){PPWM=0;}                                                       //Prevents Indefinite Answer when voltageInput is zero
    else{PPWM=(PPWM_margin*pwmMax*voltageOutput)/(100.00*voltageInput);}               //Compute for predictive PWM Floor and store in variable
    if(PPWM>pwmMaxLimited){PPWM=pwmMaxLimited;}                                        //PPWM Exceeded allowable PWM ceiling; Limit PPWM to maximum allowable PWM value
    else if(PPWM<0){PPWM=0;}                                                           //PPWM Exceeded allowable PWM floor value; Limit PPWM to 0
  }  
}        

void Charging_Algorithm(){
  if(ERR>0||chargingPause==1){buck_Disable();}                                         //ERROR PRESENT  - Turn off MPPT buck when error is present or when chargingPause is used for a pause override
  else{                                                                                //NO ERROR FOUND - Resume to normal operation                                                                      
    if(REC==1&&MPPT_Mode==1){                                                          //IUV RECOVERY - (Only active for MPPT mode)
      REC=0;                                                                           //Reset IUV recovery boolean identifier 
      buck_Disable();                                                                  //Disable buck before PPWM initialization
      Serial.println("> Solar Panel Detected");                                        //Display serial message
      Serial.print("> Computing For Predictive PWM ");                                 //Display serial message
      for(int i = 0; i<20; i++){Serial.print(".");delay(20);}                          //For loop "loading... effect
      Serial.println("");                                                              //Display a line break on serial for next lines
      Read_Sensors();
      predictivePWM();
      PWM = PPWM;    
    }
    //NORMAL OPERATION
    else{
      buck_Enable();
      /////////////////////// MPPT & CC-CV CHARGING ALGORITHM ///////////////////////               
      if(currentOutput>currentCharging){PWM--;}                                        //Current Is Above → Decrease Duty Cycle
      else if(voltageOutput>voltageBatteryMax){PWM--;}                                 //Voltage Is Above → Decrease Duty Cycle   
      else if(voltageOutput<voltageBatteryMax && MPPT_Mode==0){PWM++;}                 //Increase duty cycle when output is below charging voltage (for CC-CV only mode)
      else{   
        if(MPPT_Mode==1){                                                              //MPPT ALGORITHM
          if(powerInput>powerInputPrev && voltageInput>voltageInputPrev)     {PWM--;}  //  ↑P ↑V ; →MPP  //D--
          else if(powerInput>powerInputPrev && voltageInput<voltageInputPrev){PWM++;}  //  ↑P ↓V ; MPP←  //D++
          else if(powerInput<powerInputPrev && voltageInput>voltageInputPrev){PWM++;}  //  ↓P ↑V ; MPP→  //D++
          else if(powerInput<powerInputPrev && voltageInput<voltageInputPrev){PWM--;}  //  ↓P ↓V ; ←MPP  //D--
          else if(voltageOutput<voltageBatteryMax && MPPT_Mode==1)           {PWM++;}  //  MP MV ; MPP Reached - 
          else{}                                                                       //  MP MV ; MPP Reached -  
          powerInputPrev   = powerInput;                                               //Store Previous Recorded Power
          voltageInputPrev = voltageInput;                                             //Store Previous Recorded Voltage         
        }      
      }  
    }      
    /////////////////////// PWM MODULATION ///////////////////////  
    predictivePWM();                                               
    if(PWM>pwmMaxLimited){PWM=pwmMaxLimited;}                                      //PWM Ceiling Limiter - Limits maximum PWM value
    else if(PWM<PPWM){PWM=PPWM;}                                                   //PWM Floor Limiter   - Limits minimum PWM value  
    ledcWrite(pwmChannel,PWM);                                                     //Set PWM duty cycle and write to GPIO when buck is enabled                
  }



  
}
