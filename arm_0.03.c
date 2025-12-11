#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#define LED_PIN 1   // WiringPi pin 1 = GPIO18

int busy = 0;       // lock flag

FILE *fp;           // global FIFO pointer

// Flush old FIFO data
void clear_fifo() {
    char dump[256];

    // Non-blocking flush (consume everything)
    while (fgets(dump, sizeof(dump), fp)) {
        // discard buffer content
    }

    // Reset FIFO pointer
    fclose(fp);
    fp = fopen("/tmp/qrpipe", "r");
}

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

    pinMode(LED_PIN, PWM_OUTPUT);
}

int main() {
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[256];

    wiringPiSetup();
    pinMode(LED_PIN, PWM_OUTPUT);
    pwmWrite(LED_PIN, 0);

    fp = fopen(fifo_path, "r");
    if (!fp) {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Waiting for QR data...\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;

        // Skip if busy
        if (busy == 1)
            continue;

        // Skip "0"
        if (strcmp(buffer, "0") == 0)
            continue;

        printf("Received: %s\n", buffer);

        busy = 1;  // lock

        if (strcmp(buffer, "Razole") == 0) {
            pwm_fade_twice();
        } else if (strcmp(buffer, "0123456789") == 0) {
            blink_twice();
        } else {
            printf("Unknown QR code\n");
        }

        // IMPORTANT: CLEAR all old buffered QR data
        clear_fifo();

        busy = 0; // unlock
    }

    fclose(fp);
    return 0;
}
