#include <wiringPi.h>
#include <stdio.h>

int main(void)
{
    wiringPiSetupGpio();  // BCM numbering

    int ledPin = 18;      // PWM pin 18

    pinMode(ledPin, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1024);
    pwmSetClock(32);

    while (1)
    {
        for (int i = 0; i <= 1023; i++) {
            pwmWrite(ledPin, i);
            delay(2);
        }

        for (int i = 1023; i >= 0; i--) {
            pwmWrite(ledPin, i);
            delay(2);
        }
    }

    return 0;
}

