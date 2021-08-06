#include <mbed.h>
#include <mbed_memory_status.h>
#include <mbed_wait_api.h>
#include <platform/CircularBuffer.h>

using namespace mbed;

CircularBuffer<int, 32> buf;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  print_all_thread_info();
  print_heap_and_isr_stack_info();
}

void loop() {
  // put your main code here, to run repeatedly:
  wait_ns(5000);

  for (int i = 0; i<10; i++){
    buf.push(i);
  }

  int data = -1;

  Serial.print("Is the buffer empty? ");
  Serial.println(buf.empty());
  Serial.print("Is the buffer full? ");
  Serial.println(buf.full());
  Serial.print("Elements currently in the buffer: ");
  Serial.println(buf.size());

  while (buf.pop(data)){
    Serial.println(data);
    wait_ns(250);
  }

  Serial.println("-------------------");
  Serial.println("");
  
}
