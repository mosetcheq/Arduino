#include <Adafruit_NeoPixel.h>
const int Pixels = 100;
const int Density = 10;
const int FadeSpeed = 6;
const float RED = 0.6284 / 5;
const float GREEN = 0.1724 / 5;
const float BLUE = 0.3142 / 5;

Adafruit_NeoPixel strip(Pixels, 8, NEO_RGB + NEO_KHZ800);

uint8_t *PixRam;
int density;
int ofs;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  PixRam = strip.getPixels();
  ofs = 0;
//  density = Density;
}


void FadeOut(uint8_t *PixRam, int len) {
  for(int pos = 0; pos < len * 3; pos ++) {
    if(PixRam[pos] > 0) PixRam[pos]--;
  }
}

void FadeOutFast(uint8_t *PixRam, int len) {
  for(int pos = 0; pos < len * 3; pos ++) {
    PixRam[pos] = PixRam[pos] >> 1;
  }
}

void FadeOutSpeed(uint8_t *PixRam, int len) {
  for(int pos = 0; pos < len * 3; pos ++) {
    if(PixRam[pos] > FadeSpeed) PixRam[pos]-= FadeSpeed;
    else PixRam[pos] = 0;
  }
}

void Scroll(uint8_t *PixRam, int len) {
  for(int pos = len * 3; pos > 3; pos --) {
    PixRam[pos] = PixRam[pos - 3];
  }
}

void Scroll2(uint8_t *PixRam, int len) {
  for(int pos = len * 3; pos > 3; pos --) {
    if(PixRam[pos] < PixRam[pos - 3]) PixRam[pos] = PixRam[pos - 3];
  }
}

void loop() {
  // put your main code here, to run repeatedly:
/*
  for(int c = 0; c < Pixels; c++) {
    PixRam[c * 3] = int((sin((c + ofs) * RED) + 1) * 127);
    PixRam[c * 3 + 1] = int((sin((c + ofs) * GREEN) + 1) * 127);
    PixRam[c * 3 + 2] = int((sin((c + ofs) * BLUE) + 1) * 127);
  }
  strip.show();
  ofs++;
  delay(20);
  */
  
  if(density-- == 0) {
    strip.setPixelColor(random(Pixels), random(255), random(255), random(255));
    // strip.setPixelColor(random(Pixels), 128, 191, 255);
    // strip.setPixelColor(0, random(255), random(255), random(255));
    density = Density;
  }
  strip.show();
  FadeOutSpeed(PixRam, Pixels);
  Scroll2(PixRam, Pixels);
  delay(20);

}
