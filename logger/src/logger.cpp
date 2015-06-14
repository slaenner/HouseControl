#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>

#include "logger.h"
#include "dht11.h"

static FILE *fpdbg;

void loggerExit(void)
{
  /* Trace */
  RL_PRINT("Closing debug log file\n");

  /*Close the log file */
  CloseLogFile(fpdbg);
}


void loggerInit(void)
{
  /* Open a program debug log file */
#if defined(SIMULATION)
  fpdbg = OpenLogFile("PrgDbgLog.txt");
#else
  fpdbg = OpenLogFile("/home/pi/LogData/PrgDbgLog.txt");
#endif

  /* Print to the program debug log file that a new "session" has started */
  PrintDbgLog(__FUNCTION__, "\n\n#### New program session ####\n\n", false);

  /* Trace */
  RL_PRINT("Initialized PrgDbgLog.txt for write\n");
}

FILE *OpenLogFile(const char *FileName)
{
  /* File handle */
  FILE *fp = NULL;

  /* Open specified filename for write append */
  if((fp = fopen(FileName, "a")) == NULL)
  {
    printf("Failed to open %s\n", FileName);
    exit(1);
  }

  /* Return handle for file */
  return fp;
}

void CloseLogFile(FILE *handle)
{
  /* Close the file specified in handle */
  fclose(handle);
}

void LogSensorData(FILE *handle, SensorData_t *data, unsigned int SensorId)
{
  struct timeval time;
  time_t curtime;
  char ts[30];
  char logstr[200];

  /* Get time of day and reformat into ts */
  gettimeofday(&time, NULL);
  curtime = time.tv_sec;
  strftime(ts, 30,"%m-%d-%Y %T",localtime(&curtime));

  /* Build sensor data string*/
  sprintf(logstr, "Temp=%d\n", data->TemperatureC);
  fprintf(handle, "%s: [SENSOR%d]: %s", ts, SensorId, logstr);

  sprintf(logstr, "RH=%d\n", data->RHPercent);
  fprintf(handle, "%s: [SENSOR%d]: %s", ts, SensorId, logstr);

  sprintf(logstr, "DP=%f\n", data->DevPointC);
  fprintf(handle, "%s: [SENSOR%d]: %s", ts, SensorId, logstr);
}

void PrintDbgLogStr(const char *str)
{
}

void PrintDbgLog(const char *func, const char *str, const bool noTs)
{
    struct timeval time;
    time_t curtime;
    char ts[30];

    /* Get time of day and reformat into buf */
    gettimeofday(&time, NULL);
    curtime = time.tv_sec;
    strftime(ts, 30,"%m-%d-%Y %T",localtime(&curtime));

    /* Check if the time stamp should be printed */
    if(!noTs)
    {
        /* Write the time stamp to debug log file */
        fprintf(fpdbg, "%s ", ts);
        
        /* Also print it to the screen */
        printf("%s ", ts);
        
    }

    /* Check if the function name should be printed */
    if(func != NULL)
    {
        /* Write the time stamp to debug log file */
        fprintf(fpdbg, "%s(): ", func);
        
        /* Also print it to the screen */
        printf("%s(): ", func);
    }

    /* Write the specified string to the debug log */
    fprintf(fpdbg, "%s", str);

    /* Also print it to the screen */
    printf("%s", str);
}
