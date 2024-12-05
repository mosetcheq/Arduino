#define FASTLED_ALLOW_INTERRUPTS 0
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "FastLED.h"
#include "uri/UriBraces.h"

#define NUM_LEDS 2

const char* ap_ssid = "PixMtx01";
const char* ap_pwd = "mtxadmin";

ESP8266WebServer server(80);

IPAddress local_IP(192,168,2,1);
IPAddress gateway(192,168,2,10);
IPAddress subnet(255,255,255,0);

CRGB leds[NUM_LEDS];

void setup(void)
{
  // Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ap_ssid, ap_pwd);
  IPAddress AP_IP = WiFi.softAPIP();

  WiFi.begin("quitko", "opice2020");

  server.on("/", WWW_Root);
  server.on(UriBraces("/setcolor/{}"), WWW_Color);
  server.begin();
  // Serial.println("server running");

  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void WWW_Root() {
  // Serial.println("requesting index");
  server.send(200, "text/html", "<!DOCTYPE html><head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>PixMatrix</title> <style>body{font-family: helvetica, sans-serif; font-size: 1em;}</style></head><script type=\"text/javascript\">var xhr=new XMLHttpRequest(); function sendColor(color){xhr.open('GET', '/setcolor/' + color.substr(1, 6).toUpperCase()); xhr.send();}</script><body> <h1>PixMtx</h1> <form> <label>Solid color </label> <input type=\"color\" name=\"color\" onchange=\"sendColor(this.value);\"> </form></body></html>");
}


void WWW_Color() {
  char hcolor[7];
  String color = server.pathArg(0);
  // Serial.println("requesting color change \"" + color + "\"");
  color.toCharArray(hcolor, 7);
  unsigned long int number = strtoul(hcolor, NULL, 16);
  // Serial.println(hcolor);
  // Serial.println(number);
  byte bb = number & 255;
  byte gg = (number >> 8) & 255;
  byte rr = number >> 16;
  // Serial.println(rr);
  // Serial.println(gg);
  // Serial.println(bb);
  for(int c = 0; c < NUM_LEDS; c++) {
    leds[c] = CRGB(rr, gg, bb);
  }
  FastLED.show();
}


void loop() 
{
  server.handleClient();
  // EMPTY
}
