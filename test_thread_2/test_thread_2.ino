#include <mbed.h>

using namespace mbed;
using namespace rtos;

Thread t1;
Thread t2;

void func1(){
  while (1){
    Serial.println("A");
    wait_us(2000);
  }
}

void func2(){
  while (1){
    Serial.println("B");
    wait_us(3000);
  }
}


//DigitalOut led(LED1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  t1.start(func1);
  t2.start(func2);
}

void loop() {
  // put your main code here, to run repeatedly:

}
