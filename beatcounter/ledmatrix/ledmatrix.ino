#include "LedControl.h"
LedControl lc(11, 13, 9, 4);
int x, c, bar;

byte n[10][6] = {
  {
  B11111110,
  B11000110,
  B11000110,
  B11000110,
  B11000110,
  B11111110,
  },
  {
  B00001110,
  B00000110,
  B00000110,
  B00000110,
  B00000110,
  B00000110,
  },
  {
  B11111110,
  B11000110,
  B00000110,
  B11111110,
  B11000000,
  B11111110,
  },
  {
  B11111110,
  B00000110,
  B00111110,
  B00000110,
  B11000110,
  B11111110,
  },
  {
  B11000110,
  B11000110,
  B11000110,
  B11111110,
  B00000110,
  B00000110,
  },
  {
  B11111110,
  B11000000,
  B11111110,
  B00000110,
  B11000110,
  B11111110,
  },
  {
  B11111110,
  B11000000,
  B11111110,
  B11000110,
  B11000110,
  B11111110,
  },
  {
  B11111110,
  B11000110,
  B00000110,
  B00000110,
  B00000110,
  B00000110,
  },
  {
  B11111110,
  B11000110,
  B11111110,
  B11000110,
  B11000110,
  B11111110,
  },
  {
  B11111110,
  B11000110,
  B11000110,
  B11111110,
  B00000110,
  B11111110,
  },
};


byte sn[4][6] = {
  {
  B00000111,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B10000011,
  },
  {
  B00111111,
  B00110011,
  B00000011,
  B00111111,
  B00110000,
  B10111111,
  },
  {
  B00111111,
  B00000011,
  B00001111,
  B00000011,
  B00110011,
  B10111111,
  },
  {
  B00110011,
  B00110011,
  B00110011,
  B00111111,
  B00000011,
  B10000011,
  },
};

byte msg_nomidi[32] = {
  B10001000,B00000000,B01000101,B01111001,
  B10001000,B00000000,B01000101,B01000101,
  B11001000,B00000000,B01101101,B01000101,
  B10101001,B11000000,B01010101,B01000101,
  B10011010,B00100000,B01000101,B01000101,
  B10001010,B00100000,B01000101,B01000101,
  B10001001,B11000000,B01000101,B01111001,
  B00000000,B00000000,B00000000,B00000000,
};

byte msg_midiok[32] = {
  B10001010,B11110010,B00000011,B10010001,
  B10001010,B10001010,B00000100,B01010010,
  B11011010,B10001010,B00000100,B01010100,
  B10101010,B10001010,B00000100,B01011000,
  B10001010,B10001010,B00000100,B01010100,
  B10001010,B10001010,B00000100,B01010010,
  B10001010,B11110010,B00000011,B10010001,
  B00000000,B00000000,B00000000,B00000000,
};

void setup() {
  // put your setup code here, to run once:
  for(x =0; x < 4; x++) {
    lc.shutdown(x, false);
    lc.setIntensity(x, 0);
  }
  for(c = 1; c < 4; c++) {
    for(x = 0; x < 6; x++) {
      lc.setRow(c, x, n[0][x]);
    }
  }
  for(x = 0; x < 6; x++) {
    lc.setRow(0, x, sn[0][x]);
  }
 bar = 0;
 Graphics(msg_nomidi);
 delay(2000);
 Graphics(msg_midiok);
 delay(2000);
 Clear(false);
}


void ShowBar(int num, int row, bool mode) {
  int lastled = ((mode ? 8 : 7) - (num & 7));
  int pos = num >> 3;
  for(int c = 3; c >= 0; c--) {
    if(pos > c) lc.setRow(c, row, (mode ? 255 : 0));
    if(pos == c) lc.setRow(c, row, (mode ? 255 : 128) >> lastled);
    if(pos < c) lc.setRow(c, row, 0);
  }
}

void Clear(bool mode) {
  for(c = 0; c < 4; c++) {
    for(x = 0; x < 8; x++) {
      lc.setRow(c, x, (mode ? 255 : 0));
    }
  }
}

void Graphics(byte source[]) {
 for(c = 0; c < 4; c++) {
  for(x = 0; x < 8; x++) {
    lc.setRow(3 - c, x, source[x * 4 + c]);
  }
 }
}


void DisplayNum(int num) {
  int curnum;
  int divider = 100;
  for(int c = 0; c < 3; c++) {
    curnum = num / divider;
    num = num - (curnum * divider);
    for(int r = 0; r < 6; r++) {
      lc.setRow(3 - c, n[curnum][r]);
    }
    divider = divider / 10;
  }
}


void loop() {

  // put your main code here, to run repeatedly:
  for(c = 0; c < 4; c++) {
    for(x = 0; x < 6; x++) {
      lc.setRow(0, x, sn[c][x]);
    }
    ShowBar(bar & 31, 7, false);
    bar++;
    delay(100);
  }

}
