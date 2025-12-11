#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *fifo_path = "/tmp/qrpipe";

    // Open FIFO for reading
    FILE *fp = fopen(fifo_path, "r");
    if (fp == NULL) {
        perror("Error opening FIFO");
        return 1;
    }

    printf("Waiting for data from Python...\n");

    char buffer[256];

    // Continuously read lines from FIFO
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("Received: %s", buffer);
        fflush(stdout);
    }

    fclose(fp);
    return 0;
}
