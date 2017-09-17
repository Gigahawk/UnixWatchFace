#include "weather.h"
#include "window.h"
#include "bluetooth.h"
#include "settings.h"

#define RETRY_COUNT 1

// Used layers
TextLayer* m_weather_layer;
char m_weather_layer_buffer[32];

// Storage for weather data
weather_struct temp_storage;

#define NUM_COLORS 4
const int weather_colors[] = {0xFFFFFF, //White
                              0xFFFF55, //Icterine
                              0xFFAA55, //Rajah
                              0xFF5500, //Orange
                              0xFF0000  //Red
                             };

// Redraw the weather text
void update_weather()
{   
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Updating text");
#endif
  
  //Update the text buffer
  snprintf(m_weather_layer_buffer, sizeof(m_weather_layer_buffer), "%d%c, %s", temp_storage.temp, m_settings.s_weather_unit[0]=='s'? 'C':'F', temp_storage.conditions);
  
  //Set the text
  text_layer_set_text(m_weather_layer, m_weather_layer_buffer);
  
  
  //Update screen to make textlayers move
  refresh_display();
}

void update_color()
{
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Updating color");
#endif
  
  text_layer_set_text_color(m_weather_layer, GColorFromHEX(weather_colors[temp_storage.color]));
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
  
  //Reset number of retries
  weather_retries = 0;
  
  weather_reset_color();
  
  temp_storage.temp = (int)weather_temperature->value->int32;

  snprintf(temp_storage.conditions, sizeof(temp_storage.conditions), "%s", weather_conditions->value->cstring);  

  // Redraw the text
  update_weather();  
}

void weather_timer_callback()
{
#ifdef DEBUG_WEATHER
  printf("weather_c: Checking if weather was recieved");
#endif
    
  if(is_weather_waiting) {
#ifdef DEBUG_WEATHER
    printf("weather_c: Didn't get weather from phone");
#endif
    if(weather_retries < RETRY_COUNT){
#ifdef DEBUG_WEATHER
      printf("weather_c: Retrying...");
#endif
      weather_retries++;
      weather_request();
    } else {
#ifdef DEBUG_WEATHER
      printf("weather_c: Too many retrys, do you have an internet connection?");
#endif
      weather_retries = 0;
      weather_iterate_color();
    }
  } else {
#ifdef DEBUG_WEATHER
    printf("weather_c: Weather was recieved");
#endif
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
    
    update_color();
    update_weather();
  } else {
#ifdef DEBUG_WEATHER
    printf("weather_c: No data found");
#endif
    
    //Set initial value for color index
    temp_storage.color = 0;
    update_color();
    
    return;
  }
}

// Request a weather update
void weather_request()
{
  // Debug printout
#ifdef DEBUG_WEATHER
  printf("weather_c: Requesting the weather");
#endif
  
  //Only check weather if watch is connected to phone
  if (!is_connected) {
#ifdef DEBUG_WEATHER
    printf("weather_c: No connection detected, skipping weather check");
#endif
    weather_iterate_color();
    is_weather_waiting = false;
    return;
  }
  
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
  
  // Init pebblekitJS Settings first
  update_weather_settings(m_settings.s_weather_provider,m_settings.s_weather_unit);

  // Init the vars 
  m_weather_layer = get_weather_layer();
  is_weather_waiting = false;
  weather_retries = 0;

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

void weather_iterate_color()
{
#ifdef DEBUG_WEATHER
  printf("weather_c: Iterating color from %d",temp_storage.color);
#endif

#ifdef PBL_BW
  printf("weather_c: Can't iterate color on a non-color display, continuing...");
  return;
#endif
  
  // Only iterate color if the fail indicator is active
  if(!m_settings.s_weather_update_fail_indicator)
  {
#ifdef DEBUG_WEATHER
    printf("weather_c: Weather fail indicator is disabled, continuing...");
#endif
    return;
  }
  
  if(temp_storage.color < NUM_COLORS){
    temp_storage.color++;
    update_color();
  }
}

void weather_reset_color()
{
#ifdef DEBUG_WEATHER
  printf("weather_c: Resetting color from %d",temp_storage.color);
#endif
  
  temp_storage.color = 0;
  update_color();
}

void weather_set_fail_indicator(bool enable){
#ifdef DEBUG_WEATHER
  printf("weather_c: Setting fail indicator to %d", enable);
#endif
  
  //weather_fail_indicator = enable;
}