#if !defined(BMC_H)
#define BMC_H

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define MAXTIMINGS 100

// GPIO's defined on RP model B rev 2
typedef enum
{
    pinGPIO4  =  7,
    pinGPIO17 = 11,
    pinGPIO18 = 12,
    pinGPIO22 = 15,
    pinGPIO23 = 16,
    pinGPIO24 = 18,
    pinGPIO25 = 22,
    pinGPIO27 = 13
} gpioPinId;


// Pin state definitions
typedef enum
{
    gpioPinStateLow = 0,
    gpioPinStateHigh
} gpioPinState;

// GPIO function selections
#define GPIO_INPUT  BCM2835_GPIO_FSEL_INPT
#define GPIO_OUTPUT BCM2835_GPIO_FSEL_OUTP

void ControlPin(gpioPinId gpio, gpioPinState state);
void InitPin(gpioPinId gpio, bcm2835FunctionSelect fsel);
void InitBcmHw();

#endif /* BMC_H */
