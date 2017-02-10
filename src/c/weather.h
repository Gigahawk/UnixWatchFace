#ifndef WEATHER_H
#define WEATHER_H

#include <pebble.h>
#include "window.h"
#include "communication.h"
#include "time.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_WEATHER
#endif

// Battery struct
typedef struct 
{
  int temp;
  char conditions[32];
} weather_struct;


bool is_weather_waiting;
AppTimer* weather_timer;

// Functions used in other files
void weather_handle(Tuple *weather_temperature,Tuple *weather_conditions);

void weather_init();
void weather_deinit();
void weather_request();

#endif