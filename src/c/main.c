#include <pebble.h>

#include "window.h"
#include "weather.h"
#include "battery.h"
#include "bluetooth.h"
#include "clock.h"
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
  
  bluetooth_deinit();
  
  battery_deinit();
  
  clock_deinit();
  
  communication_deinit();
  
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
