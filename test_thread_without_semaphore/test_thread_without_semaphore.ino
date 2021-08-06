#include <mbed.h>
#include <rtos.h>
#include <platform/Callback.h>

using namespace rtos;

#define ENGINE_STACK 224
#define BRAKE_STACK 300

Semaphore sem(1);

//unsigned char *mem1;

Thread engine;
Thread brake;
Thread fork;

int commonCount = 0;
bool countUp = true;
bool countUp2 = true;


// create square wave with f = 100kHz
void engineTask(void const *name){
  while (true){
    
    if (countUp){
      digitalWrite(5, HIGH);              
    } else {
      digitalWrite(5, LOW);  
    }

    commonCount++;

    //Serial.print((char *)name);
    //Serial.print(": ");
    //Serial.println(commonCount);
    
    countUp = !countUp;
    
    
    ThisThread::sleep_for(5000);
  }
}

// create square wave with f = 100kHz
void brakeTask(void const *name){
  while (true){
    
    if (countUp2){
      digitalWrite(6, HIGH);     
               
    } else {
      digitalWrite(6, LOW);  
    }

    commonCount++;
    
    //Serial.print((char *)name);
    //Serial.print(": ");
    //Serial.println(commonCount);
    
    countUp2 = !countUp2;
    
    
    ThisThread::sleep_for(5000);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // define 3 ports
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  engine.start(mbed::callback(engineTask, (void *)"engine"));
  brake.start(mbed::callback(brakeTask, (void *)"brake"));
  //fork.start(mbed::callback(count, (void *)"fork"));
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
