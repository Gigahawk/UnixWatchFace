#ifndef BATTERY_H
#define BATTERY_H

#include <pebble.h>

#ifdef DEBUG_ENABLED
   #define DEBUG_BATTERY
#endif

// Battery struct
struct battery_struct
{
  uint8_t id;
  uint8_t level;
  bool is_charging;
  char text[16];
};



// Functions used in other files
void battery_init();
void battery_deinit();
void battery_handle_phone(int level,bool is_charging);
#endif