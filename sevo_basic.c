#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>

pthread_t t1, t2;

#define SERVO1 19
#define SERVO2 12

int arr[] = {0, 90, 180, 90, 0};

int angleToPwm(int angle)
{
    return 50 + (angle * 200) / 180;
}

void *fun1(void *arg)
{
    int current = angleToPwm(0);

    for (int i = 0; i < 5; i++)
    {
        int target = angleToPwm(arr[i]);

        if (current < target)
        {
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO1, j);
                delay(20);
            }
        }
        else
        {
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO1, j);
                delay(20);
            }
        }
        current = target;
    }
    return NULL;
}

void *fun2(void *arg)
{
    int current = angleToPwm(0);

    for (int i = 0; i < 5; i++)
    {
        int target = angleToPwm(arr[i]);

        if (current < target)
        {
            for (int j = current; j <= target; j++)
            {
                pwmWrite(SERVO2, j);
                delay(20);
            }
        }
        else
        {
            for (int j = current; j >= target; j--)
            {
                pwmWrite(SERVO2, j);
                delay(20);
            }
        }
        current = target;
    }
    return NULL;
}

int main()
{
    wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);   // 50 Hz

    pthread_create(&t1, NULL, fun1, NULL);
    pthread_create(&t2, NULL, fun2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
