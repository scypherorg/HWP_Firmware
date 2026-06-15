#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
bool isRouter = false;
uint8_t deviceID = 0;
typedef void (*Net_OnReceive)(uint8_t *data, uint8_t length);
static Net_OnReceive onReceive;
const uint8_t sync[2] = {0x11, 0xAA};
void Net_Start(uint8_t _deviceID, Net_OnReceive _callback, bool _isRouter)
{
    deviceID = _deviceID;
    isRouter = _isRouter;
    onReceive = _callback;
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
}
void Net_Send(uint8_t ep, uint8_t* data, uint8_t length)
{
    uart_write_blocking(uart0, sync, 2);
    uart_write_blocking(uart0, &ep, 1);
    uart_write_blocking(uart0, &length, 1);
    uart_write_blocking(uart0, data, length);
}
void Net_Update()
{
    if(!uart_is_readable(uart0))
        return;
    static uint8_t ep = 0;
    static uint8_t length = 0;
    static uint8_t data[255];
    static bool synced;
    static uint8_t lastByte;
    synced = false;
    lastByte = uart_getc(uart0);
    while(!synced)
    {
        if(!uart_is_readable(uart0))
            return;
        if(lastByte == sync[0])
        {
            lastByte = uart_getc(uart0);
            synced = lastByte == sync[1];
            continue;
        }
        lastByte = uart_getc(uart0);
    }
    if(!synced)
        return;
    uart_read_blocking(uart0, &ep, 1);
    uart_read_blocking(uart0, &length, 1);
    uart_read_blocking(uart0, data, length);
    if(ep == deviceID)
        onReceive(data, length);
    else if(isRouter)
        Net_Send(ep, data, length);
}