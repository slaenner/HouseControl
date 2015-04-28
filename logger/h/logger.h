#if !defined(LOGGER_H)
#define LOGGER_H

#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include "dht11.h"

#define RL_PRINT(x) (PrintDbgLog( __FUNCTION__, x))

void ExitLogger(void);
void InitLogger(void);
FILE *OpenLogFile(char *FileName);
void CloseLogFile(FILE *handle);
void PrintDbgLog(const char *func, char *str);
void LogSensorData(FILE *handle, SensorData_t *data, unsigned int SensorId);

#endif
