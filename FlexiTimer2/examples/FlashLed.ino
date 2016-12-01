#include <FlexiTimer2.h> // Found on https://github.com/dvarrazzo/arduino/blob/master/ardulib/FlexiTimer2.h

// Switch on LED on pin 13 each second


static boolean output = HIGH;

void flash() {
  digitalWrite(13, output);
  output = !output;
}

void setup() {
  pinMode(13, OUTPUT);

  FlexiTimer2::set(500, 1, flash); // 500ms period
  FlexiTimer2::start();
}

long Next = millis()+15000;
void loop() {
  if (millis() > Next) {
    digitalWrite(13,output);
    output = !output;
    Next += 250 + 250*output;
  }
}
