#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#define LED_PIN 1   // WiringPi pin 1 = GPIO18

int busy = 0;       // lock flag (0 = free, 1 = processing)

void pwm_fade_twice() {
    for (int n = 0; n < 2; n++) {
        for (int i = 0; i < 1024; i++) {
            pwmWrite(LED_PIN, i);
            delay(1);
        }
        for (int i = 1023; i >= 0; i--) {
            pwmWrite(LED_PIN, i);
            delay(1);
        }
    }
}

void blink_twice() {
    pinMode(LED_PIN, OUTPUT);

    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
    }

    pinMode(LED_PIN, PWM_OUTPUT); // return PWM mode
}

int main() {
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[256];

    wiringPiSetup();
    pinMode(LED_PIN, PWM_OUTPUT);
    pwmWrite(LED_PIN, 0);

    FILE *fp = fopen(fifo_path, "r");
    if (!fp) {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Waiting for QR data...\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;

        // IGNORE all incoming data while busy
        if (busy == 1) {
            continue;
        }

        // If no QR detected (“0”), ignore
        if (strcmp(buffer, "0") == 0)
            continue;

        printf("Received: %s\n", buffer);

        // Lock processing
        busy = 1;

        // Perform action ONLY ONCE
        if (strcmp(buffer, "Razole") == 0) {
            printf("Action: PWM Fade 2 times\n");
            pwm_fade_twice();
        }
        else if (strcmp(buffer, "0123456789") == 0) {
            printf("Action: Blink 2 times\n");
            blink_twice();
        }
        else {
            printf("Unknown QR code\n");
        }

        // Unlock after action completed
        busy = 0;
    }

    fclose(fp);
    return 0;
}
