#ifndef HOUSECONTROL_H
#define HOUSECONTROL_H

#if !defined(SIMULATION)
#define LOG_DATA_FILE "/home/pi/LogData/LogData.txt"
#endif

typedef struct 
{
	char cmd[10]; /* Command */
	char par[10]; /* Parameter */
} serverCommand_t;

#endif
