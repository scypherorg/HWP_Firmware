#include <stdio.h>
#include "pico/stdlib.h"
#include "Net.h"
#include "Motor.h"
#include "LDR.h"
#define PIN_MOTOR1 3
#define PIN_MOTOR2 4
#define MOTOR_SPEEDUP 30
#define MOTOR_SPEEDDOWN 20
#define MODE_UP 0
#define MODE_READY 1
#define MODE_DISPENSE 2
#define MODE_DOWN 3
#define MODE_WAITING 4
uint8_t LDR_ID;
static uint8_t mode = 0;
static absolute_time_t timerStart;
void goUp()
{
    Motor_SetSpeed(MOTOR_SPEEDUP);
    Motor_Forward();
    mode = MODE_UP;
    printf("Going up\n");
}
void goDown()
{
    Motor_SetSpeed(MOTOR_SPEEDDOWN);
    Motor_Backward();
    timerStart = get_absolute_time();
    mode = MODE_DOWN;
    printf("Going down\n");
}
void dispense()
{
    if(mode != MODE_READY)
        return;
    Motor_SetSpeed(100);
    Motor_Forward();
    timerStart = get_absolute_time();
    mode = MODE_DISPENSE;
    printf("Dispensing\n");
}
void OnReceive(uint8_t* data, uint8_t length)
{
    if(length < 1)
        return;
    printf("Recieved %c\n", data[0]);
    if(data[0] == 'u')
        goUp();
    if(data[0] == 'd')
        dispense();
}
void RAD_Start()
{
    Net_SetCallback(OnReceive);
    Motor_Start(PIN_MOTOR1, PIN_MOTOR2);
    Motor_SetSpeed(MOTOR_SPEEDDOWN);
    Motor_Backward();
    sleep_ms(500);
    Motor_Brake();
    sleep_ms(1000);
    LDR_ID = LDR_Start(26);
    goUp();
}
void RAD_Update()
{
    Net_Update();
    printf("LDR Triggered: %s\n", LDR_Triggered(LDR_ID) ? "Yes" : "No");
    sleep_ms(100);
    switch (mode)
    {
        case MODE_DISPENSE:
            if(absolute_time_diff_us(timerStart, get_absolute_time()) > 500000)
                goDown();
            else if(absolute_time_diff_us(timerStart, get_absolute_time()) > 100000)
                Motor_Brake();
            break;
        case MODE_DOWN:
            if(absolute_time_diff_us(timerStart, get_absolute_time()) > 1000000)
                Motor_Brake();
            break;
        case MODE_UP:
            if(!LDR_Triggered(LDR_ID))
                break;
            Motor_Brake();
            mode = MODE_READY;
            break;
        default:
            break;
    }
}