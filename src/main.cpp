/**
 * @file main.cpp (new2.cpp)
 * @brief Main file for the ESP32 temperature scanner project.
 *
 * This file contains the main setup and loop functions, as well as the
 * necessary includes and definitions for the project.
 */

// DHT library URL: https://github.com/RobTillaart/DHTstable

#include "config.h"            //Getting configs
#include <ArduinoJson.h>       //creats json data structure
#include <DHTStable.h>         //DHT22 Library
#include <ESPAsyncWebServer.h> // Library for sending files
// #include <Preferences.h>       //Storing informations
#include <WiFi.h>   // Wi-Fi library
#include <ezTime.h> // Time

DHTStable DHT;

// const char *testEintrag = "erster Test";
// Preferences preferences;

String time() {
  // EzTime setup
  Serial.println();
  Serial.println("UTC:             " + UTC.dateTime());

  Timezone myTZ;

  // Provide official timezone names
  // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  myTZ.setLocation(F("de"));
  Serial.print(F("Uhrzeit:         "));
  Serial.println(myTZ.dateTime());

  // Sync - anti DDOS delay
  delay(500);

  // See if local time can be obtained (does not work in countries that span
  // multiple timezones)
  Serial.print(F("Local (GeoIP):   "));
  if (myTZ.setLocation()) {
    Serial.println(myTZ.dateTime());
  } else {
    Serial.println(errorString());
  }
  return myTZ.dateTime();
}

void setup() {
  pinMode(DHT22_PIN, INPUT_PULLUP); // AWEICHUNGEN VLLT OHNE WIDERSTAND?
  Serial.begin(baudRate);
  Serial.println(__FILE__);
  Serial.print("DHT LIBRARY VERSION: ");
  Serial.println(DHTSTABLE_LIB_VERSION);
  Serial.println();

  delay(500);
  // preferences.begin("temp_events", false);
  // preferences.putString("Test_Eintrag", testEintrag);
  // preferences.end();

  // time();
  //  Allocate the JSON document
  JsonDocument doc;

  JsonObject contextObject = doc.to<JsonObject>();
  contextObject["@context"] =
      "https://ref.gs1.org/standards/epcis/2.0.0/epcis-context.jsonld";

  // Add values in the document
  doc["type"] = "EPCISDocument";
  doc["schemaVersion"] = "2.0";
  doc["creationDate"] = time();

  // Add an array
  JsonArray data = doc["data"].to<JsonArray>();
  data.add(48.756080);
  data.add(2.302038);

  // Generate the minified JSON and send it to the Serial port
  // serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // Start a new line
  Serial.println();

  // Generate the prettified JSON and send it to the Serial port
  serializeJsonPretty(doc, Serial);
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
}

/**
 * @brief Sends a file to the specified IP address.
 *
 * @param ip The IP address to send the file to.
 * @param port The port number to use for sending the file.
 * @param filename The name of the file to send.
 */
void sendFile(const char *ip, int port, const char *filename) {
  // ... KEINE AHNUNG BISHER ...
}

/**
 * @brief Connects to Wi-Fi and sends a file.
 *
 * @param ssid The SSID of the Wi-Fi network to connect to.
 * @param password The password for the Wi-Fi network.
 * @param ip The IP address to send the file to.
 * @param port The port number to use for sending the file.
 */

void connectAndSendFile(const char *ssid, const char *password, const char *ip,
                        int port) {
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Send file to the specified IP address
  sendFile(ip, port, "filename.json");

  // Disconnect from Wi-Fi
  WiFi.disconnect();
  Serial.println("Disconnected from WiFi");
}

/**
 * @brief Checks if any Wi-Fi connection is available and attempts to connect.
 */
void WiFiScanner() {
  if (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i < NUM_WIFI_CONFIGS; i++) {
      const char *currentSsid;
      const char *currentPassword;
      const char *currentIp;
      int currentPort;

      switch (i) {
      case 0:
        currentSsid = WIFI_SSID1;
        currentPassword = WIFI_PASSWORD1;
        currentIp = WIFI_IP1;
        currentPort = WIFI_PORT1;
        break;
      case 1:
        currentSsid = WIFI_SSID2;
        currentPassword = WIFI_PASSWORD2;
        currentIp = WIFI_IP2;
        currentPort = WIFI_PORT2;
        break;
      default:
        // Handle unexpected case (shouldn't happen)
        break;
      }
      // Connect and send file for the current configuration
      connectAndSendFile(currentSsid, currentPassword, currentIp, currentPort);

      // Exit loop if connection is successful
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
    }
  }
  // Handle case where connection failed for all configurations
  Serial.println("Failed to connect to any WiFi network");
}

/**
 * @brief Scans the temperature and humidity using the DHT22 sensor.
 */
int measurement = 1;
void temperatureScan() {

  Serial.print("Measurement #  \t");
  Serial.print(measurement);
  Serial.print(" \t");

  // Init to check the DHT22 module
  int chk =
      DHT.read22(DHT22_PIN); // reads the data from the DHT22 sensor connected
                             // to the - in the config.h defined - pin

  // checking DHT22 status and giving feedback
  switch (chk) {
  case DHTLIB_OK:
    Serial.print("Satus: OK,\t");
    break;
  case DHTLIB_ERROR_CHECKSUM:
    Serial.print("Satus: Checksum error,\t");
    break;
  case DHTLIB_ERROR_TIMEOUT:
    Serial.print("Satus: Time out error,\t");
    break;
  default:
    Serial.print("Satus: Unknown error,\t");
    break;
  }

  // Displaying measurment data
  /* If needed, uncommend to read humidity, too
  Serial.print(DHT.getHumidity(), 1);
  Serial.print(" %,\t");
  */
  Serial.print(DHT.getTemperature(), 1);
  Serial.print(" °C,\t");
  Serial.println();

  measurement++;
}

/*void ausgabe() {
  preferences.begin("temp_events", false);
  String value = preferences.getString("Test_Eintrag", "FEHLER");
  Serial.println("Ausgabe: " + value);
  preferences.end();
}*/

void loop() {
  temperatureScan();
  // ausgabe();
  delay(delayMeasurement);
  WiFiScanner();
  time();
}