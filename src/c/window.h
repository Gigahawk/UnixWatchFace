#ifndef WINDOW_H
#define WINDOW_H

#include <pebble.h>
//#include "configuration.h"
#include "battery.h"

//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
   #define DEBUG_MAINWINDOW
#endif

#ifdef DEBUG_ENABLED
   //#define DEBUG_IMAGEREDRAW
#endif

// Functions used in other files
void show_mainwindow(void);
void hide_mainwindow(void);

void refresh_display(void);

// Functions to return all the text pointers
TextLayer* get_time_layer();
TextLayer* get_watchbattery_layer();
TextLayer* get_phonebattery_layer();
TextLayer* get_weather_layer();
TextLayer* get_bluetooth_layer();
#endif //WINDOW_H