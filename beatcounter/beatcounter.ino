#include "TM1638lite.h"

TM1638lite UI(4, 3, 2);
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

const uint8_t circle[6] = {
  1, 2, 4, 8, 16, 32
};

byte tick, beat, semi, pattern, cpattern, b, c;
word measure;
bool is_play;

void setup() {
  // put your setup code here, to run once:
  UI.reset();
  Serial.begin(31250);
  for(semi = 0; semi < 8; semi++) {
    UI.displaySS(semi, 64);
  }
  measure = 1;
  tick = pattern = cpattern = 0;
  beat = semi = 1;
  is_play = false;
}

void DisplayNum(int num, byte offset = 0) {
  int curnum;
  int divider = 100;
  for(int c = 0; c < 3; c++) {
    curnum = num / divider;
    num = num - (curnum * divider);
    UI.displaySS(offset + c, numbers[curnum]);
    divider = divider / 10;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  b = UI.readButtons();
  if(b != 0) {
    for(c = 0; c < 8; c++) {
      if((b & (1 << c)) != 0) {
        pattern = c;
      }
    }
  }
}


void serialEvent() {
  while(Serial.available()) {
    byte cmd = (byte)Serial.read();
    switch(cmd) {
      case 0xF8: // tick
        tick++;
        if(tick == 6) {
          tick = 0;
          if(is_play)
          {
            semi++;
            if(semi == 5) {
              beat++;
              semi = 1;
              if(beat == 5) {
                measure++;
                beat = 1;
                cpattern ++;
                DisplayNum(measure, 0);
                if(cpattern == 8) {
                  for(cpattern = pattern; cpattern < 8; cpattern ++) {
                    UI.setLED(cpattern, 1);
                  }
                  cpattern = pattern;
                }
              }
            }
          }
        }
        UI.displaySS(7, circle[tick]);
        UI.displaySS(6, numbers[semi]);
        UI.displaySS(4, numbers[beat]);
        UI.setLED(cpattern, semi & 1);
      break;
      case 0xFA: // play
        for(cpattern = 0; cpattern < 8; cpattern ++) {
           UI.setLED(cpattern, 1);
        }
        cpattern = 0;
        tick = 0;
        beat = semi = 1;
        measure = 1;
        is_play = true;
      break;
      case 0xFC: // stop
        for(semi = 0; semi < 7; semi++) {
          UI.displaySS(semi, 64);
        }
        is_play = false;
      break;
    }
  }
}
