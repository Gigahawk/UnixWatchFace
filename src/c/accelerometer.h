#ifndef ACCELERATION_H
#define ACCELERATION_H

//#include "weather.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_ACCELERATION
#endif

// Functions used in other files
void accelerometer_init(void);
void accelerometer_deinit(void);

#endif