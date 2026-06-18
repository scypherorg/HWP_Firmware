#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#define PWM_WRAP 1000
static uint8_t in1;
static uint8_t in2;
static uint16_t motorSpeed = 500;
void Motor_Start(uint8_t pin_IN1, uint8_t pin_IN2)
{
    in1 = pin_IN1;
    in2 = pin_IN2;
    gpio_set_function(pin_IN1, GPIO_FUNC_PWM);
    gpio_set_function(pin_IN2, GPIO_FUNC_PWM);
    uint8_t slice1 = pwm_gpio_to_slice_num(in1);
    uint8_t slice2 = pwm_gpio_to_slice_num(in2);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 8.0f);
    pwm_config_set_wrap(&config, PWM_WRAP);
    pwm_init(slice1, &config, true);
    if(slice1 != slice2)
        pwm_init(slice2, &config, true);
    pwm_set_gpio_level(in1, PWM_WRAP);
    pwm_set_gpio_level(in2, PWM_WRAP);
}
void Motor_SetSpeed(uint16_t speed)
{
    motorSpeed = speed*10;
}
void Motor_Forward()
{
    pwm_set_gpio_level(in1, 1000 - motorSpeed);
    pwm_set_gpio_level(in2, PWM_WRAP);
}
void Motor_Backward()
{
    pwm_set_gpio_level(in1, PWM_WRAP);
    pwm_set_gpio_level(in2, 1000 - motorSpeed);
}
void Motor_Brake()
{
    pwm_set_gpio_level(in1, PWM_WRAP);
    pwm_set_gpio_level(in2, PWM_WRAP);
}
void Motor_Coast()
{
    pwm_set_gpio_level(in1, 0);
    pwm_set_gpio_level(in2, 0);
}