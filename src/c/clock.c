#include "clock.h"
#include "weather.h"
#include "settings.h"

// Used layers
TextLayer* m_time_layer;

// Used variables 

static void update_time()
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a term buffer
  static char buffer[30];

  // Write the current hours and minutes into the buffer
  if (is_clock_24h) {
    // Use 24 hour format
    strftime(buffer, sizeof(buffer), "%a %b %d %H:%M %Y", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %I:%M:%S %p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(m_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
#ifdef DEBUG_CLOCK
  bool check = quiet_hours_enabled;
#endif
//   if(m_settings.s_quiet_hours && tick_time->tm_hour >= m_settings.s_quiet_hours_start_h && tick_time->tm_min >= m_settings.s_quiet_hours_start_m){
//     quiet_hours_enabled = true;
//   } else {
//     quiet_hours_enabled = false;
//   }
#ifdef DEBUG_CLOCK
  if(check != quiet_hours_enabled)
    printf("clock_c: Quiet time is now %d", quiet_hours_enabled);
#endif
  
  update_time();
  
  // Also get weather update every 60 minutes
  if(!(tick_time->tm_min % m_settings.s_weather_update_interval)) {
    weather_request();
  }
}

// Init the used variables and register with the time service
void clock_init()
{
  // Debug printout
#ifdef DEBUG_CLOCK
  printf("time_c: Initializing");
#endif
   
  // Init the vars
  m_time_layer = get_time_layer();
  is_clock_24h = clock_is_24h_style();
  
  
  // Register with TickTimerService
  if (is_clock_24h) {
    // Use 24 hour format (no seconds, saves battery)
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  } else {
    // Use 12 hour format (seconds)
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  }
  
  
  // Make sure the time is displayed from the start
  update_time();
}

// Unregister from the service
void clock_deinit()
{
  // Debug printout
#ifdef DEBUG_CLOCK
  printf("time_c: Deinitializing");
#endif
  // Unregister from the service
  tick_timer_service_unsubscribe();
}