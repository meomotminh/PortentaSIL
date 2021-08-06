#include <mbed.h>
#include <rtos.h>
#include <mbed_wait_api.h>
#include <platform/CircularBuffer.h>
#include <platform/Callback.h>

#define BUFF_SIZE 8

using namespace rtos;

// Semaphore manage access to a shared resource
// This semaphore allows one thread to access the shared buffer
Semaphore sem(1);

// These threads will write to the buffer, the first thread will read it, if there's data in it
Thread t2;
Thread t3;

// The shared buffer
mbed::CircularBuffer<char, BUFF_SIZE> buff;

// name of threads
char sandy[6] = {'S','a','n','d','y','\0'};
char bill[5] = {'B','i','l','l','\0'};

// the 2 writing threads will execute this method over and over again

void writeBuffer(char* val){
  while (true){
      // wait for the shared resource to be free
    sem.acquire();
      // no bounds check is necessary in this case
      // overflowing data will simply overwrite the same buffer
      if (!buff.empty()){
        // don't forget to release the semaphore otherwise the other threads will wait forever
        sem.release();
        continue;
      }

      // reset buffer
      buff.reset();

      // store new message
      int index = 0;

      while (val[index] != '\0'){
        buff.push(val[index++]);
      }

      // tell the semaphore that this thread is done writing
      sem.release();      
    
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  t3.start(mbed::callback(writeBuffer, bill));
  t2.start(mbed::callback(writeBuffer, sandy));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char data = 0;
  
  if (!buff.empty()){
    
    sem.acquire();
      Serial.print("The buffer contained: ");

      while (buff.pop(data)){
        Serial.print(data);
      }

      Serial.println("");

      sem.release();  
    
       
  } else {
    //Serial.println("Hier");
    wait_ns(1500000);
  }
  
}
