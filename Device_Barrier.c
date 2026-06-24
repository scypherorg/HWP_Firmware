#include <stdio.h>
#include "pico/stdlib.h"
#include "Net.h"
#include "Servo.h"
#include "LDR.h"
#include "LED.h"
#define LDR_PIN 27
#define LDR_LED_PIN 26
#define USER_SERVO_PIN 2
#define USER_LED_PIN 12
#define BRAZIL_SERVO_PIN 6
#define BRAZIL_LED_PIN 10 //Tube 1
#define JAPAN_SERVO_PIN 4
#define JAPAN_LED_PIN 13 //Tube 2
#define NETHERLANDS_SERVO_PIN 8
#define NETHERLANDS_LED_PIN 11 //Tube 3
#define MODE_IDLE 0
#define MODE_START_DELAY 1
#define MODE_LOWERING 2
#define MODE_RESET_DELAY 3
#define TIME_BRAZIL 2000
#define TIME_NETHERLANDS 3000
#define TIME_JAPAN 4000
#define TIME_USER_MIN 1000
#define TIME_USER_MAX 5000
#define TIME_RESET_DELAY 5000
#define TIME_START_DELAY 5000
#define USER_MAX_ACTIVATION 2200
#define LED_BLINK_COUNT 30
static uint8_t mode = 0;
static absolute_time_t timerStart;
static uint32_t userTarget;
static uint8_t LDR_ID;
void raiseAll()
{
    Servo_SetAngle(USER_SERVO_PIN, 180);
    LED_SetEnabled(USER_LED_PIN, false);
    Servo_SetAngle(BRAZIL_SERVO_PIN, 180);
    LED_SetEnabled(BRAZIL_LED_PIN, false);
    Servo_SetAngle(JAPAN_SERVO_PIN, 180);
    LED_SetEnabled(JAPAN_LED_PIN, false);
    Servo_SetAngle(NETHERLANDS_SERVO_PIN, 180);
    LED_SetEnabled(NETHERLANDS_LED_PIN, false);
    mode = MODE_IDLE;
}
void startLowering()
{
    timerStart = get_absolute_time();
    mode = MODE_START_DELAY;
}
void lowerBarrier()
{
    timerStart = get_absolute_time();
    userTarget = TIME_USER_MIN;
    mode = MODE_LOWERING;
}
void OnRecieve(uint8_t* data, uint8_t length)
{
    if(data[0] == 'u')
    {
        if(length > 1)
        {
            if(data[1] == 'u')
                Servo_SetAngle(USER_SERVO_PIN, 180);
            else if(data[1] == 'b')
                Servo_SetAngle(BRAZIL_SERVO_PIN, 180);
            else if(data[1] == 'j')
                Servo_SetAngle(JAPAN_SERVO_PIN, 180);
            else if(data[1] == 'n')
                Servo_SetAngle(NETHERLANDS_SERVO_PIN, 180);
            else
                raiseAll();
            return;
        }
        raiseAll();
        return;
    }
    if(data[0] == 'l')
    {
        if(length > 1)
        {
            if(data[1] == 'u')
            {
                Servo_SetAngle(USER_SERVO_PIN, 0);
                LED_SetEnabled(USER_LED_PIN, true);
            }
            else if(data[1] == 'b')
            {
                Servo_SetAngle(BRAZIL_SERVO_PIN, 0);
                LED_SetEnabled(BRAZIL_LED_PIN, true);
            }
            else if(data[1] == 'j')
            {
                Servo_SetAngle(JAPAN_SERVO_PIN, 0);
                LED_SetEnabled(JAPAN_LED_PIN, true);
            }
            else if(data[1] == 'n')
            {
                Servo_SetAngle(NETHERLANDS_SERVO_PIN, 0);
                LED_SetEnabled(NETHERLANDS_LED_PIN, true);
            }
            else
            {
                Servo_SetAngle(USER_SERVO_PIN, 0);
                Servo_SetAngle(BRAZIL_SERVO_PIN, 0);
                Servo_SetAngle(JAPAN_SERVO_PIN, 0);
                Servo_SetAngle(NETHERLANDS_SERVO_PIN, 0);
            }
            return;
        }
        lowerBarrier();
        return;
    }
    if(data[0] == 's')
        startLowering();
}
void Barrier_Start()
{
    Net_SetCallback(OnRecieve);
    LED_Start(LDR_LED_PIN, 1);
    LED_SetEnabled(LDR_LED_PIN, true);
    LDR_ID = LDR_Start(LDR_PIN);
    Servo_Start(USER_SERVO_PIN);
    LED_Start(USER_LED_PIN, 1);
    Servo_Start(BRAZIL_SERVO_PIN);
    LED_Start(BRAZIL_LED_PIN, 1);
    Servo_Start(JAPAN_SERVO_PIN);
    LED_Start(JAPAN_LED_PIN, 1);
    Servo_Start(NETHERLANDS_SERVO_PIN);
    LED_Start(NETHERLANDS_LED_PIN, 1);
    raiseAll();
}
void Barrier_Update()
{
    Net_Update();
    static bool pressed = false;
    switch(mode)
    {
        case MODE_START_DELAY:
            if(absolute_time_diff_us(timerStart, get_absolute_time()) / 1000 > TIME_START_DELAY)
                lowerBarrier();
            break;
        case MODE_LOWERING:
            uint32_t elapsed = absolute_time_diff_us(timerStart, get_absolute_time()) / 1000;
            if(LDR_Triggered(LDR_ID))
            {
                if(!pressed && elapsed < userTarget)
                {
                    pressed = true;
                    if(userTarget < TIME_USER_MAX)
                        userTarget += (TIME_USER_MAX - TIME_USER_MIN) / USER_MAX_ACTIVATION;
                }
            }
            else
                pressed = false;
            if(elapsed < TIME_BRAZIL)
            {
                Servo_SetAngle(BRAZIL_SERVO_PIN, 180 - (elapsed * 180 / TIME_BRAZIL));
                LED_SetEnabled(BRAZIL_LED_PIN, (elapsed*LED_BLINK_COUNT/TIME_BRAZIL)%2);
            }
            else
            {
                Servo_SetAngle(BRAZIL_SERVO_PIN, 0);
                LED_SetEnabled(BRAZIL_LED_PIN, true);
            }
            if(elapsed < TIME_JAPAN)
            {
                Servo_SetAngle(JAPAN_SERVO_PIN, 180 - (elapsed * 180 / TIME_JAPAN));
                LED_SetEnabled(JAPAN_LED_PIN, (elapsed*LED_BLINK_COUNT/TIME_JAPAN)%2);
            }
            else
            {
                Servo_SetAngle(JAPAN_SERVO_PIN, 0);
                LED_SetEnabled(JAPAN_LED_PIN, true);
            }
            if(elapsed < TIME_NETHERLANDS)
            {
                Servo_SetAngle(NETHERLANDS_SERVO_PIN, 180 - (elapsed * 180 / TIME_NETHERLANDS));
                LED_SetEnabled(NETHERLANDS_LED_PIN, (elapsed*LED_BLINK_COUNT/TIME_NETHERLANDS)%2);
            }
            else
            {
                Servo_SetAngle(NETHERLANDS_SERVO_PIN, 0);
                LED_SetEnabled(NETHERLANDS_LED_PIN, true);
            }
            if(elapsed < userTarget)
            {
                Servo_SetAngle(USER_SERVO_PIN, 180 - (elapsed * 180 / userTarget));
                LED_SetEnabled(USER_LED_PIN, (elapsed*LED_BLINK_COUNT/userTarget)%2);
            }
            else
            {
                Servo_SetAngle(USER_SERVO_PIN, 0);
                LED_SetEnabled(USER_LED_PIN, true);
            }
            if(elapsed > userTarget && elapsed > TIME_BRAZIL && elapsed > TIME_JAPAN && elapsed > TIME_NETHERLANDS)
            {
                mode = MODE_RESET_DELAY;
                timerStart = get_absolute_time();
                printf("User Delay: %dms\n", userTarget);
            }
            break;
        case MODE_RESET_DELAY:
            if(absolute_time_diff_us(timerStart, get_absolute_time()) / 1000 > TIME_RESET_DELAY)
                raiseAll();
            break;
        default:
            break;
    }
}