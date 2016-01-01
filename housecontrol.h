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

#define LIGHT_POWER  pinGPIO17 /* PIN11 */
#define SENSOR_POWER pinGPIO4  /* PIN7  */
#define LIGHT_POWER2 pinGPIO27 /* PIN13 */

typedef enum 
{
    SwitchStateOff,
    SwitchStateOn
} SwitchState_t;

typedef enum 
{
    SwitchId1 = 0,
    SwitchId2 = 2,
    SwitchId3 = 4,
    SwitchId4 = 6
} SwitchId_t;

gpioPinId SwithIdPinMapping[8] = {
    pinGPIO4,  /* Relay 0 */
    pinGPIO17, /* Relay 1 */
    pinGPIO18, /* Relay 2 */
    pinGPIO22, /* Relay 3 */
    pinGPIO23, /* Relay 4 */
    pinGPIO24, /* Relay 5 */
    pinGPIO25, /* Relay 6 */
    pinGPIO27  /* Relay 7 */
};

#endif
