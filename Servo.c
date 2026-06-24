#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
void Servo_Start(uint8_t pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 19999);
    pwm_init(pwm_gpio_to_slice_num(pin), &config, true);
}
void Servo_SetAngle(uint8_t pin, uint8_t angle)
{
    pwm_set_gpio_level(pin, 500 + angle * 10);
}