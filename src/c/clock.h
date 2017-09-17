#ifndef CLOCK_H
#define CLOCK_H

#include <pebble.h>
#include "debug.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_CLOCK
#endif

// Vars used in other files
bool quiet_hours_enabled;
bool is_clock_24h;

// Functions used in other files
void clock_init();
void clock_deinit();

#endif