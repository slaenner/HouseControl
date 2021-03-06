//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//
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
#include <sys/ioctl.h>
#include <math.h>

#if !defined(SIMULATION)
#include <bcm2835.h>
#include <linux/i2c-dev.h>
#else
#include "stubs.h"
#endif

#include "logger.h"
#include "dht11.h"

static int initialized = 0;

void DhtInit(void)
{
  if (bcm2835_init() == 0)
  {
    exit(0);
  }
  initialized = 1;
}

void ReadDht11(SensorData_t * ReturnData, int SensorId)
{
  SensorData_t *pSensorData;
  
  if (initialized == 0)
  {
    fprintf(stderr, "Error BMC2835 not initialized\n");
    exit(0);
  }

  if(SensorId == DHT11_SENSOR_1)
  {
    /* Read the data from sensor #1 */
    pSensorData = GetData(DHT11_1_Pin);
  
    /* When the sensor reads data print it */
    if(pSensorData->NewData != DHT11_OK)
    {
      RL_PRINT("Failed reading DHT11 #1\n");
    }
  }
  
  if(SensorId == DHT11_SENSOR_2)
  {
    /* Read the data from sensor #1 */
    pSensorData = GetData(DHT11_2_Pin);
  
    /* When the sensor reads data print it */
    if(pSensorData->NewData != DHT11_OK)
    {
      RL_PRINT("Failed reading DHT11 #1\n");
    }
  }

  *ReturnData = *pSensorData;

  usleep(500000);
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
double DewPoint(double celsius, double humidity)
{
  double A0= 373.15/(273.15 + celsius);
  double SUM = -7.90298 * (A0-1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
  SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM-3) * humidity;
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558-T);
}

int bits[250], data[100];
int bitidx = 0;

#if !defined(SIMULATION)
SensorData_t* GetData(int pin)
{
  /* Allocate memory to which return point will be given */
  static SensorData_t ReturnData;
  ReturnData.NewData = DHT11_ERROR;

  int counter = 0;
  int laststate = HIGH;
  int j=0;
  
  // Reset global variables
  memset(&bits, 0, 250);
  memset(&data, 0, 100);
  bitidx = 0;
  
  // Set GPIO pin to output
  bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(pin, HIGH);
  usleep(500000);  // 500 ms
  bcm2835_gpio_write(pin, LOW);
  usleep(20000);
  
  bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
  
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
  // wait for pin to drop?
  while (bcm2835_gpio_lev(pin) == 1) {
    usleep(1);
  }
  
  // read data!
  int i;
  for (i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while ( bcm2835_gpio_lev(pin) == laststate) {
      counter++;
      //nanosleep(1);		// overclocking might change this?
      if (counter == 1000)
        break;
    }
    laststate = bcm2835_gpio_lev(pin);
    if (counter == 1000) break;
    bits[bitidx++] = counter;
    
    if ((i>3) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > 200)
        data[j/8] |= 1;
      j++;
    }
  }
  
  /* Check if 40 bits was returned from sensor */
  if(j < 39)
  {
    ReturnData.NewData = DHT11_ERROR_TIMEOUT;
  }
  
  /* Check if the checksum matched */
  if(!(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
  {
    ReturnData.NewData = DHT11_ERROR_CHECKSUM;
  }
    
  if ((j >= 39) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
  {
    // yay!
    ReturnData.TemperatureC = data[2];
    ReturnData.RHPercent = data[0];
    ReturnData.DevPointC = DewPoint((double)data[2], (double)data[0]);
    ReturnData.NewData = DHT11_OK;
    return &ReturnData;
  }

  return &ReturnData;
}

void dht11ReadSensorRetry(SensorData_t * ReturnData, int SensorId, int retries)
{
    char buf[100];
    int cnt = 0;
    sprintf(buf, "Reading from DHT11 Sensor %d\n", SensorId);
    RL_PRINT(buf);
    
    /* Invalidate sensor return code */
    ReturnData->NewData = DHT11_ERROR;
    
    /* Read sensor until result has been received successfull */
    while((ReturnData->NewData != DHT11_OK) &&
          (cnt <= retries))
    {
        RL_PRINT("Fetching data from DHT11\n");
        
        /* Read DHT11 sensor 2 */
        ReadDht11(ReturnData, SensorId);
        
        if(ReturnData->NewData == DHT11_OK)
        {
            RL_PRINT("Data from DHT11 read OK\n");
        }
        else
        {
            RL_PRINT("Data from DHT11 ERROR - Retrying\n");
        }
        cnt++;
    }
}

#endif
