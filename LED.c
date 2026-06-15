#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "LED.pio.h"
uint32_t* color;
uint16_t ledCount;
bool hasChanged;
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return
    ((uint32_t)(g)<<8)|
    ((uint32_t)(r)<<16)|
    ((uint32_t)(b));
}
void LED_Start(uint pin, uint16_t _ledCount)
{
    led_program_init(pio0, 0, pio_add_program(pio0, &led_program), pin, 800000, false);
    uint32_t c[ledCount];
    color = c;
    ledCount = _ledCount;
}
void LED_Update()
{
    if(!hasChanged)
        return;
    for (uint16_t i = 0; i < ledCount; i++)
        pio_sm_put_blocking(pio0, 0, color[i]);
    hasChanged = false;
}
void LED_setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    while (index > ledCount)
        index -= ledCount;
    while(index < 0)
        index += ledCount;
    color[index] = urgb_u32(r, g, b)<<8u;
    hasChanged = true;
}