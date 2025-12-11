#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#define LED_PIN 1  // GPIO18 (WiringPi pin 1)

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
    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(300);
        digitalWrite(LED_PIN, LOW);
        delay(300);
    }
}

int main() {
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[256];

    // WiringPi Setup
    wiringPiSetup();
    pinMode(LED_PIN, PWM_OUTPUT);  // For PWM fading
    pwmWrite(LED_PIN, 0);          // Start LED off

    // Open FIFO
    FILE *fp = fopen(fifo_path, "r");
    if (!fp) {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Waiting for QR data...\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = 0;

        printf("Received: %s\n", buffer);

        // SWITCH CASE using strcmp
        if (strcmp(buffer, "Razole") == 0) {
            printf("Action: PWM Fade 2 times\n");
            pwm_fade_twice();
        }
        else if (strcmp(buffer, "0123456789") == 0) {
            printf("Action: Blink 2 times\n");
            pinMode(LED_PIN, OUTPUT);  // change to normal output
            blink_twice();
            pinMode(LED_PIN, PWM_OUTPUT); // switch back to PWM for next fade
        }
        else if (strcmp(buffer, "0") == 0) {
            // QR NOT detected – do nothing
        }
        else {
            printf("Unknown QR code\n");
        }
    }

    fclose(fp);
    return 0;
}
