#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

// SAFE START ANGLES
int a1 = 90;
int a2 = 30;
int a3 = 140;
int a4 = 140;

// SAFE LIMITS (CHANGE IF NEEDED)
int min1=0,   max1=180;
int min2=0,   max2=180;
int min3=0,   max3=180;
int min4=0,   max4=180;

int angleToPwm(int angle){
    if(angle<0) angle=0;
    if(angle>180) angle=180;
    return 50 + (angle * 200) / 180;
}

void smoothMove(int pin,int *current,int target){
    if(target<*current) (*current)--;
    else if(target>*current) (*current)++;

    pwmWrite(pin, angleToPwm(*current));
    delay(10);
}

int clamp(int x,int min,int max){
    if(x<min) return min;
    if(x>max) return max;
    return x;
}

int main(){

    wiringPiSetupGpio();

    pinMode(SERVO1,PWM_OUTPUT);
    pinMode(SERVO2,PWM_OUTPUT);
    pinMode(SERVO3,PWM_OUTPUT);
    pinMode(SERVO4,PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

    // ------- SAFE START POSITION -------
    pwmWrite(SERVO1, angleToPwm(a1));
    pwmWrite(SERVO2, angleToPwm(a2));
    pwmWrite(SERVO3, angleToPwm(a3));
    pwmWrite(SERVO4, angleToPwm(a4));

    printf("SAFE HOME POSITION SET\n");

    // ------- NETWORK -------
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    char buffer[128];

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    bind(server_fd,(struct sockaddr*)&server,sizeof(server));
    listen(server_fd,1);

    printf("Waiting for GUI connection...\n");

    socklen_t len = sizeof(client);
    client_fd = accept(server_fd,(struct sockaddr*)&client,&len);
    printf("Connected\n");

    while(1){
        memset(buffer,0,sizeof(buffer));
        int n = read(client_fd,buffer,sizeof(buffer)-1);
        if(n<=0) break;

        int t1,t2,t3,t4;
        if(sscanf(buffer,"%d,%d,%d,%d",&t1,&t2,&t3,&t4)==4){

            // clamp to safe limits
            t1=clamp(t1,min1,max1);
            t2=clamp(t2,min2,max2);
            t3=clamp(t3,min3,max3);
            t4=clamp(t4,min4,max4);

            // gradually move
            for(int i=0;i<3;i++){
                smoothMove(SERVO1,&a1,t1);
                smoothMove(SERVO2,&a2,t2);
                smoothMove(SERVO3,&a3,t3);
                smoothMove(SERVO4,&a4,t4);
            }

            printf("Now at: %d %d %d %d\n",a1,a2,a3,a4);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
