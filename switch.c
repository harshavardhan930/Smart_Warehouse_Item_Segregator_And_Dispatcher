#include <stdio.h>
#include <wiringPi.h>

#define LED_PIN     1    // WiringPi pin for BCM GPIO 18
#define SWITCH_PIN  25   // WiringPi pin for BCM GPIO 26

int main() {

    // Setup wiringPi
    if (wiringPiSetup() == -1) {
        printf("Failed to init wiringPi\n");
        return 1;
    }

    pinMode(LED_PIN, OUTPUT);     // LED pin output
    pinMode(SWITCH_PIN, INPUT);   // Switch pin input
    pullUpDnControl(SWITCH_PIN, PUD_UP);  // Enable pull-up for stable input

    while (1) {
        int sw = digitalRead(SWITCH_PIN);
		
		int flag=0;
        if (sw == LOW) { 
		if(flag==1){
			flag=0;
           digitalWrite(LED_PIN, HIGH);
        } else {
			flag=1;
            digitalWrite(LED_PIN, LOW);
        }
		}

        delay(100); // debounce delay
    }

    return 0;
}
