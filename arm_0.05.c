#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_PIN 1

int busy = 0;

void flush_fifo() {
    int fd = open("/tmp/qrpipe", O_RDONLY | O_NONBLOCK);
    if (fd < 0) return;

    char dump[128];
    while (read(fd, dump, sizeof(dump)) > 0) {
        // discard silently
    }

    close(fd);
}



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
    flush_fifo();
}

void blink_twice() {
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH); delay(300);
        digitalWrite(LED_PIN, LOW);  delay(300);
    }
    pinMode(LED_PIN, PWM_OUTPUT);
    sleep(5);
    flush_fifo();
}

int main() {
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[50];

    wiringPiSetup();
    pinMode(LED_PIN, PWM_OUTPUT);

    while (1) {

        // Always open FIFO once per cycle
        FILE *fp = fopen(fifo_path, "r");
        if (!fp) { perror("FIFO open error"); return 1; }

        printf("Waiting for QR data...\n");

        if (!fgets(buffer, sizeof(buffer), fp)) {
            fclose(fp);
            continue;
        }

        fclose(fp);

        buffer[strcspn(buffer, "\n")] = 0;

        if (busy == 1)
            continue;

        if (strcmp(buffer, "0") == 0)
            continue;

        printf("Received: %s\n", buffer);

        busy = 1;

        if (strcmp(buffer, "Razole") == 0) {
			flush_fifo();
            pwm_fade_twice();
        }
        else if (strcmp(buffer, "0123456789") == 0) {
			flush_fifo();
            blink_twice();
        }
        else {
            printf("Unknown QR\n");
        }

        busy = 0;
    }

    return 0;
}
