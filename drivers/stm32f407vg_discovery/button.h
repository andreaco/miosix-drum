#ifndef MIOSIX_DRUM_BUTTON_H
#define MIOSIX_DRUM_BUTTON_H

void buttonsSetup()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // PIN 1
    int gpioPin = 1;
    GPIOD->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
    GPIOD->PUPDR &= ~(3 << (gpioPin * 2)); // reset
    GPIOD->OTYPER &= ~(1 << gpioPin); // reset
    GPIOD->PUPDR |= (2 << (gpioPin * 2)); // pull-down

    // PIN 2
    gpioPin = 2;
    GPIOD->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
    GPIOD->PUPDR &= ~(3 << (gpioPin * 2)); // reset
    GPIOD->OTYPER &= ~(1 << gpioPin); // reset
    GPIOD->PUPDR |= (2 << (gpioPin * 2)); // pull-down

    // PIN 3
    gpioPin = 3;
    GPIOD->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
    GPIOD->PUPDR &= ~(3 << (gpioPin * 2)); // reset
    GPIOD->OTYPER &= ~(1 << gpioPin); // reset
    GPIOD->PUPDR |= (2 << (gpioPin * 2)); // pull-down

    // PIN 4
    gpioPin = 4;
    GPIOD->MODER &= ~(3 << (gpioPin * 2)); // setting the GPIO in input mode
    GPIOD->PUPDR &= ~(3 << (gpioPin * 2)); // reset
    GPIOD->OTYPER &= ~(1 << gpioPin); // reset
    GPIOD->PUPDR |= (2 << (gpioPin * 2)); // pull-down
}

template <uint32_t GPIO_BASE, int PIN>
class Button
{
public:
    static void init()
    {
        GPIO_TypeDef* GPIO = (GPIO_TypeDef*) GPIO_BASE;

        // GPIO Setup
        // TODO: GLOBAL ENABLEGPIO
        enableGPIORCC(GPIO);

        GPIO->MODER  &= ~(3 << (PIN * 2)); // setting the GPIO in input mode
        GPIO->PUPDR  &= ~(3 << (PIN * 2)); // reset
        GPIO->OTYPER &= ~(1 << PIN);       // reset
        GPIO->PUPDR  |=  (2 << (PIN * 2)); // pull-down
    }

    static bool getState()
    {
        return (GPIOD->IDR & (1 << 1)) >> 1;
    }

private:
};


#endif //MIOSIX_DRUM_BUTTON_H