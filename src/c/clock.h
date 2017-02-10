#ifndef CLOCK_H
#define CLOCK_H

#include <pebble.h>

#ifdef DEBUG_ENABLED
   #define DEBUG_CLOCK
#endif

// Vars used in other files
bool is_clock_24h;

// Functions used in other files
void clock_init();
void clock_deinit();

#endif