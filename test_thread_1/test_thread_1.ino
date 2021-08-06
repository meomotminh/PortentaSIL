#include "Arduino.h"
#include <ChainableLED.h>
#include <mbed.h>

// demo from https://github.com/pjpmarques/ChainableLED
#define NUM_LEDS  1

//ChainableLED leds(4, 5, NUM_LEDS);

// setup is generic for both threads
void setup() {
    Serial.begin(115200);
    //leds.init();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Welcome to Arduino on Mbed OS");
}

float hue = 0.0;
boolean up = true;

// loop through all colors on the Grove Chainable LED
void loop() {
    

    delay(50);

    if (up)
        hue += 0.025;
    else
        hue -= 0.025;

    if (hue >= 1.0 && up)
        up = false;
    else if (hue <= 0.0 && !up)
        up = true;

    Serial.println(hue);
}

// another loop to blink the built-in LED
void loop_builtin_led() {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED is now on!");
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED is now off!");
    delay(1000);
}

// the thread needs a function to run, loop here
void builtin_thread_main() {
    while (1) loop_builtin_led();
}

// Mbed OS bootstrap code
int main() {
    setup();

    // start a new thread and run the 'builtin_thread_main' function in there
    Thread builtin_thread;
    builtin_thread.start(&builtin_thread_main);

    while (1) loop();
}
