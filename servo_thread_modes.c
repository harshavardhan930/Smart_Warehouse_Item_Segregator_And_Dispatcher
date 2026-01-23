#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>

#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

#define MODES 5
#define SERVOS 4
#define STEPS 8

pthread_t t1, t2, t3, t4;

/* Mode-based angle table */
int modes[MODES][SERVOS][STEPS] =
{
    // MODE 1
    {
        {90, 170, 170, 170, 170,170,25,90},
        {30, 30, 180, 180, 180,150,170,30},
        {140, 140, 140, 50,49,90,40,140},
        {140,140,120,120,140,140,100,140}
    },

    // MODE 2
    {
        {90, 0, 90, 0, 90},
        {0, 90, 0, 90, 0},
        {180, 90, 0, 90, 180},
        {60, 120, 180, 120, 60}
    },

    // MODE 3
    {
        {0, 30, 60, 90, 120},
        {120, 90, 60, 30, 0},
        {45, 90, 135, 90, 45},
        {180, 150, 120, 90, 60}
    },

    // MODE 4
    {
        {180, 90, 0, 90, 180},
        {0, 45, 90, 135, 180},
        {90, 45, 0, 45, 90},
        {60, 90, 120, 150, 180}
    },

    // MODE 5
    {
        {90, 0, 0, 120, 150},
        {30, 170, 170, 60, 30},
        {140, 40, 90, 60, 0},
        {140, 100, 900, 140, 180}
    }
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
        int target = angleToPwm(modes[selectedMode][0][i]);
        for (int j = current; j != target; j += (j < target ? 1 : -1))
        {
            pwmWrite(SERVO1, j);
            delay(40);
        }
        current = target;
        
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
            delay(40);
        }
        current = target;
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
            delay(40);
        }
        current = target;
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
            delay(40);
        }
        current = target;
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

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192); // 50 Hz

    pthread_create(&t1, NULL, servo1, NULL);
    pthread_create(&t2, NULL, servo2, NULL);
    pthread_create(&t3, NULL, servo3, NULL);
    pthread_create(&t4, NULL, servo4, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}

    return 0;
}
