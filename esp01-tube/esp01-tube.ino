#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "uri/UriBraces.h"
#include "EEPROM.h"
#include "./index.h"

#define NUM_LEDS 60

String config[14];

const char* AP_name = "LedTube01";
const char* AP_pwd = "test";
bool scan;

ESP8266WebServer server(80);

IPAddress local_IP(10,0, 0,10);
IPAddress gateway(10,0,0,1);
IPAddress subnet(255,255,255,0);


void setup(void)
{
  Serial.begin(115200);
  EEPROM.begin(512);

  Serial.println("\nLED TUBE Driver");

  byte confSize = EEPROM.read(0);
  if(confSize != 0) {
    String configStr = "";
    for(int c = 1; c <= confSize; c++) {
      configStr = configStr + char(EEPROM.read(c));
    }
    Serial.println(configStr);
  } else {
    config[0] = "#FFFFFF";
    config[1] = "1";
    config[2] = "1";
    config[3] = "6";
    config[4] = "LedTube01";
    config[5] = "tubesetup";
    config[6] = "";
    config[7] = "";
    config[8] = "1";
    config[9] = "";
    config[10] = "";
    config[11] = "";
  }

  // WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP( config[4], config[5]);

  IPAddress AP_IP = WiFi.softAPIP();

  if(config[6] != "") {
    WiFi.begin(config[6], config[7]);
  }

  server.on("/", WWW_Root);
  /*
  server.on(UriBraces("/setcolor/{}"), WWW_Color);
  */
  server.on(UriBraces("/scan"), WWW_Scan);
  server.on(UriBraces("/config"), WWW_Config);
  server.on(UriBraces("/save"), WWW_ConfigSave);
  server.begin();
  Serial.println("server running");
  scan = false;
}

void getConfiguration() {
}


void WWW_Root() {
  Serial.println("requesting index");
  // char result[8192];
  // snprintf(result, 8192, indexHtml);
  server.send(200, "text/html", indexHtml);
}


void WWW_Color() {

}


void WWW_ConfigSave() {
  String body = server.arg("plain");
  Serial.println(body);
  if(body.length() > 0) {
    EEPROM.write(0, body.length());
    for(int c = 0; c < body.length(); c++) {
      EEPROM.write(c + 1, body[c]);
    }
    EEPROM.commit();
    server.send(200, "text/plain", "OK");
  }
  server.send(200, "text/plain", ".");
}


void WWW_Config() {
  Serial.println("Config");
  String output = ""; char conv [32];
  for(int c = 0; c <= 11; c++) {
    itoa(c, conv, 10);
    output = output + "E" + (c < 10 ? "0" : "") + conv + ":" + config[c] + ";";
  }
  Serial.println(output);
  server.send(200, "text/plain", output);
}

void WWW_Scan() {
  Serial.println("Scaning network");
  if(!scan) {
    WiFi.disconnect();
    WiFi.scanNetworks();
    scan = true;
    server.send(200, "text/plain", ".");
  } else {
    int n = WiFi.scanComplete();
    if(n < 0) {
      server.send(200, "text/plain", ".");
    } else {
      String output = "";
      for(int c = 0; c < n; c++) {
        output = output + WiFi.SSID(c) + "\n";
      }
      server.send(200, "text/plain", output);
      Serial.println(output);
      scan = false;
    }
  }
}

void loop()
{
  server.handleClient();
}
