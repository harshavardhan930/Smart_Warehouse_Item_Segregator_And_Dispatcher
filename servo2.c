#include <wiringPi.h>
#include <stdio.h>

#define SERVO_PIN 19      // BCM 18 (hardware PWM)

#define MIN_PWM 100       // 1.0 ms pulse
#define MAX_PWM 200       // 2.0 ms pulse

int sweep[5] = {0, 90, 180, 90, 0};

int angleToPwm(int angle)
{
    return MIN_PWM + (angle * (MAX_PWM - MIN_PWM)) / 180;
}

int main(void)
{
    int targetAngle;
    int targetPwm;
    int speedDelay = 10;          // movement speed (ms)
    static int currentPwm = 150; // start at 

    wiringPiSetupGpio();
    pinMode(SERVO_PIN, PWM_OUTPUT);

    // 50 Hz servo PWM
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

    // Initial position
    pwmWrite(SERVO_PIN, currentPwm);
    delay(500);

    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            targetAngle = sweep[i];

            // Safety clamp
            if (targetAngle < 0)   targetAngle = 0;
            if (targetAngle > 180) targetAngle = 180;

            targetPwm = angleToPwm(targetAngle);

            // Smooth movement
            if (currentPwm < targetPwm)
            {
                for (int p = currentPwm; p <= targetPwm; p++)
                {
                    pwmWrite(SERVO_PIN, p);
                    delay(speedDelay);
                }
            }
            else
            {
                for (int p = currentPwm; p >= targetPwm; p--)
                {
                    pwmWrite(SERVO_PIN, p);
                    delay(speedDelay);
                }
            }

            currentPwm = targetPwm;
            delay(500);   // hold position
        }
    }

    return 0;
}
