#include <wiringPi.h>
#include <stdio.h>

#define SERVO1 19
#define SERVO2 12
#define SERVO3 18
#define SERVO4 13

int angleToPwm(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    return 50 + (angle * 200) / 180;
}

/* Smooth movement */
void smoothMove(int pin, int *current, int target)
{
    if (*current < target)
    {
        for (int p = *current; p <= target; p++)
        {
            pwmWrite(pin, p);
            delay(20);
        }
    }
    else
    {
        for (int p = *current; p >= target; p--)
        {
            pwmWrite(pin, p);
            delay(20);
        }
    }
    *current = target;
}

int main()
{
    int servo, angle;
    int currentPwm[4] = {150, 150, 150, 150}; // start 

    wiringPiSetupGpio();

    pinMode(SERVO1, PWM_OUTPUT);
    pinMode(SERVO2, PWM_OUTPUT);
    pinMode(SERVO3, PWM_OUTPUT);
    pinMode(SERVO4, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);   // 50 Hz

    // Move all servos to center
    pwmWrite(SERVO1, 150);
    pwmWrite(SERVO2, 150);
    pwmWrite(SERVO3, 150);
    pwmWrite(SERVO4, 150);

    while (1)
    {
        printf("\n--- SERVO CALIBRATION ---\n");
        printf("1) SERVO1\n2) SERVO2\n3) SERVO3\n4) SERVO4\n0) Exit\n");
        printf("Select servo: ");
        scanf("%d", &servo);

        if (servo == 0) break;
        if (servo < 1 || servo > 4)
        {
            printf("Invalid servo!\n");
            continue;
        }

        printf("Enter angle (0180): ");
        scanf("%d", &angle);

        int target = angleToPwm(angle);

        switch (servo)
        {
            case 1: smoothMove(SERVO1, &currentPwm[0], target); break;
            case 2: smoothMove(SERVO2, &currentPwm[1], target); break;
            case 3: smoothMove(SERVO3, &currentPwm[2], target); break;
            case 4: smoothMove(SERVO4, &currentPwm[3], target); break;
        }

        printf("Servo %d ? Angle %d? PWM %d\n", servo, angle, target);
    }

    return 0;
}
