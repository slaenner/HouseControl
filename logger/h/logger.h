#if !defined(LOGGER_H)
#define LOGGER_H

#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include "dht11.h"

#define RL_PRINT(x) (PrintDbgLog( __FUNCTION__, x, false))
#define HC_PRINT(x) (PrintDbgLog( NULL, x, true))

void loggerExit(void);
void loggerInit(void);
FILE *OpenLogFile(const char *FileName);
void CloseLogFile(FILE *handle);
void PrintDbgLog(const char *func, const char *str, const bool noTs);
void LogSensorData(FILE *handle, SensorData_t *data, unsigned int SensorId);

#endif
