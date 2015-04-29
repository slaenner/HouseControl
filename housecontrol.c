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

int main(int argc, char **argv)
{
    char lcdstr[16];
    sprintf(lcdstr, "Stefan\n");
    
    /* Initialize */
    Init();

    /* Trace */
    RL_PRINT("System initialized\n");

    PrintLcdText(lcdstr);
    
    /* Shutdown */
    Exit();
    
    return 0;
}

