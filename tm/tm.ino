#include "TM1638lite.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "SPI.h"
#include "Ethernet2.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFE, 0xED};

TM1638lite UI(7, 6, 5);
const uint8_t numbers[10] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */  
};

// text "Out"
const uint8_t outText[4] = {
  0, 0x3F, 0x1C, 0x78 
};

OneWire OneWire(8);
DallasTemperature DS(&OneWire);
EthernetClient client;

float temp;

void setup() {
  // put your setup code here, to run once:


  UI.reset();
  DS.begin();
  Serial.begin(9600);
  // UI.sendCommand(0x89);

  Ethernet.begin(mac);
  Serial.println(Ethernet.localIP());
  

  int devices = DS.getDeviceCount();
  for(int c = 1; c <= 8; c++) UI.setLED(c - 1, c <= devices ? 1 : 0);
  DS.setResolution(12);
  
}

void DisplayFloat(float num, byte offset = 0) {
  int curnum;
  num = num * 10;
  if((num < -990) || (num > 1250)) {
    for(int c = 0; c < 4; c++) UI.displaySS(offset + c, outText[c]);
  } else {
    uint8_t sign = num < 0 ? 64 : 0;
    num = (int)abs(num);
    UI.displaySS(offset, (num > 100 ? sign: 0));
    int divider = 100;
    for(int c = 1; c < 4; c++) {
      curnum = num / divider;
      num = num - (curnum * divider);
      UI.displaySS(offset + c, ((divider == 100) && (curnum == 0) ? sign : numbers[curnum] + (divider == 10 ? 128 : 0)));
      divider = divider / 10;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  DS.requestTemperatures();

  uint8_t buttons = UI.readButtons();
  for(int c = 0; c < 8; c++) UI.setLED(c, (buttons & (1 << c) ? 1 : 0));
  temp = DS.getTempCByIndex(0);
  DisplayFloat(temp, 0);
  temp = DS.getTempCByIndex(1);
  DisplayFloat(temp, 4);

}
