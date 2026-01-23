#include <wiringPi.h>
#include <stdio.h>

#define SERVO1 18
#define SERVO2 19
#define SERVO3 12
#define SERVO4 13

#define MIN_PWM 100
#define MAX_PWM 200

int sweep[5] = {0, 90, 180, 90, 0};

int angleToPwm(int angle)
{
    return MIN_PWM + (angle * (MAX_PWM - MIN_PWM)) / 180;
}

void smoothMove(int pin, int *currentPwm, int targetAngle, int speedDelay)
{
    int targetPwm;

    if (targetAngle < 0)   targetAngle = 0;
    if (targetAngle > 180) targetAngle = 180;

    targetPwm = angleToPwm(targetAngle);

    if (*currentPwm < targetPwm)
    {
        for (int p = *currentPwm; p <= targetPwm; p++)
        {
            pwmWrite(pin, p);
            delay(speedDelay);
        }
    }
    else
    {
        for (int p = *currentPwm; p >= targetPwm; p--)
        {
            pwmWrite(pin, p);
            delay(speedDelay);
        }
    }

    *currentPwm = targetPwm;
}

int main(void)
{
    int speedDelay = 10;

    int pwm1 = 150, pwm2 = 150, pwm3 = 150, pwm4 = 150;

    wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);
    pinMode(SERVO3, PWM_OUTPUT);
    pinMode(SERVO4, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

    pwmWrite(SERVO1, pwm1);
    pwmWrite(SERVO2, pwm2);
    pwmWrite(SERVO3, pwm3);
    pwmWrite(SERVO4, pwm4);

    delay(500);

    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            smoothMove(SERVO1, &pwm1, sweep[i], speedDelay);
            smoothMove(SERVO2, &pwm2, sweep[i], speedDelay);
            smoothMove(SERVO3, &pwm3, sweep[i], speedDelay);
            smoothMove(SERVO4, &pwm4, sweep[i], speedDelay);

            delay(500);
        }
    }
}
