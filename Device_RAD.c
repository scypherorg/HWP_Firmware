#include <stdio.h>
#include "pico/stdlib.h"
#include "Net.h"
#include "Motor.h"
#define PIN_MOTOR1 5
#define PIN_MOTOR2 6

void OnReceive(uint8_t* data, uint8_t length)
{

}
void RAD_Start()
{
    Net_SetCallback(OnReceive);
    Motor_Start(PIN_MOTOR1, PIN_MOTOR2);
}
void RAD_Update()
{
    Net_Update();
}