#include "bluetooth.h"
#include "weather.h"

#define VIBRATE_CYCLE 10

TextLayer *m_bluetooth_layer;

//Update the bluetooth layer and all changes that need to be made
void update_bluetooth()
{
  if (is_connected) {
    text_layer_set_text(m_bluetooth_layer, "19485e21    device");
  } else {
    text_layer_set_text(m_bluetooth_layer, " ");
    
    //The battery module will disable the display when it recieves an invalid battery level
    battery_handle_phone(101,0);
    
    //Vibrate the watch
    for (int i = 0; i < VIBRATE_CYCLE; i++) {
      vibes_short_pulse();
      psleep(100);
      vibes_cancel();
      psleep(100);
    }
  }
}

// Bluetooth connection changes are processed here
static void bt_handle(bool connected) 
{
  //Only run if a change has occured
  if (is_connected == connected)
    return;
   
  //Debug printout
#ifdef DEBUG_BLUETOOTH
  printf("bluetooth_c: Connection status changed");
#endif
  
  // Save the current state
  is_connected = connected;
   
  if (is_connected) {
#ifdef DEBUG_BLUETOOTH
    printf("bluetooth_c: Connected!");
#endif
    
    //Update weather
    weather_request();
  } else {
      #ifdef DEBUG_BLUETOOTH
         printf("bluetooth_c: Disconnected!");
      #endif
  }

  //Update the interface 
  update_bluetooth();
}

// Init the bluetooth vars and register with the bluetooth connection service
void bluetooth_init()
{
  // Debug printout
#ifdef DEBUG_BLUETOOTH
  printf("bluetooth_c: Initializing");
#endif
  
  // Init vars
  m_bluetooth_layer = get_bluetooth_layer();
  
  // Register to the service
  bt_handle(connection_service_peek_pebble_app_connection());
  connection_service_subscribe((ConnectionHandlers) {.pebble_app_connection_handler = bt_handle});
}

// Unregister from the bluetooth connection service
void bluetooth_deinit()
{
  // Debug printout
#ifdef DEBUG_BLUETOOTH
  printf("bluetooth_c: Deinitializing");
#endif
  
  // Unregister from the bluetooth service
  connection_service_unsubscribe(); 
}