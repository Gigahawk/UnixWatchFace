#include "communication.h"
#include "weather.h"
#include "battery.h"
#include "settings.h"

DictionaryIterator *message;

#ifdef DEBUG_COMMUNICATION
const char* messageKeyToString(uint32_t key){
  if(key == MESSAGE_KEY_END)
    return "END";
  if(key == MESSAGE_KEY_TIME_STORAGE)
    return "TIME_STORAGE";
  if(key == MESSAGE_KEY_WEATHER_STORAGE)
    return "WEATHER_STORAGE";
  if(key == MESSAGE_KEY_READY)
    return "READY";
  if(key == MESSAGE_KEY_weather_units)
    return "weather_units";
  if(key == MESSAGE_KEY_PHONE_BATTERY)
    return "PHONE_BATTERY";
  if(key == MESSAGE_KEY_PHONE_CHARGING)
    return "PHONE_CHARGING";
  if(key == MESSAGE_KEY_weather_interval)
    return "weather_interval";
  if(key == MESSAGE_KEY_weather_provider)
    return "weather_provider";
  if(key == MESSAGE_KEY_WEATHER_CONDITIONS)
    return "WEATHER_CONDITIONS";
  if(key == MESSAGE_KEY_WEATHER_TEMPERATURE)
    return "WEATHER_TEMPERATURE";
  if(key == MESSAGE_KEY_weather_units)
    return "weather_units";
  if(key == MESSAGE_KEY_weather_fail_indicator)
    return "weather_fail_indicator";
  
  return "UNKNOWN";
}
#endif

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  // Get settings from dictionary
  Tuple *weather_interval_t = dict_find(iterator, MESSAGE_KEY_weather_interval);
  Tuple *weather_fail_indicator_t = dict_find(iterator, MESSAGE_KEY_weather_fail_indicator);
  Tuple *quiet_hours_disabled_features_t = dict_find(iterator, MESSAGE_KEY_quiet_hours_disabled_features);

  // Get data from dictionary
  Tuple *weather_t = dict_find(iterator, MESSAGE_KEY_WEATHER);
  Tuple *weather_failed_t = dict_find(iterator, MESSAGE_KEY_WEATHER_FAILED);
  Tuple *weather_gps_t = dict_find(iterator, MESSAGE_KEY_WEATHER_GPS);
  
  Tuple *battery_exists_t = dict_find(iterator, MESSAGE_KEY_PHONE_BATTERY_EXISTS);
  Tuple *battery_level_t = dict_find(iterator, MESSAGE_KEY_PHONE_BATTERY);
  Tuple *battery_charging_t = dict_find(iterator, MESSAGE_KEY_PHONE_CHARGING);
  
  Tuple *com_ready_t = dict_find(iterator, MESSAGE_KEY_READY);
  
  if (weather_interval_t){
    int weather_interval = (int)weather_interval_t->value->int32;
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Weather interval setting recieved: %d minutes", weather_interval);
#endif
    setNum(o_weather_update_interval, weather_interval);
  }
  
  if (weather_fail_indicator_t){
    bool enable = weather_fail_indicator_t->value->int32;
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Weather Fail Indicator setting recieved: %d", enable);
#endif
    setNum(o_weather_update_fail_indicator, enable);
  }
  
  if (quiet_hours_disabled_features_t){
//     bool settings[QUIET_HOURS_NUM_SETTINGS];
    
//     for(int i = 0; i < QUIET_HOURS_NUM_SETTINGS; i++){
//       quiet_hours_disabled_features_t = dict_find(iterator, MESSAGE_KEY_quiet_hours_disabled_features + i);
//       settings[i] = (bool)quiet_hours_disabled_features_t->value->uint8;
//     }
    
// #ifdef DEBUG_COMMUNICATION
//     printf("communication_c: Quiet Hours disabled features recieved: [0]%d, [1]%d, [2]%d",(int)settings[0],(int)settings[1],(int)settings[2]);
// #endif
    
    //setArray(o_quiet_hours_disabled_features, &settings, QUIET_HOURS_NUM_SETTINGS);
    
  }
  
  if (weather_t && weather_gps_t) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Weather info recieved");
#endif
    
    weather_handle(weather_t, weather_gps_t);
  }
  if (weather_failed_t){
    weather_iterate_color();
  }
  
  if(battery_exists_t) {
    
  }

  if (battery_level_t) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Battery info recieved");
#endif
    battery_handle_phone((int)battery_level_t->value->int32,(bool)battery_charging_t->value->int32);
  }
  
  if (com_ready_t) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Communications are ready");
#endif
    is_com_active = true;
    
    //Request weather when communication is established (also push messages still in queue)
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
  app_message_outbox_begin(&iterator);
  app_message_outbox_send();
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
      if(!is_com_active){
#ifdef DEBUG_COMMUNICATION
        printf("communication_c: Error: Coms are not active yet");
#endif
        return;
      }
      dict_write_uint32(message,MESSAGE_KEY_END,1);
      app_message_outbox_send();
    }
    
    is_message_building = false;
    return;
  }
  
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Packaging key %s,id: %d",messageKeyToString(key), (int)key);
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

void update_weather_settings(const char* provider,const char* units){
#ifdef DEBUG_COMMUNICATION
  printf("communication_c: Updating weather provider to %s", provider);
#endif
  
  if (!is_message_building) {
#ifdef DEBUG_COMMUNICATION
    printf("communication_c: Start of new package");
#endif
    is_message_building = true;
    app_message_outbox_begin(&message);
  }
  
  dict_write_cstring(message, MESSAGE_KEY_weather_provider, provider);
  dict_write_cstring(message, MESSAGE_KEY_weather_units, units);

  communication_send(MESSAGE_KEY_END);
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