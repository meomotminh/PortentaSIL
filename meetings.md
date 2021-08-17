## Meeting 06.08.2021 

### Deliverable : Running Multithread on Portenta H7

### Findings 
#### IDE Selections

| IDE               |   Pros                |    Cons                                 |
| ------------------|:-------------         |:-------------                           |
| mbed Studio       | - mbed Native         |- not auto recognize Portenta H7         |
|                   | - unit tests          |- cannot upload code                     |
| mbed CLI          | - mbed Native         |- not auto recognize Portenta H7         |
|                   | - can run on VM       |- cannot upload code                     |
| platformIO        | - freedom             |- not auto recognize Portenta H7         |
|                   | - unit tests          |- can upload code with .io modification  |
| Arduino IDE       | - integrated mbed     |- no unit tests                          |
|                   | - auto detect board   |                                         |


Version : 
- Portenta H7 running on Arduino IDE using Arduino Mbed OS Portenta Boards is mbed OS 6

Uploading Problem :
- Portenta H7 follow Arduino design of uploading via Serial
- Other mbed enable board upload by using "mass storage" upload

Two possible approaches:
- Write code in mbed flavor and upload using platformIO --> Serial Monitor not work
- Write code in arduino flavor and upload using Arduino IDE --> *

### Difficulties

- mbed OS:
    - Thread API docs on constructor is inadequate (using thread constructor allow 1 thread run and stop ???)
    - some APIs are deprecated (wait())

- arduino:
    - not sure how Serial.print behave (adding unexpected delay)
    - Threads with Semaphore act similar to without semaphore (underlying unique variable access ???)

### Achievements

- upload using platformIO (but Serial monitor deactivated ???)
- using Arduino IDE:
    - run several Multithread examples
    - Test semaphore behavior
    - using Analog Discovery 2 to test parallelism 

- Test 2 : Start 2 threads, thread 1 print A, thread 2 print B

![](/images/test2.png)

- Test 3 : Test CircularBuffer

![](/images/test3.png)

- Test 4 : Using semaphore + CircularBuffer, start 2 threads doing the same method, get semaphore, push data into buffer and release semaphore, main loop as another thread to print out CircularBuffer values

![](/images/test4.png)

- Test with semaphore : Start 3 threads, count up to 100 and count down to 0 using a common variable, must get semaphore before count

![](/images/with_semaphore.png)

- Test without semaphore : Start 2 threads, count up a common variable, LED on/off alternately

![](/images/without_semaphore.png)

