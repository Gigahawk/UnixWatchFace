#ifndef BATTERY_H
#define BATTERY_H

#include <pebble.h>
#include "debug.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_BATTERY
#endif

// Battery struct
typedef struct
{
  int id;
  int level;
  bool is_charging;
  char text[16];
}battery_struct;



// Functions used in other files
void battery_init();
void battery_deinit();
void battery_handle_phone(int level,bool is_charging);
#endif