const int B = 9;
const int G = 10;
const int R = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  for(int lvl = 0; lvl <= 255; lvl++) {
    analogWrite(R, lvl);
    delay(10);
  }
  for(int lvl = 255; lvl>= 0; lvl--) {
    analogWrite(B, lvl);
    delay(10);
  }
  for(int lvl = 0; lvl <= 255; lvl++) {
    analogWrite(G, lvl);
    delay(10);
  }
  for(int lvl = 255; lvl>= 0; lvl--) {
    analogWrite(R, lvl);
    delay(10);
  }
  for(int lvl = 0; lvl <= 255; lvl++) {
    analogWrite(B, lvl);
    delay(10);
  }
  for(int lvl = 255; lvl>= 0; lvl--) {
    analogWrite(G, lvl);
    delay(10);
  }


}
