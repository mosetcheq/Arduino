#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "Adafruit_NeoPixel.h"
#include "uri/UriBraces.h"

extern "C" const uint8_t _binnary_text_start[];

#define NUM_LEDS 60

const char* ap_ssid = "PixMtx01";
const char* ap_pwd = "mtxadmin";

uint8_t *PixRam;
bool automode;
bool nextstep;

byte auto_pos;
byte auto_val[3] = {0, 0, 0};
const int autotable[24] = {
   0,  0,  1,
   0,  1,  0,
   0,  0, -1,
   1,  0,  0,
   0, -1,  0,
   0,  0,  1,
  -1,  0,  0,
   0,  0, -1
};


ESP8266WebServer server(80);

IPAddress local_IP(192,168,2,1);
IPAddress gateway(192,168,2,10);
IPAddress subnet(255,255,255,0);

Adafruit_NeoPixel Strip(NUM_LEDS, 2, NEO_RGB + NEO_KHZ800);

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

  Strip.begin();
  Strip.show();
  PixRam = Strip.getPixels();

  automode = true;
  auto_pos = 0;
}

void WWW_Root() {
  // Serial.println("requesting index");
  server.send(200, "text/html", "<!DOCTYPE html><head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>PixMatrix</title> <style>body{font-family: helvetica, sans-serif; font-size: 1em;}</style></head><script type=\"text/javascript\">var xhr=new XMLHttpRequest(); function sendColor(color){xhr.open('GET', '/setcolor/' + color.substr(1, 6).toUpperCase()); xhr.send();}</script><body> <h1>PixMtx</h1> <form> <label>Solid color </label> <input type=\"color\" name=\"color\" onchange=\"sendColor(this.value);\"> </form></body></html>");
}


void WWW_Color() {
  automode = false;
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
    PixRam[c*3 + 0] = gg;
    PixRam[c*3 + 1] = rr;
    PixRam[c*3 + 2] = bb;
  }
  Strip.show();
}


void loop() 
{
  server.handleClient();
  // EMPTY
  if(automode) {
    nextstep = false;
    for(int c = 0; c < 3; c++) {
      auto_val[c]+= autotable[auto_pos * 3 + c];
      if((autotable[auto_pos * 3 + c] == 1) && (auto_val[c] == 255)) nextstep = true;
      if((autotable[auto_pos * 3 + c] == -1) && (auto_val[c] == 0)) nextstep = true;
    }
    for(int c = 0; c < NUM_LEDS; c++) {
      PixRam[c * 3 + 0] = auto_val[0];
      PixRam[c * 3 + 1] = auto_val[1];
      PixRam[c * 3 + 2] = auto_val[2];
    }
    Strip.show();
    if(nextstep) {
      nextstep = false;
      auto_pos++;
      if((auto_pos * 3) > 23) {
        auto_pos = 0;
      }
    }
    delay(10);
  }
}
