// Adafruit IO Subscription Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/************************ Example Starts Here *******************************/

#define LAMP_INPUT 14
#define TEMPERATURE_INPUT 2

// set up the 'room-1' feed Adafruit
AdafruitIO_Feed *room1lamp = io.feed("room-1-lamp");
AdafruitIO_Feed *room1temperature = io.feed("room-1-temperature");

// Format string with temperature
String tempString;

char temp[400];
float tempC, tmpFrac;
int tmpInt1, tmpInt2;


// Create webserver
ESP8266WebServer server(80);

// Create a oneWire instance for communication with devices on the bus
OneWire oneWire(TEMPERATURE_INPUT);

DallasTemperature sensors(&oneWire);

// sensors array
DeviceAddress insideThermometer;

void setup() {
  // Set pin mode
  pinMode(LAMP_INPUT, OUTPUT);
  pinMode(TEMPERATURE_INPUT, INPUT_PULLUP);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.println();
  Serial.println("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed. The handleMessage function (defined below) will be called whenever a message is received from adafruit io.
  room1lamp->onMessage(handleMessage);

  // wait for an MQTT connection - NOTE: when blending the HTTP and MQTT API, always use the mqttStatus, method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  room1lamp->get();

  // we are connected
  Serial.println("");
  Serial.println("IO MQTT STATUS TEXT...");
  Serial.println(io.statusText());


  // DS18B20 Settings and Search
  Serial.println("");
  Serial.println("Search DS18B20...");
  sensors.begin();
  Serial.print("Found: ");
  Serial.println(sensors.getDeviceCount(), DEC);
  
  Serial.print("Parasite Power Mode: ");
  if (sensors.isParasitePowerMode()) Serial.println("Ligado");
  else Serial.println("Desligado");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Address not found for sensor");

  // Shows the address of the sensor found on the bus
  Serial.print("Address device: ");
  printAddress(insideThermometer);
  Serial.println();

  // Set sensors resolution to 9-bit
  sensors.setResolution(insideThermometer, 9);

  Serial.print("Sensor Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  // WIFI network settings and connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected on network ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP Server UP!");
}

void loop() {
  // io.run(); is required for all sketches, it should always be present at the top of your loop function, it keeps the client connected to io.adafruit.com, and processes any incoming data.
  io.run();

  calcTemp();

  room1temperature->save(tempString);
  
  server.handleClient();
  MDNS.update();

  delay(3000);
}

// this function is called whenever a message is received from Adafruit IO. it was attached to the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());

  if(data->isTrue()) {
    digitalWrite(LAMP_INPUT, HIGH);
  }
  else {
    digitalWrite(LAMP_INPUT, LOW);
  }
}

void calcTemp() {
  sensors.requestTemperatures();
  tempC = sensors.getTempC(insideThermometer);

  //Exibe no serial monitor o valor da temperatura lida
  //Serial.print("Temperatura medida: ");
  //Serial.println(tempC);

  tmpInt1 = tempC;
  tmpFrac = tempC - tmpInt1;
  tmpInt2 = trunc(tmpFrac * 10);

  tempString = (String) tmpInt1 + "." + (String) tmpInt2;
}

void handleRoot()
{
  snprintf(
    temp, 
    400,
    "<html>\
        <head>\
          <meta http-equiv='refresh' content='5'/>\
          <title>Temperature</title>\
          <style>\
            body { background-color: #3b444b; font-family: Arial, Helvetica, Sans-Serif; Color: #fff; }\
          </style>\
        </head>\
        <body>\
          <h1>WebServer DS18B20</h1>\
          <hr>\
          <h1>Temperature: %d.%d°</h1>\
          <hr>\
        </body>\
    </html>",
    tmpInt1, tmpInt2
  );
  
  server.send(200, "text/html", temp);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// Show address device
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
