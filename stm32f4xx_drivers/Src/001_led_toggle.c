/*
 * 001_led_toggle.c
 *
 *  Created on: Aug 9, 2026
 *      Author: Robbie
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

void delay(int time)
{
    for(int i=0; i < time; i++);
}
int main(void)
{
    GPIO_Handle_t gpio_led_green;
    GPIO_Handle_t gpio_led_orange;

    gpio_led_green.pGPIOx = GPIOD;
    gpio_led_green.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    gpio_led_green.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpio_led_green.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    gpio_led_green.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_led_green.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    gpio_led_orange.pGPIOx = GPIOD;
    gpio_led_orange.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    gpio_led_orange.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpio_led_orange.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    gpio_led_orange.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_led_orange.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&gpio_led_green);
    GPIO_Init(&gpio_led_orange);

    while(1)
    {
        GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
        GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13);
        delay(100000);
    }

    return 0;
}
