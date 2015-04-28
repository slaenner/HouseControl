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

#include "bmc.h"
#include "logger.h"

void InitBcmHw()
{
  if (bcm2835_init() == 0)
  {
    exit(0);
  }
}

void InitPin(gpioPinId gpio, bcm2835FunctionSelect fsel)
{
    // Set GPIO pin to output
    bcm2835_gpio_fsel(gpio, fsel);
}

void ControlPin(gpioPinId gpio, gpioPinState state)
{
    bcm2835_gpio_write(gpio, state);
}