#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#define LED_PIN 1
FILE *fp;
int busy = 0;

void pwm_fade_twice() {
    for (int n = 0; n < 2; n++) {
        for (int i = 0; i < 1024; i++) {
            pwmWrite(LED_PIN, i); delay(1);
        }
        for (int i = 1023; i >= 0; i--) {
            pwmWrite(LED_PIN, i); delay(1);
        }
    }
    sleep(5);
}

void blink_twice() {
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH); delay(300);
        digitalWrite(LED_PIN, LOW);  delay(300);
    }
    pinMode(LED_PIN, PWM_OUTPUT);
    sleep(5);
}

int main() {
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[50];

    wiringPiSetup();
    pinMode(LED_PIN, PWM_OUTPUT);

    

    while (1) {
		fp = fopen(fifo_path, "r");
    if (!fp) { perror("FIFO open error"); return 1; }

    printf("Waiting for QR data...\n");
        // read one line ONLY
        if (!fgets(buffer, sizeof(buffer), fp))
            continue;

        buffer[strcspn(buffer, "\n")] = 0;

        // If action is still running ? ignore new inputs
        if (busy == 1)
            continue;

        // Ignore "0"
        if (strcmp(buffer, "0") == 0)
            continue;

        printf("Received: %s\n", buffer);

        busy = 1;  // lock

        if (strcmp(buffer, "Razole") == 0) {
			fclose(fp);
            pwm_fade_twice();
        }
        else if (strcmp(buffer, "0123456789") == 0) {
			fclose(fp);
            blink_twice();
        }
        else {
            printf("Unknown QR\n");
        }

        busy = 0; // unlock after action
        fclose(fp);
    }

    fclose(fp);
    return 0;
}
