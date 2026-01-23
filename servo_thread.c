#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>

/* Thread handles */
pthread_t t1, t2, t3, t4;

/* Servo pins (BCM numbering) */
#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

/* Individual angle arrays */
int arr1[] = {0, 45, 90, 45, 0};
int arr2[] = {90, 135, 180, 135, 90};
int arr3[] = {0, 90, 180, 90, 0};
int arr4[] = {180, 120, 60, 120, 180};

#define STEPS 5

/* Angle ? PWM conversion */
int angleToPwm(int angle)
{
    return 50 + (angle * 200) / 180;
}

/* ---------------- SERVO 1 ---------------- */
void *fun1(void *arg)
{
    int current = angleToPwm(arr1[0]);1

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(arr1[i]);

        if (current < target)
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO1, j);
                delay(20);
            }
        else
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO1, j);
                delay(20);
            }

        current = target;
    }
    return NULL;
}

/* ---------------- SERVO 2 ---------------- */
void *fun2(void *arg)
{
    int current = angleToPwm(arr2[0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(arr2[i]);

        if (current < target)
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO2, j);
                delay(20);
            }
        else
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO2, j);
                delay(20);
            }

        current = target;
    }
    return NULL;
}

/* ---------------- SERVO 3 ---------------- */
void *fun3(void *arg)
{
    int current = angleToPwm(arr3[0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(arr3[i]);

        if (current < target)
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO3, j);
                delay(20);
            }
        else
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO3, j);
                delay(20);
            }

        current = target;
    }
    return NULL;
}

/* ---------------- SERVO 4 ---------------- */
void *fun4(void *arg)
{
    int current = angleToPwm(arr4[0]);

    for (int i = 0; i < STEPS; i++)
    {
        int target = angleToPwm(arr4[i]);

        if (current < target)
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO4, j);
                delay(20);
            }
        else
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO4, j);
                delay(20);
            }

        current = target;
    }
    return NULL;
}

/* ---------------- MAIN ---------------- */
int main()
{
    wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);
    pinMode(SERVO3, PWM_OUTPUT);
    pinMode(SERVO4, PWM_OUTPUT);

    /* Shared PWM hardware setup */
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);   // 50 Hz

    pthread_create(&t1, NULL, fun1, NULL);
    pthread_create(&t2, NULL, fun2, NULL);
    pthread_create(&t3, NULL, fun3, NULL);
    pthread_create(&t4, NULL, fun4, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}
