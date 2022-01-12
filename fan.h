#include <Arduino.h>
#include <EEPROM.h>

//======================================== FAN CONTROL ============================================//
// Class for control fan ON/OFF and speed                                                          //
//=================================================================================================//
class Fan
{
public:
    // Fan operation mode
    enum Mode
    {
        OFF,    // ALWAYS OFF
        ON,     // ALWAYS ON
        ON_OFF, // ON WHEN FAN TEMPERATURE REACHED
        DYNAMIC // DYNAMIC PWM COOLING MODE
    };

    // Fan constructor
    Fan()
    {
        EEPROM.begin(512);
        Mode mode = (Mode)EEPROM.read(EEPROM_FANMODE);
        int triggerTemperature = EEPROM.read(EEPROM_FANTRIGGERTEMPERATURE);
        int maxBoardTemperature = EEPROM.read(EEPROM_MAXBOARDTEMPERATURE);
        Setup(mode, triggerTemperature, maxBoardTemperature);
        lastUpdate = millis();
    }

    //==============================================================
    // Initialize fan
    // Params:
    // mode: OFF (ALWAYS OFF), ON (ALWAYS ON), ON_OFF (ON when temperature reaches fanMaxBoardTemperature), DYNAMIC (PWM speed control)
    // fanTriggerTemperature: Temperature for start fan
    // fanMaxBoardTemperature: Maximum allowed temperature
    //==============================================================
    void Setup(Mode fanMode, int fanTriggerTemperature, int fanMaxBoardTemperature);

    //==============================================================
    // Handle the fan
    // Params: temperature in °C
    // Return: Fan speed in percentage
    //==============================================================
    int Handle(float temperature);

private:
    int maxFanPwm;           // Max value of PWM
    int minFanPwm;           // Min value of PWM
    int triggerTemperature;  // Temperature for start fan
    int maxBoardTemperature; // Max temperature of board
    int lastUpdate;          // Last fan update
    Mode mode;
};

void Fan::Setup(Mode fanMode, int fanTriggerTemperature, int fanMaxBoardTemperature)
{
    maxFanPwm = pow(2, FAN_PWM_RESOLUTION) - 1; // Get PWM Max Bit Ceiling
    minFanPwm = (FAN_PWM_MIN * maxFanPwm) / 100;

    ledcSetup(FAN_PWM_CHANNEL, FAN_PWM_FRECUENCY, FAN_PWM_RESOLUTION); // Set PWM Parameters
    ledcAttachPin(FAN_PIN, FAN_PWM_CHANNEL);                           // Set pin as PWM
    mode = fanMode;
    triggerTemperature = fanTriggerTemperature;
    maxBoardTemperature = fanMaxBoardTemperature;
}

int Fan::Handle(float temperature)
{
    static int dutyCycle;

    // Only update every 0.01sec
    if (millis() - lastUpdate < 10)
        return dutyCycle * 100 / maxFanPwm;
    else
        lastUpdate = millis();

    switch (mode)
    {
    case OFF:
        dutyCycle = 0; // Turn off
        break;
    case ON:
        dutyCycle = maxFanPwm; // Turn on
        break;
    case ON_OFF:
        if (temperature < triggerTemperature)
            dutyCycle = 0; // Turn off
        else
            dutyCycle = maxFanPwm; // Turn on
        break;
    case DYNAMIC:
    {
        if (dutyCycle == 0 && temperature > triggerTemperature)
            dutyCycle = maxFanPwm; // Turn on fan using max power for overcoming inertia
        else if (temperature < triggerTemperature - FAN_THRESHOLD)
            dutyCycle = 0; // Turn off fan
        if (dutyCycle > 0)
        {
            // Calculate duty cycle
            float steep = (maxFanPwm - minFanPwm) / (float)(maxBoardTemperature - triggerTemperature);
            int targetDutyCycle = (temperature - triggerTemperature + FAN_THRESHOLD) * steep + minFanPwm;
            if (targetDutyCycle > maxFanPwm)
                targetDutyCycle = maxFanPwm;

            // Slow adjust duty cycle. Overcoming fan inertia and lower noise.
            if (dutyCycle < targetDutyCycle)
                dutyCycle++;
            if (dutyCycle > targetDutyCycle)
                dutyCycle--;
        }
    }
    break;
    default:
        dutyCycle = 0;
        Serial.println("Wrong fanMode. Fix and Reboot");
        break;
    }

    // Send value to FAN
    ledcWrite(FAN_PWM_CHANNEL, dutyCycle);
    return dutyCycle * 100 / maxFanPwm;
}
