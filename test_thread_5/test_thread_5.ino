#include <mbed.h>
#include <mbed_wait_api.h>

using namespace mbed;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(5, HIGH);
  wait_us(1e6);
  
  digitalWrite(5, LOW);
  wait_us(1e6);

}
