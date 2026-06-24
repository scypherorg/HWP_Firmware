#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#define THRESHOLD_MARGIN 500
static uint32_t threshold[4];
uint8_t LDR_Start(uint8_t pin)
{
    adc_init();
    adc_gpio_init(pin);
    gpio_pull_up(pin);
    uint8_t id = pin - 26;
    threshold[id] = THRESHOLD_MARGIN;
    for(uint8_t i = 0; i < 16; i++)
    {
        threshold[id] += adc_read();
        sleep_ms(1);
    }
    return id;
}
bool LDR_Triggered(uint8_t id)
{
    adc_select_input(id);
    static uint32_t sum;
    sum = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        sum += adc_read();
        sleep_us(10);
    }
    if(sum + THRESHOLD_MARGIN > threshold[id])
        threshold[id]++;
    if(sum + THRESHOLD_MARGIN < threshold[id])
        threshold[id]--;
    return sum > threshold[id];
}