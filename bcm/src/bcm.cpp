#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#if !defined(SIMULATION)
#include <bcm2835.h>
#else
#include "stubs.h"
#endif

#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>

#include "bcm.h"
#include "logger.h"

void bcmCloseModule(void)
{
	bcm2835_close();
  RL_PRINT("BCM2835 closed\n");
}

void bcmInitModule(void)
{
  if (bcm2835_init() == 0)
  {
    exit(0);
  }
  RL_PRINT("BCM2835 initialized\n");
}

void bcmInitPin(gpioPinId gpio, bcm2835FunctionSelect fsel)
{
    char printBuf[100];
    sprintf(printBuf, "Initializing GPIO: GPIO%d=%d [0: input, 1:output]\n", (int)gpio, (int)fsel);
    RL_PRINT(printBuf);

    // Set GPIO pin to output
    bcm2835_gpio_fsel(gpio, fsel);
}

void bcmControlPin(gpioPinId gpio, gpioPinState state)
{
    char printBuf[100];
    sprintf(printBuf, "Setting GPIO state: GPIO%d=%d [0: low, 1:high]\n", (int)gpio, (int)state);
    RL_PRINT(printBuf);
    bcm2835_gpio_write(gpio, state);
}
