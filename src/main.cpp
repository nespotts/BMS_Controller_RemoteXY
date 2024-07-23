
long currenttime = 0;

#include <jled.h>
// // breathe LED for 5 times, LED is connected to pin 9 (PWM capable) gpio
auto led_on = JLed(LED_BUILTIN).Breathe(3000).Forever();
auto led_off = JLed(LED_BUILTIN).LowActive().Off();

#include <ESP8266WiFi.h>
#include "network.h"
#include "bms2.h"
OverkillSolarBms2 bms = OverkillSolarBms2();

#include "RemoteXY_functions.h"
#include "RemoteXY_callbacks.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "OTA.h"


void setup()
{
  led_off.Update();
  Serial.begin(9600);

  ConnectWifi();

  OTA_Functions();

  bms.begin(&Serial);
  bms.set_query_rate(500);

  while(1) {
    bms.main_task(true);
    if (millis() >= 5000) {
        break;
    }
    delay(10);
  }


}

void loop()
{
  currenttime = millis();
  manageBlynkConnection2();
  bms.main_task(true);

  led_on.Update();
  ArduinoOTA.handle();

  // apply changes for App Outputs - only need to do this for controls
  RemoteXY_callbacks();

  sendBMSData();
}



