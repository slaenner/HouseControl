#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined(SIMULATION)
#include <bcm2835.h>
#else
#include "stubs.h"
#endif

#include <pthread.h>

#include "logger.h"
#include "dht11.h"
#include "bcm.h"
#include "lcd_exp.h"
#include "housecontrol.h"
#include "socketserver.h"

static FILE *LogFileHandle;
static SocketServer* socketServer;

#define LIGHT_POWER pinGPIO4
#define SENSOR_POWER pinGPIO17

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
    bcmInitPin(LIGHT_POWER,  GPIO_OUTPUT); // Relay 0
    bcmInitPin(SENSOR_POWER, GPIO_OUTPUT); // Relay 1
    
    /* Initialize socket server */
    socketServer = new SocketServer();
    socketServer->CreateSocket();
    socketServer->Bind();
    
    /* Trace */
    RL_PRINT("Completed\n");
}

void Exit(void)
{
    /* Trace before log files are closed */
    RL_PRINT("Closing all handles\n");

    /* Close socket server */
    delete socketServer;
    
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

bool handleCommand(serverCommand_t *serverCmd)
{
    bool res = false;

    /* Print the received command on the LCD */
    char lcdstr[32];
    sprintf(lcdstr, "Cmd Received:   %s = %s", serverCmd->cmd, serverCmd->par);
    PrintLcdText(lcdstr);

    /* Check if it is a command to control light */
    if(!strcmp(serverCmd->cmd, "Light"))
    {
        if(strcmp(serverCmd->par, "On") == 0)
        {
            RL_PRINT("Turning light on\n");

            /* Enable override power and disable sensor */
            bcmControlPin(LIGHT_POWER, gpioPinStateHigh);
            bcmControlPin(SENSOR_POWER, gpioPinStateLow);
        }
        else if(strcmp(serverCmd->par, "Off") == 0)
        {
            RL_PRINT("Turning light off\n");

            /* Turn off power to both sensor and override relay */
            bcmControlPin(LIGHT_POWER, gpioPinStateLow);
            bcmControlPin(SENSOR_POWER, gpioPinStateLow);
        }
        else if(strcmp(serverCmd->par, "Sensor") == 0)
        {
            RL_PRINT("Light controlled by sensor\n");

            /* Disable override power and enable sensor */
            bcmControlPin(LIGHT_POWER, gpioPinStateLow);
            bcmControlPin(SENSOR_POWER, gpioPinStateHigh);
        }
        else if(strcmp(serverCmd->par, "SHUTDOWN") == 0)
        {
            RL_PRINT("Light controlled by sensor and program shutdown\n");

            /* Leave only sensor enabled when shutting down server */
            bcmControlPin(LIGHT_POWER, gpioPinStateLow);
            bcmControlPin(SENSOR_POWER, gpioPinStateHigh);
            
            /* Set return code to indicate program shutdown */
            res = true;
        }
    }
            
    return res;
}

/* Parses sever message and puts command into *cmd */
bool parseMessage(char *msg, serverCommand_t *cmd)
{
    bool ret = false;
    char printBuf[100];
    char msgType[10];
    char cmdType[10];
    char cmdPars[10];

    sprintf(printBuf, "Parsing server message:'%s'\n", msg);
    RL_PRINT(printBuf);
    
    /* Parse message */
    sscanf(msg, "%s : %s = %s ;", msgType, cmdType, cmdPars);

    /* Ensure that server gave a command and that it was a light
       control command */
    if(!strcmp(msgType, "Cmd") && 
       !strcmp(cmdType, "Light"))
    {
        strcpy(cmd->cmd, cmdType);
        strcpy(cmd->par, cmdPars);

        RL_PRINT("Extracted command:\n");
        sprintf(printBuf, "  ->msgType = %s, cmdType = %s, cmdPar = %s\n", msgType, cmdType, cmdPars);
        RL_PRINT(printBuf);

        ret = true;
    }

    return ret;
}

int main(int argc, char **argv)
{
    bool run = true;

    /* Initialize */
    Init();

    /* Trace */
    RL_PRINT("System initialized\n");

    while(run)
    {
        char msg[32];
        serverCommand_t serverCmd;
    
        /* Listen for server messages */
        socketServer->Listen(msg);

        /* Parse server message and extract command according
           to defined command interface */
        if(parseMessage(msg, &serverCmd))
        {
            /* Call the command handler and exit in case command
               indicates it */
            if(handleCommand(&serverCmd))
            {
                /* Terminate program */
                run = false;
            }
        }
    }
    
    /* Shutdown */
    Exit();

    return 0;
}

