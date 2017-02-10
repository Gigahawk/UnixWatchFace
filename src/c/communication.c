#include "communication.h"
#include "weather.h"
#include "battery.h"

//#define DEBUG_COMMUNICATION

DictionaryIterator *message;

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  // Get data from dictionary
  Tuple *weather_temperature = dict_find(iterator, MESSAGE_KEY_WEATHER_TEMPERATURE);
  Tuple *weather_conditions = dict_find(iterator, MESSAGE_KEY_WEATHER_CONDITIONS);
  
  Tuple *battery_level = dict_find(iterator, MESSAGE_KEY_PHONE_BATTERY);
  Tuple *battery_charging = dict_find(iterator, MESSAGE_KEY_PHONE_CHARGING);
  
  Tuple *com_ready = dict_find(iterator, MESSAGE_KEY_READY);
  
  if (weather_temperature && weather_conditions) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Weather info recieved");
#endif
    
    weather_handle(weather_temperature, weather_conditions);
  }
  
  if (battery_level) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Battery info recieved");
#endif
    battery_handle_phone((int)battery_level->value->int32,(bool)battery_charging->value->int32);
  }
  
  if (com_ready) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Communications are ready");
#endif
    is_com_active = true;
    
    //Request weather when communication is established
    weather_request();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Inbox message dropped - %d",reason);
#endif
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) 
{
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Outbox message sent");
#endif
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Error: Could not send AppMessage - %d",reason);
#endif
}



void communication_send(uint32_t key)
{
  if (key == MESSAGE_KEY_END) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Recieved END key, pushing message to phone");
#endif
    
    if (!is_message_building) {
#ifdef DEBUG_COMMUNICATION
      printf("communication_c: Error: No message to send");
#endif
    } else {
      dict_write_uint32(message,MESSAGE_KEY_END,1);
      app_message_outbox_send();
    }
    
    is_message_building = false;
    return;
  }
  
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Packaging key %d",(int)key);
#endif
  
  if (!is_message_building) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Start of new package");
#endif
    is_message_building = true;
    app_message_outbox_begin(&message);
  }
  
  dict_write_uint32(message,key,1);
}



// Register with the app messaging service
void communication_init()
{
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Initializing");
#endif
  
  is_com_active = false;
  is_message_building = false;

   // Register callbacks
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  
  app_message_open(500, 500); 
}

// Unregister from the messaging service
void communication_deinit()
{
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Deinitializing");
#endif
   
  app_message_deregister_callbacks();   
}