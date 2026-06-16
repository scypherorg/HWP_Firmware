#include <stdio.h>
#include "pico/stdlib.h"
static uint8_t in1;
static uint8_t in2;
void Motor_Start(uint8_t pin_IN1, uint8_t pin_IN2)
{
    in1 = pin_IN1;
    in2 = pin_IN1;
    gpio_init(pin_IN1);
    gpio_init(pin_IN2);
    gpio_set_dir(pin_IN1, true);
    gpio_set_dir(pin_IN2, true);
    gpio_put(in1, true);
    gpio_put(in2, true);
}
void Motor_Forward()
{
    gpio_put(in1, true);
    gpio_put(in2, false);
}
void Motor_Backward()
{
    gpio_put(in1, false);
    gpio_put(in2, true);
}
void Motor_Brake()
{
    gpio_put(in1, true);
    gpio_put(in2, true);

}
void Motor_Coast()
{
    gpio_put(in1, false);
    gpio_put(in2, false);
}