#include <wiringPi.h>
#include <stdio.h>

int main(void)
{
    // Initialize wiringPi using BCM GPIO numbering
    if (wiringPiSetupGpio() == -1) {
        printf("wiringPi setup failed\n");
        return 1;
    }

    int pin = 26;   // GPIO 26 (BCM)

    pinMode(pin, INPUT);
    while(1){

    int value = digitalRead(pin);

    if (value == LOW)
        printf("1\n");
    else
        printf("0\n");
}
    return 0;
}
