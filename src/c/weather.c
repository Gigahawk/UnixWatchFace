#include "weather.h"
#include "window.h"
#include "bluetooth.h"

// Used layers
TextLayer* m_weather_layer;
char m_weather_layer_buffer[32];

// Storage for weather data
weather_struct temp_storage;

// Redraw the weather text
void update_weather()
{   
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Updating text");
#endif
  
  //Update the text buffer
  snprintf(m_weather_layer_buffer, sizeof(m_weather_layer_buffer), "%dC, %s", temp_storage.temp, temp_storage.conditions);
  
  //Set the text
  text_layer_set_text(m_weather_layer, m_weather_layer_buffer);
  
  //Update screen to make textlayers move
  refresh_display();
}

// New weather information will be processed here
void weather_handle(Tuple *weather_temperature,Tuple *weather_conditions)
{
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Recieved weather data");
#endif
  
  //Recieved weather so no longer waiting
  is_weather_waiting = false;
  
  temp_storage.temp = (int)weather_temperature->value->int32;

  snprintf(temp_storage.conditions, sizeof(temp_storage.conditions), "%s", weather_conditions->value->cstring);  

  // Redraw the text
  update_weather();  
}

void weather_timer_callback()
{
  if (is_weather_waiting) {
#ifdef DEBUG_WEATHER
    printf("weather_c: Didn't get weather from watch, retrying...");
#endif
    weather_request();
  }
}

void weather_save(){
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Saving data to persistent storage");
#endif
  
  persist_write_data(MESSAGE_KEY_WEATHER_STORAGE, &temp_storage, sizeof(weather_struct));
}

void weather_load(){
#ifdef DEBUG_WEATHER
  printf("weather_c: Checking persistent storage for weather data");
#endif
  
  if (persist_exists(MESSAGE_KEY_WEATHER_STORAGE)){
#ifdef DEBUG_WEATHER
    printf("weather_c: Found weather data, refreshing display");
#endif
    
    persist_read_data(MESSAGE_KEY_WEATHER_STORAGE, &temp_storage, sizeof(weather_struct));
    
    update_weather();
  } else {
#ifdef DEBUG_WEATHER
    printf("weather_c: No data found");
#endif
    
    return;
  }
}

// Request a weather update
void weather_request()
{
  //Only check weather if watch is connected to phone
  if (!is_connected) {
    is_weather_waiting = false;
    return;
  }
  
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Requesting the weather");
#endif
  
  is_weather_waiting = true;
  
  // Send Request
  communication_send(MESSAGE_KEY_WEATHER_TEMPERATURE);
  communication_send(MESSAGE_KEY_WEATHER_CONDITIONS);
  communication_send(MESSAGE_KEY_END);
  
  app_timer_register(20000, weather_timer_callback, NULL);
}

//Init all the variables
void weather_init()
{
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Initializing");
#endif

  // Init the vars 
  m_weather_layer = get_weather_layer();
  is_weather_waiting = false;
  
  weather_load();
}

//Deinit the weather module
void weather_deinit()
{
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Deinitializing");
#endif
  
  weather_save();
}