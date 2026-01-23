#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>


//------------------------------------------------------------------------------------servo--v


#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

#define SERVO1_REST 90
#define SERVO2_REST 170
#define SERVO3_REST 140
#define SERVO4_REST 140


#define MODES 8
#define SERVOS 5
#define STEPS 7

int speed1 = 20;
int ledPin = 22;
int ir_sensor = 26;

int ir_error=0;

pthread_t t1, t2, t3, t4 ,t5;

#define THREAD_COUNT 5

pthread_barrier_t barrier;

#define SERVO_FILE "servo_pos.csv"


//-----------------------------------servo present values
int servo_1=90;
int servo_2=170;
int servo_3=140;
int servo_4=140;
//-----------------------------------servo present values




/* Mode-based angle table */
int modes[MODES][SERVOS][STEPS] =
{
    // MODE 1
    {
        {90,172,172,5,5,5,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,0,1,1,1,0,0}
    },

    // MODE 2
    {
        {90,172,172,20,20,20,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,0,1,1,1,0,0}
    },

    // MODE 3
    {
        {90,172,172,37,37,37,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,0,1,1,1,0,0}
    },

    // MODE 4
    {
        {90,172,172,53,53,53,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,0,1,1,1,0,0}
    },

    // MODE 11
    {
        {90,5,5,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,0,1,1,1,0,0}
    },
    // MODE 12
    {
        {90,20,20,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,0,1,1,1,0,0}
    },	
    // MODE 13
    {
        {90,37,37,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,0,1,1,1,0,0}
    },
    // MODE 14
    {
        {90,53,53,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,0,1,1,1,0,0}
    },
};

int selectedMode;

/* Angle ? PWM */
int angleToPwm(int angle)
{
    return 50 + (angle * 200) / 180;
}

void save_servo_position()
{
    FILE *fp = fopen(SERVO_FILE, "w");
    if (!fp) {
        perror("Servo file write error");
        return;
    }

    fprintf(fp, "servo1,servo2,servo3,servo4\n");
    fprintf(fp, "%d,%d,%d,%d\n", servo_1, servo_2, servo_3, servo_4);

    fclose(fp);
}

/* ---------- SERVO FUNCTIONS ---------- */

void *servo1(void *arg)
{
    int current = angleToPwm(modes[selectedMode][0][0]);

    for (int i = 0; i < STEPS; i++)
    {
		printf("count:%d\n",i);
        int target = angleToPwm(modes[selectedMode][0][i]);
        for (int j = current; j != target; j += (j < target ? 1 : -1))
        {
			if(ir_error==0){
            pwmWrite(SERVO1, j);
            servo_1=j;
            save_servo_position();
            delay(speed1);
		}else{
            
			continue;
		}
		
        }
        current = target;
        pthread_barrier_wait(&barrier);
        
    }
    
    
    return NULL;
}

