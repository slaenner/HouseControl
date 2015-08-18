#if !defined(BMC_H)
#define BMC_H

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define MAXTIMINGS 100

// GPIO's defined on RP model B rev 2
typedef enum
{
    pinGPIO4  = RPI_GPIO_P1_07,
    pinGPIO17 = RPI_GPIO_P1_11,
    pinGPIO18 = RPI_GPIO_P1_12,
    pinGPIO22 = RPI_GPIO_P1_15,
    pinGPIO23 = RPI_GPIO_P1_16,
    pinGPIO24 = RPI_GPIO_P1_18,
    pinGPIO25 = RPI_GPIO_P1_22,
    pinGPIO27 = RPI_GPIO_P1_13
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

void bcmControlPin(gpioPinId gpio, gpioPinState state);
void bcmInitPin(gpioPinId gpio, bcm2835FunctionSelect fsel);
void bcmInitModule(void);
void bcmCloseModule(void);

#endif /* BMC_H */
