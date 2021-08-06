#include <mbed.h>
#include <rtos.h>
#include <platform/Callback.h>

using namespace rtos;

#define ENGINE_STACK 224
#define BRAKE_STACK 300

Semaphore sem(1);

//unsigned char *mem1;

Thread engine;
//Thread engine;
Thread brake;
Thread fork;

int commonCount = 0;
bool countUp = true;

void count(void const *name){
  while (true){
    sem.acquire();

    if (commonCount == 100) countUp = false;
    
    if (commonCount == 0) countUp = true;
             
    if (countUp){
      commonCount++;
    } else {
      commonCount--;
    }

    String toPrint = String((const char *)name) + ": " + commonCount;    
    Serial.println(toPrint);
      
    sem.release();
    
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  engine.start(mbed::callback(count, (void *)"engine"));
  brake.start(mbed::callback(count, (void *)"brake"));
  fork.start(mbed::callback(count, (void *)"fork"));
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(commonCount);   
}
