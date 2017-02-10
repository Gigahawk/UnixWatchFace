
#include <pebble.h>

#include "window.h"
#include "weather.h"
#include "battery.h"
#include "bluetooth.h"
#include "time.h"
#include "communication.h"



static void init() {
  show_mainwindow();
  
  communication_init();
  
  clock_init();
  
  battery_init();
  
  bluetooth_init();
  
  weather_init();
 
}

static void deinit() {
  weather_deinit();
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
