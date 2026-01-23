#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

              

#define LED_PIN 1
#define SWITCH_PIN  25 

int busy = 0;

int choise;

int paused = 0;        // 0 = running, 1 = paused
int lastBtn = HIGH;    // for edge detection



    
    
void input_mode();
void output_mode();

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

    flush_fifo();
}

void blink_twice() {
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < 2; i++) {
        digitalWrite(LED_PIN, HIGH); delay(300);
        digitalWrite(LED_PIN, LOW);  delay(300);
    }
    pinMode(LED_PIN, PWM_OUTPUT);

    flush_fifo();
}



int main() {
		 wiringPiSetup();
		pinMode(LED_PIN, PWM_OUTPUT);
		 pinMode(SWITCH_PIN, INPUT);   // Switch pin input
    pullUpDnControl(SWITCH_PIN, PUD_UP);  // Enable pull-up for stable input
		
		while(1){
			
		printf("1)INPUT MODE\n2)OUTPUT MODE\n3)EXIT\nSelect your option:");
		scanf("%d",&choise);
		while(getchar()!='\n');
		
		switch(choise){
			case 1:
				printf("input mode started\n");
				input_mode();
				break;
			case 2:
				printf("output mode started\n");
				//pwm_fade_twice();
				output_mode();
				break;
			default:
				printf("enter valid choise!\n");
				break;
		}
		
	}
    return 0;
}

void output_mode(){
	choise = 0;
	FILE *pd;
	char buffer[120];
	char line[120];
	char date[120];
	char time[120];
	char product[120];
	
	pd=fopen("data.txt","r");
	
	printf("Enter Product name:");
	fgets(buffer,sizeof(buffer),stdin);
	buffer[strcspn(buffer,"\n")]='\0';
	getchar();
	fgets(line,sizeof(line),pd);
	getchar();
	while(fgets(line,sizeof(line),pd)){
		sscanf(line,"%s %s %s",date,time,product);
		printf("%s %s %s\n",date,time,product);
		if((strcmp(product,buffer))==0){
			blink_twice();
		}
	}
	fclose(pd);
	
}


void input_mode() {
    int sw = digitalRead(SWITCH_PIN);
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    const char *fifo_path = "/tmp/qrpipe";
    char buffer[50];

    while (1) {
        FILE *fp = fopen(fifo_path, "r");
        if (!fp) perror("FIFO open error");

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

        printf("%s Received: %s\n", ctime(&t), buffer);
        busy = 1;

        if (strcmp(buffer, "Razole") == 0) {
            FILE *da = fopen("data.txt", "a");
            fprintf(da, "%02d-%02d-%04d  %02d:%02d:%02d Razole\n",
                tm_info->tm_mday,
                tm_info->tm_mon + 1,
                tm_info->tm_year + 1900,
                tm_info->tm_hour,
                tm_info->tm_min,
                tm_info->tm_sec);
            fclose(da);

            printf("data logged successfully!\n");
            flush_fifo();
            pwm_fade_twice();
        }

        else if (strcmp(buffer, "0123456789") == 0) {
            FILE *da = fopen("data.txt", "a");
            fprintf(da, "%02d-%02d-%04d  %02d:%02d:%02d jaggannapeta\n",
                tm_info->tm_mday,
                tm_info->tm_mon + 1,
                tm_info->tm_year + 1900,
                tm_info->tm_hour,
                tm_info->tm_min,
                tm_info->tm_sec);
            fclose(da);

            printf("data logged successfully!\n");

            flush_fifo();
            blink_twice();
        }
        else {
            printf("Unknown QR\n");
        }

        busy = 0;
    }
}
