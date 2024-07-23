
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>



String mainURL ="http://192.168.8.148";
JsonDocument doc;


long get_data_timer = 0;
long get_data_interval = 500; // ms

long send_data_timer = 0;
long send_data_interval = 1000; // ms


// convert float to char array
String floatToCharArray(float val, int decimal_places=3) {
  char temp[32];
  dtostrf(val, 0, decimal_places, temp);
  return String(temp);
}



void set_value(String key, String value) {
  WiFiClient client;
  HTTPClient http;

  // Serial.print("[HTTP] begin...");

  if (http.begin(client, mainURL + "/set_value/" + key + "/" + value)) {  // HTTP
    // Serial.print("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // Serial.printf("[HTTP] GET code: %d", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        // deserializeJson(doc, payload);
        // Serial.println(payload);
      }
    } else {
      // Serial.printf("[HTTP] GET failed, error: %s", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    // Serial.printf("[HTTP} Unable to connect");
  }
}

void set_BMS_values(int bms, String values) {
  WiFiClient client;
  HTTPClient http;

  // Serial.print("[HTTP] begin...");

  if (http.begin(client, mainURL + "/set_bms_params/" + bms + "/" + values)) {  // HTTP
    // Serial.print("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // Serial.printf("[HTTP] GET code: %d", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        // deserializeJson(doc, payload);
        // Serial.println(payload);
      }
    } else {
      // Serial.printf("[HTTP] GET failed, error: %s", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    // Serial.printf("[HTTP} Unable to connect");
  }
}


double get_max_delta() {
  // find minimum of cell voltage of all cells
  double min_voltage = 5;
  double max_voltage = 0;
  for (int i = 0; i < 4; i++) {
    if (bms.get_cell_voltage(i) < min_voltage) {
      min_voltage = bms.get_cell_voltage(i);
    } 
    if (bms.get_cell_voltage(i) > max_voltage) {
      max_voltage = bms.get_cell_voltage(i);
    }
  }

  return max_voltage - min_voltage;
}


void sendBMSData() {
  if ((currenttime - send_data_timer) < send_data_interval) {
    return;
  }
  send_data_timer = currenttime;
  // Assign all variable values to Sensor structure
  String values = "";

  double soc = bms.get_balance_capacity();
  double capacity = bms.get_rate_capacity();
  double soc_percent = (soc / capacity) * 100.0;

  values += String(bms.get_voltage()) + "/";
  values += String(bms.get_current()) + "/";
  values += String(soc_percent) + "/";
  values += String(soc_percent) + "/";
  values += String(bms.get_discharge_mosfet_status()) + "/";
  values += String(bms.get_charge_mosfet_status()) + "/";
  values += String(bms.get_ntc_temperature(0)*1.8 + 32.0) + "/";
  values += String(bms.get_ntc_temperature(1)*1.8 + 32.0) + "/";
  values += String(soc) + "/";
  values += String(capacity) + "/";
  // cell voltages
  values += floatToCharArray(bms.get_cell_voltage(0), 3) + "/";
  values += floatToCharArray(bms.get_cell_voltage(1), 3) + "/";
  values += floatToCharArray(bms.get_cell_voltage(2), 3) + "/";
  values += floatToCharArray(bms.get_cell_voltage(3), 3) + "/";
  // balance state
  values += String(bms.get_balance_status(0)) + "/";
  values += String(bms.get_balance_status(1)) + "/";
  values += String(bms.get_balance_status(2)) + "/";
  values += String(bms.get_balance_status(3)) + "/";
  // cycle count
  values += String(bms.get_cycle_count()) + "/";
  // fault count
  values += String(bms.get_fault_count()) + "/";
  values += floatToCharArray(get_max_delta(), 3) + "/";

  values = values.substring(0, values.length() - 1);
  // Serial.println(values);
  
  set_BMS_values(1, values);

  // Blynk.virtualWrite(V5, bms.get_voltage());
  // Blynk.virtualWrite(V6, bms.get_current());
  // Blynk.virtualWrite(V7, soc_percent);
  // Blynk.virtualWrite(V8, bms.get_discharge_mosfet_status());
  // Blynk.virtualWrite(V9, bms.get_charge_mosfet_status());
  // Blynk.virtualWrite(V10, bms.get_ntc_temperature(0)*1.8 + 32.0);
  // Blynk.virtualWrite(V11, bms.get_ntc_temperature(1)*1.8 + 32.0);
  // Blynk.virtualWrite(V12, soc);
  // Blynk.virtualWrite(V13, capacity);
  // // cell voltages
  // Blynk.virtualWrite(V14, bms.get_cell_voltage(0));
  // Blynk.virtualWrite(V0, bms.get_cell_voltage(1));
  // Blynk.virtualWrite(V15, bms.get_cell_voltage(2));
  // Blynk.virtualWrite(V16, bms.get_cell_voltage(3));
  // // balance state
  // Blynk.virtualWrite(V1, bms.get_balance_status(0));
  // Blynk.virtualWrite(V2, bms.get_balance_status(1));
  // Blynk.virtualWrite(V3, bms.get_balance_status(2));
  // Blynk.virtualWrite(V4, bms.get_balance_status(3));
  // // cycle count
  // Blynk.virtualWrite(V47, bms.get_cycle_count());
  // // fault count
  // Blynk.virtualWrite(V48, bms.get_fault_count());
}


