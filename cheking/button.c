#include <wiringPi.h>
#include <stdio.h>

#define BUTTON 21     // BCM GPIO 23 (Physical pin 16)
#define RED_LED 16    // BCM GPIO 16 (Physical pin 36)
#define GREEN_LED 20  // BCM GPIO 20 (Physical pin 38)

int main(void)
{
    // Use BCM GPIO numbering
    if (wiringPiSetupGpio() == -1) {
        printf("wiringPi setup failed\n");
        return 1;
    }

    pinMode(BUTTON, INPUT);
    pullUpDnControl(BUTTON, PUD_UP);   // Button uses pull-up

    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    while (1) {
        int buttonState = digitalRead(BUTTON);

        if (buttonState == LOW) {   // Button pressed
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
        } else {                    // Button released
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
        }

        delay(100);   // debounce + CPU friendly
    }

    return 0;
}
