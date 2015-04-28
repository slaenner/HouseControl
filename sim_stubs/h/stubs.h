
#include "dht11.h"

int bcm2835_init(void);
SensorData_t* GetData(int pin);

typedef enum
{
    BCM2835_GPIO_FSEL_INPT  = 0,   ///< Input
    BCM2835_GPIO_FSEL_OUTP  = 1,   ///< Output
    BCM2835_GPIO_FSEL_ALT0  = 4,   ///< Alternate function 0
    BCM2835_GPIO_FSEL_ALT1  = 5,   ///< Alternate function 1
    BCM2835_GPIO_FSEL_ALT2  = 6,   ///< Alternate function 2
    BCM2835_GPIO_FSEL_ALT3  = 7,   ///< Alternate function 3
    BCM2835_GPIO_FSEL_ALT4  = 3,   ///< Alternate function 4
    BCM2835_GPIO_FSEL_ALT5  = 2,   ///< Alternate function 5
    BCM2835_GPIO_FSEL_MASK  = 7    ///< Function select bits mask
} bcm2835FunctionSelect;

void bcm2835_gpio_fsel(int pin, int fsel);
void bcm2835_gpio_write(int pin, int fsel);
