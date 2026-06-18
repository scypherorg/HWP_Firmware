#pragma region Includes
#include <stdio.h>
#include "pico/stdlib.h"
#include "Net.h"
#include "Device_RAD.h"
#pragma endregion
#pragma region Defines
#define DEVICE_ROUTER 0
#define DEVICE_RAD 1
#define DEVICE_BARRIER 2
#define DEVICE_PC 255
#define LED_PIN 3
#define LED_COUNT 0
#define RAD_MOTOR_IN1 2
#define RAD_MOTOR_IN2 3
#pragma endregion
const uint8_t DEVICE = DEVICE_RAD;

int main()
{
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, true);
    gpio_put(25, false);
    Net_Start(DEVICE, DEVICE == DEVICE_ROUTER);
    switch (DEVICE)
    {
        case DEVICE_RAD:
            RAD_Start();
            gpio_put(25, true);
            while (true)
                RAD_Update();
            break;
        default:
            return -1;
    }
    gpio_put(25, false);
}