void *servo2(void *arg)
{
    int current = angleToPwm(modes[selectedMode][1][0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(modes[selectedMode][1][i]);
        for (int j = current; j != target; j += (j < target ? 1 : -1))
        {
			if(ir_error==0){
            pwmWrite(SERVO2, j);
            servo_2=j;
            save_servo_position();
            delay(speed1);
		}else{

			continue;
		}
        }
        current = target;
        
         pthread_barrier_wait(&barrier);
    }

    return NULL;
}

void *servo3(void *arg)
{
    int current = angleToPwm(modes[selectedMode][2][0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(modes[selectedMode][2][i]);
        for (int j = current; j != target; j += (j < target ? 1 : -1))
        {
			if(ir_error==0){

            pwmWrite(SERVO3, j);
            servo_3=j;
            save_servo_position();
            delay(speed1);
		}else{
            
			continue;
		}
        }
        current = target;
        pthread_barrier_wait(&barrier);
    }
    
    return NULL;
}

void *servo4(void *arg)
{
    int current = angleToPwm(modes[selectedMode][3][0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(modes[selectedMode][3][i]);
        for (int j = current; j != target; j += (j < target ? 1 : -1))
        {
			if(ir_error==0){
            pwmWrite(SERVO4, j);
            servo_4=j;
            save_servo_position();

            delay(speed1);
		}else{

			continue;
		}
        }
        current = target;

        pthread_barrier_wait(&barrier);
    }
    
    
    return NULL;
}

void *magnet(void *arg)
{
	pinMode(ir_sensor,INPUT);

	for(int i =0;i<STEPS;i++){
		int ir_check = digitalRead(ir_sensor);
		int a = modes[selectedMode][4][i];
		if(a==1){
			digitalWrite(ledPin, HIGH);
		}else{
			digitalWrite(ledPin, LOW);
		}
		if (i != 5 && i != 6){
			//object checking 
			if(!ir_check==a){
				printf("object detected\n");
				
				
			}else{
				printf("errorrrr rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr object not detected\n");
				ir_error=1;
                pthread_barrier_wait(&barrier);
                continue;
            }
		}
		
		pthread_barrier_wait(&barrier);
	}
	
	return NULL;
	
}

void fun(){
    pthread_create(&t1, NULL, servo1, NULL);
    pthread_create(&t2, NULL, servo2, NULL);
    pthread_create(&t3, NULL, servo3, NULL);
    pthread_create(&t4, NULL, servo4, NULL);
    pthread_create(&t5, NULL, magnet, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
   
}
//------------------------------------------------------------------------------------servo--^
           
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

void go_to_rest_position_1()
{
    int target1 = angleToPwm(SERVO1_REST);
    int target2 = angleToPwm(SERVO2_REST);
    int target3 = angleToPwm(SERVO3_REST);
    int target4 = angleToPwm(SERVO4_REST);

    int done1 = 0, done2 = 0, done3 = 0, done4 = 0;

    while (!(done1 && done2 && done3 && done4))
    {
        if (!done1)
        {
            if (servo_1 < target1) servo_1++;
            else if (servo_1 > target1) servo_1--;
            else done1 = 1;

            pwmWrite(SERVO1, servo_1);
        }

        if (!done2)
        {
            if (servo_2 < target2) servo_2++;
            else if (servo_2 > target2) servo_2--;
            else done2 = 1;

            pwmWrite(SERVO2, servo_2);
        }

        if (!done3)
        {
            if (servo_3 < target3) servo_3++;
            else if (servo_3 > target3) servo_3--;
            else done3 = 1;

            pwmWrite(SERVO3, servo_3);
        }

        if (!done4)
        {
            if (servo_4 < target4) servo_4++;
            else if (servo_4 > target4) servo_4--;
            else done4 = 1;

            pwmWrite(SERVO4, servo_4);
        }

        delay(50);   // ?? controls smoothness (increase = slower)
    }


    ir_error = 0;

    save_servo_position();
}



void load_servo_position()
{
    FILE *fp = fopen(SERVO_FILE, "r");
    if (!fp) {
        printf("Servo position file not found. Using defaults.\n");
        servo_1 = angleToPwm(SERVO1_REST);
        servo_2 = angleToPwm(SERVO2_REST);
        servo_3 = angleToPwm(SERVO3_REST);
        servo_4 = angleToPwm(SERVO4_REST);
        return;
    }

    char line[128];

    // Skip header
    fgets(line, sizeof(line), fp);

    // Read values
    if (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%d,%d,%d",
               &servo_1, &servo_2, &servo_3, &servo_4);
    }

    fclose(fp);

    printf("Loaded servo positions: %d %d %d %d\n",
           servo_1, servo_2, servo_3, servo_4);
}






int choise;

int main() {
	wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);
    pinMode(SERVO3, PWM_OUTPUT);	
    pinMode(SERVO4, PWM_OUTPUT);
	

	wiringPiSetupGpio(); 
    pinMode(ledPin, OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192); 
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT);
    load_servo_position();
    go_to_rest_position_1();
		
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
	   pthread_barrier_destroy(&barrier);
    return 0;
}

char product[120];

void output_mode()
{
	printf("enter product:");
	scanf("%s",product);
	
	product[strcspn(product, "\n")] = 0;

        if (strcmp(product, "CDAC") == 0) {
			selectedMode=4;
			fun();
        }

        else if (strcmp(product, "Amalapuram") == 0) {
		
			selectedMode=5;
			fun();
        }
        else {
            printf("Unknown QR\n");
        }
	
}


char buffer[120];
void input_mode() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    const char *fifo_path = "/tmp/qrpipe";


    while (1) {
        go_to_rest_position_1();
        FILE *fp = fopen(fifo_path, "r");
        if (!fp) perror("FIFO open error");

        printf("Waiting for QR data...\n");

        if (!fgets(buffer, sizeof(buffer), fp)) {
            fclose(fp);
            continue;
        }

        fclose(fp);

        buffer[strcspn(buffer, "\n")] = 0;
        

        if (strcmp(buffer, "0") == 0)
            continue;

        printf("%s Received: %s\n", ctime(&t), buffer);

        if (strcmp(buffer, "CDAC") == 0) {
			flush_fifo();
			selectedMode=0;
			fun();
			flush_fifo();
        }

        else if (strcmp(buffer, "Amalapuram") == 0) {
			
			selectedMode=1;
			fun();
			flush_fifo();
        }	
        else if (strcmp(buffer, "Hyderabad") == 0) {
			selectedMode=2;
			fun();
			flush_fifo();
        }
        else if (strcmp(buffer, "Visakhapatnam") == 0) {
			selectedMode=3;
			fun();
			flush_fifo();
        }
        else {
            printf("Unknown QR\n");
        }


    }
}


