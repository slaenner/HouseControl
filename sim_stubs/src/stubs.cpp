#if defined(SIMULATION)

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dht11.h"
#include "logger.h"

void bcm2835_delay(int ms)
{
    char printBuf[100];
    sprintf(printBuf, "Waiting %d ms\n", ms);
    RL_PRINT(printBuf);

    usleep(ms * 1000);
}

int bcm2835_init(void)
{  
  return 1;
}

unsigned int SimulateSensorCrcResult(void)
{  
  srand48(time(NULL));
  
  if (drand48() > 0.5)
  {
    return DHT11_ERROR;
  }
  else
  {  
    return DHT11_OK;
  }
}

SensorData_t* GetData(int pin)
{  
  static SensorData_t ReturnData;

  if(pin == DHT11_1_Pin)
    ReturnData.TemperatureC = 21;
  else
    ReturnData.TemperatureC = 22;
  ReturnData.RHPercent = 75;
  ReturnData.DevPointC = DewPoint((double)ReturnData.TemperatureC, (double)ReturnData.RHPercent);
 
  ReturnData.NewData = SimulateSensorCrcResult();

  return &ReturnData;
}

void bcm2835_gpio_fsel(int pin, int fsel)
{
    char printBuf[100];
    sprintf(printBuf, "Setting GPIO%d to type %d [0: input, 1:output]\n", pin, fsel);
    RL_PRINT(printBuf);
}

void bcm2835_gpio_write(int pin, int fsel)
{
    char printBuf[100];
    sprintf(printBuf, "Setting GPIO%d=%d [0: low, 1:high]\n", pin, fsel);
    RL_PRINT(printBuf);
}

void bcm2835_close(void)
{
  return;
}

#endif

