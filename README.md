# FUGU-ARDUINO-MPPT-FIRMWARE
An open source Arduino ESP32 MPPT Solar Charge Controller firmware equipped with charging algorithms, WiFi, MQTT, OTA updates, LCD menus &amp; more!
Stay tuned for the design release and video tutorial.
To change a parameter, for example, the maximum charging voltage, you need to send a value without a dot or comma to the topic (your topic /voltageBatteryMax1). Type 1845 which would mean 18.45 volts.
And after sending the value and applying it in the system, the device will restart in 5 seconds and apply the new settings
all topics that can be changed are listed below
/voltageBatteryMax1     Maximum Battery Charging Voltage (Output V)
/voltageBatteryMin1     Minimum Battery Charging Voltage (Output V)
/MPPT_Mode1             Enable MPPT algorithm, when disabled charger uses CC-CV algorithm
/output_Mode1           0 = PSU MODE, 1 = Charger Mode 
/currentCharging1       Maximum Charging Current (A - Output)
/enableFan1             Enable Cooling Fan
/temperatureFan1        Temperature threshold for fan to turn on
/temperatureMax1        Overtemperature, System Shudown When Exceeded (deg C)
/enableWiFi1            Enable WiFi Connection
/backlightSleepMode1    Enable LCD display's backlight
so that new firmware can be downloaded not only via cable, the ability to download via OTA has been added to log in and download in the browser line, write the device address/update
