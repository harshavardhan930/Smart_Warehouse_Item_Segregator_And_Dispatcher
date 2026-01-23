#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define SERVO1 19
#define SERVO2 18
#define SERVO3 12
#define SERVO4 13

/* Angle arrays (different actions) */
int angles1[] = {0, 180, 90, 0};
int angles2[] = {90, 0, 180, 90};
int angles3[] = {45, 90, 180, 90, 45};
int angles4[] = {0, 30, 180, 120, 0};

int len1 = 4;
int len2 = 4;
int len3 = 5;
int len4 = 5;

int angleToPwm(int angle)
{
    return 100 + (angle * 100) / 180;
}


/* Thread argument structure */
typedef struct {
    int pin;
    int *angles;
    int length;
    const char *name;
} servo_t;

/* Servo thread */
void* servoTask(void* arg)
{
    servo_t *s = (servo_t*)arg;
    int currentAngle = s->angles[0];

    pinMode(s->pin, PWM_OUTPUT);
    pwmWrite(s->pin, angleToPwm(currentAngle));
    delay(300);

    for (int i = 1; i < s->length; i++)
    {
        int target = s->angles[i];

        if (target > currentAngle)
        {
            for (int a = currentAngle; a <= target; a++)
            {
                pwmWrite(s->pin, angleToPwm(a));
                delay(20);
            }
        }
        else
        {
            for (int a = currentAngle; a >= target; a--)
            {
                pwmWrite(s->pin, angleToPwm(a));
                delay(20);
            }
        }

        currentAngle = target;
        delay(400);
    }

    printf("%s sequence complete\n", s->name);
    pthread_exit(NULL);
}

int main()
{
    wiringPiSetupGpio();

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);   // 50Hz

    pthread_t t1, t2, t3, t4;

    servo_t s1 = {SERVO1, angles1, len1, "SERVO1"};
    servo_t s2 = {SERVO2, angles2, len2, "SERVO2"};
    servo_t s3 = {SERVO3, angles3, len3, "SERVO3"};
    servo_t s4 = {SERVO4, angles4, len4, "SERVO4"};

    pthread_create(&t1, NULL, servoTask, &s1);
    pthread_create(&t2, NULL, servoTask, &s2);
    pthread_create(&t3, NULL, servoTask, &s3);
    pthread_create(&t4, NULL, servoTask, &s4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    printf("All servo actions completed\n");
    return 0;
}
