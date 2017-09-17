#ifndef WEATHER_H
#define WEATHER_H

#include <pebble.h>
#include "debug.h"
#include "window.h"
#include "communication.h"
#include "time.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_WEATHER
#endif


// Weather struct
typedef struct 
{
  int temp;
  char conditions[32];
  short color;
} weather_struct;


bool is_weather_waiting;
AppTimer* weather_timer;

int weather_retries;

// Functions used in other files
void weather_handle(Tuple *weather_temperature,Tuple *weather_conditions);

void weather_init();
void weather_deinit();
void weather_request();

void weather_iterate_color();
void weather_reset_color();

// Settings Handlers
void weather_set_fail_indicator(bool enable);
#endif