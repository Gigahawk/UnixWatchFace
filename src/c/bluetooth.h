#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <pebble.h>
#include "debug.h"
#include "battery.h"
#include "window.h"

#ifdef DEBUG_ENABLED
   #define DEBUG_BLUETOOTH
#endif

// Vars used int other files
bool is_connected;

// Functions used in other files
void bluetooth_init();
void bluetooth_deinit();
#endif