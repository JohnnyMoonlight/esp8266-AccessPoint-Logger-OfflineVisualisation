/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>


// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[7];
char temperatureFString[7];
float tempC;
float tempF;

//Time Interval for Loop
unsigned long timeInterval = 5000;
unsigned long currentTime;
unsigned long timeOfLastMeasurement = 0;
/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

File fsUploadFile;                                    // a File variable to temporarily store the received file

ESP8266WebServer server(80);

/* Publishes the temperature measured by getTemperature method.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */ 
void handleRoot() {
  
	server.send(200, "text/html", "<h1>Die Temperature am Sensor beträgt " + String(tempC) + " Grad Celsius.</h1>");
  Serial.println("Client connected.");
}

void handleFile() {
  
  server.send(200, "text/html", "<h1>Die Temperature am Sensor beträgt " + String(tempC) + " Grad Celsius.</h1>");
  Serial.println("Client connected.");
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
   Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/temp.csv", handleFile);

  server.begin();
  Serial.println("HTTP server started");
  SPIFFS.begin();
}



void loop() {
  //If timeInterval has passed, getTemperature and print in File.
  currentTime = millis();
  if (currentTime - timeOfLastMeasurement > timeInterval)
  {
    Serial.println(currentTime);
    server.handleClient();
    getTemperature();
    timeOfLastMeasurement = currentTime;
    //saveDataToCSV(currentTime, tempC);
      File tempLog = SPIFFS.open("temp.csv", "a");
      Serial.println("Schreibe Daten in die Datei");
      tempLog.print(currentTime);
      tempLog.print(',');
      tempLog.println(tempC);
      tempLog.close();       
    
    Serial.println("End of Loop on " + String(timeOfLastMeasurement));
    File f = SPIFFS.open("temp.csv", "r");
    Serial.println("Lesen des Dateiinhaltes:");
    //Data from file
    int i;
    for(i=0;i<f.size();i++) //Read upto complete file size
    {
       Serial.print((char)f.read());
    }
    f.close();  //Schließen der Datei
    delay (100);
  }
 }

void getTemperature() {

  do {
    DS18B20.requestTemperatures(); 
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    tempF = DS18B20.getTempFByIndex(0);
    dtostrf(tempF, 3, 2, temperatureFString);
    delay(100);
  } while (tempC == 85.0 || tempC == (-127.0));
  Serial.println(tempC);
}

void saveDataToCSV(unsigned long currentTime, float tempC)
{
      File tempLog = SPIFFS.open("/temp.csv", "a"); // Write the time and the temperature to the csv file
      tempLog.print(currentTime);
      tempLog.print(',');
      tempLog.println(tempC);
      tempLog.close(); 
}
