#ifndef WINDOW_H
#define WINDOW_H

#include <pebble.h>
#include "debug.h"

//#include "configuration.h"
#include "battery.h"


#ifdef DEBUG_ENABLED
   #define DEBUG_MAINWINDOW
#endif

#ifdef DEBUG_ENABLED
   #define DEBUG_IMAGEREDRAW
#endif

bool is_flashlight;

// Functions used in other files
void show_mainwindow(void);
void hide_mainwindow(void);

void toggle_flashlight(void);

void refresh_display(void);

// Functions to return all the text pointers
TextLayer* get_time_layer();
TextLayer* get_watchbattery_layer();
TextLayer* get_phonebattery_layer();
TextLayer* get_weather_command_layer();
TextLayer* get_weather_layer();
TextLayer* get_bluetooth_layer();

TextLayer* get_sleep_layer();
#endif //WINDOW_H