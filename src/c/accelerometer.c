#include <pebble.h>
#include "accelerometer.h"
#include "window.h"
//#define DEBUG_ACCELEROMETER

static void tap_handle(AccelAxisType axis, int32_t direction){
#ifdef DEBUG_ACCELEROMETER
  printf("accelerometer_c: Tap recieved, counter is at %d", shake_counter);
#endif
  toggle_flashlight();
/*
  if(!shake_counter){
#ifdef DEBUG_ACCELEROMETER
    printf("accelerometer_c: Shake action activated");
#endif
    toggle_flashlight();
    shake_counter = 1;
    
    return;
  }
  
  shake_counter--;
*/
  
}

void accelerometer_init(void){
#ifdef DEBUG_ACCELEROMETER
  printf("acceleromter_c: Initializing");
#endif
  
  shake_counter = 1;
  
  accel_tap_service_subscribe(tap_handle);
  
  return;
}
void accelerometer_deinit(void){
#ifdef DEBUG_ACCELEROMETER
  printf("acceleromter_c: Deinitializing");
#endif
  
  accel_tap_service_unsubscribe();
  
  return;
}