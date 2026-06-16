#pragma region Includes
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "Net.h"
#include "LED.h"
#pragma endregion
#pragma region Defines
#define DEVICE_ROUTER 0
#define DEVICE_RAD 1
#define DEVICE_PC 255
#define LED_PIN 3
#define LED_COUNT 0
#pragma endregion
const uint8_t DEVICE = DEVICE_ROUTER;

void OnReceived(uint8_t* data, uint8_t length)
{
    
}
void Start()
{
    stdio_init_all();
    Net_Start(DEVICE, OnReceived, DEVICE == DEVICE_ROUTER);
}
void Update()
{
    Net_Update();
}
int main()
{
    Start();
    while (true)
        Update();
}
