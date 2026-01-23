#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>


#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

#define MODES 8
#define SERVOS 5
#define STEPS 7

int speed1 = 20;
int ledPin = 22;

pthread_t t1, t2, t3, t4 ,t5;

#define THREAD_COUNT 5

pthread_barrier_t barrier;

/* Mode-based angle table */
int modes[MODES][SERVOS][STEPS] =
{
    // MODE 1
    {
        {90,172,172,5,5,5,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,1,1,1,1,0,0}
    },

    // MODE 2
    {
        {90,172,172,20,20,20,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,1,1,1,1,0,0}
    },

    // MODE 3
    {
        {90,172,172,37,37,37,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,1,1,1,1,0,0}
    },

    // MODE 4
    {
        {90,172,172,53,53,53,90},
        {170,173,149,149,159,119,170},
        {140,50,72,72,36,110,140},
        {140,140,120,120,102,136,140},
        {0,1,1,1,1,0,0}
    },

    // MODE 11
    {
        {90,5,5,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,1,1,1,1,0,0}
    },
    // MODE 12
    {
        {90,20,20,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,1,1,1,1,0,0}
    },	
    // MODE 13
    {
        {90,37,37,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,1,1,1,1,0,0}
    },
    // MODE 14
    {
        {90,53,53,170,170,170,90},
        {170,175,125,125,180,180,170},
        {140,36,90,90,46,90,140},
        {140,108,108,108,138,140,140},
        {0,1,1,1,1,0,0}
    },
};

int selectedMode;

/* Angle ? PWM */
int angleToPwm(int angle)
{
    return 50 + (angle * 200) / 180;
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
            pwmWrite(SERVO1, j);
            delay(speed1);
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
            pwmWrite(SERVO2, j);
            delay(speed1);
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
            pwmWrite(SERVO3, j);
            delay(speed1);
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
            pwmWrite(SERVO4, j);
            delay(speed1);
        }
        current = target;

        pthread_barrier_wait(&barrier);
    }
    
    
    return NULL;
}

void *magnet(void *arg)
{


	for(int i =0;i<STEPS;i++){
		int a = modes[selectedMode][4][i];
		if(a==1){
			digitalWrite(ledPin, HIGH);
		}else{
			digitalWrite(ledPin, LOW);
		}
		
		pthread_barrier_wait(&barrier);
	}
	return NULL;
	
}

/* ---------- MAIN ---------- */

int main()
{
	
	while(1){
    printf("Select Mode (1 - 5): ");
    scanf("%d", &selectedMode);
    selectedMode--;  // convert to 0-based

    if (selectedMode < 0 || selectedMode >= MODES)
    {
        printf("Invalid mode!\n");
        return 0;
    }

    wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);
    pinMode(SERVO3, PWM_OUTPUT);	
    pinMode(SERVO4, PWM_OUTPUT);
	

	wiringPiSetupGpio(); 
    pinMode(ledPin, OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192); // 50 Hz
   // sem_init(&sem, 0, 0);
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT);

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
    
    pthread_barrier_destroy(&barrier);
}

    return 0;
}
