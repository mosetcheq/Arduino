bool blik1, blik2, blik3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(31250);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  blik1 = blik2 = blik3 = HIGH;
}

void loop() {
  // put your main code here, to run repeatedly:

}

void serialEvent() {
  while(Serial.available()) {
    byte cmd = (byte)Serial.read();
    if(cmd == 0xFE) {
      blik1 = !blik1;
      digitalWrite(8, blik1);
    }
    if(cmd == 0xF8) {
      blik2 = !blik2;
      digitalWrite(9, blik2);
    }
    if(cmd == 0xFA) {
      digitalWrite(10, HIGH);
    }
    if(cmd == 0xFC) {
      digitalWrite(10, LOW);
    }
  }
}
