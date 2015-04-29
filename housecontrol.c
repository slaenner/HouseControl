#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined(SIMULATION)
#include <bcm2835.h>
#else
#include "stubs.h"
#endif

#include "logger.h"
#include "dht11.h"
#include "bmc.h"
#include "lcd_exp.h"
#include "housecontrol.h"

static FILE *LogFileHandle;


void Init(void)
{
    /* Initialize the logger module */
    loggerInit();
    
    /* Initialize BCM HW module */
    bcmInitModule();

    /* Open log file used for logging sensor data */
    LogFileHandle = OpenLogFile(LOG_DATA_FILE);

    /* Initialize DHT11 sensors */
    //DhtInit();

    /* Initialize relay outputs */
    bcmInitPin(pinGPIO4,  GPIO_OUTPUT);  // Relay 0
    bcmInitPin(pinGPIO17, GPIO_OUTPUT); // Relay 1
    
    /* Trace */
    RL_PRINT("Completed\n");
}

void Exit(void)
{
    /* Trace before log files are closed */
    RL_PRINT("Closing all handles\n");

    /* Close down log files and call logger exit function */
    CloseLogFile(LogFileHandle);
    loggerExit();
}

void ReadSensor(SensorData_t * ReturnData, int SensorId)
{
  char buf[100];
  sprintf(buf, "Reading from DHT11 Sensor %d\n", SensorId);
  RL_PRINT(buf);

  /* Invalidate sensor return code */
  ReturnData->NewData = DHT11_ERROR;

  /* Read sensor until result has been received successfull */
  while(ReturnData->NewData != DHT11_OK)
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
  }
}

int main(int argc, char **argv)
{
    char lcdstr[16];
    sprintf(lcdstr, "Stefan");

    /* Initialize */
    Init();

    /* Trace */
    RL_PRINT("System initialized\n");

    PrintLcdText(lcdstr);

    /* Shutdown */
    Exit();

    return 0;
}

