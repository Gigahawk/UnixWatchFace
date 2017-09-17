#include <pebble.h>
#include "settings.h"

static void settings_save();
static void settings_load();


#ifdef DEBUG_SETTINGS
void settings_print(){
//   {
//   char s_weather_provider[10];
//   char s_weather_unit;
//   short s_weather_update_interval;
//   bool s_weather_update_fail_indicator;
  
//   bool s_quiet_hours;
//   char s_quiet_hours_start_h;
//   char s_quiet_hours_start_m;
//   char s_quiet_hours_end_h;
//   char s_quiet_hours_end_m; 
//   bool s_quiet_hours_disabled_features[QUIET_HOURS_NUM_SETTINGS];
// }
  printf("settings_c: Printing settings...");
  printf("Weather Provider: %s", m_settings.s_weather_provider);
  printf("Weather Unit: %s", m_settings.s_weather_unit);
  printf("Weather Update Interval: %d", m_settings.s_weather_update_interval);
  printf("Weather Update Fail Indicator: %d", (int)m_settings.s_weather_update_fail_indicator);
  printf("Quiet Hours Enabled: %d", (int)m_settings.s_quiet_hours);
  printf("Quiet Hours Start Hour: %d", m_settings.s_quiet_hours_start_h);
  printf("Quiet Hours Start Minute: %d", m_settings.s_quiet_hours_start_m);
  printf("Quiet Hours End Hour: %d", m_settings.s_quiet_hours_end_h);
  printf("Quiet Hours End Minute: %d", m_settings.s_quiet_hours_end_m);
}
#endif

//Init all the variables
void settings_init()
{
  // Debug printout
#ifdef DEBUG_SETTINGS
  printf("settings_c: Initializing");
#endif
  settings_load();
  
}


void setNum(settings_option option, int value){
  switch(option){
    case o_weather_update_interval:
      m_settings.s_weather_update_interval = value;
      break;
    case o_weather_update_fail_indicator:
      m_settings.s_weather_update_fail_indicator = value;
      break;
    case o_quiet_hours:
      m_settings.s_quiet_hours = value;
      break;
    case o_quiet_hours_start_h:
      m_settings.s_quiet_hours_start_h = value;
      break;
    case o_quiet_hours_start_m:
      m_settings.s_quiet_hours_start_m = value;
      break;
    case o_quiet_hours_end_h:
      m_settings.s_quiet_hours_end_h = value;
      break;
    case o_quiet_hours_end_m:
      m_settings.s_quiet_hours_end_m = value;
      break;
    default:
#ifdef DEBUG_SETTINGS
      printf("settings_c: Warning: unknown or noninteger input");
#endif
      break;
  }

#ifdef DEBUG_SETTINGS
  settings_print();
#endif

}

void setString(settings_option option, char* value){
  switch(option){
    case o_weather_provider:
      snprintf(m_settings.s_weather_provider, sizeof(m_settings.s_weather_provider), "%s",value);
      break;
    case o_weather_unit:
      snprintf(m_settings.s_weather_unit, sizeof(m_settings.s_weather_unit), "%s", value);
      break;
    default:
      break;
  }
}

void setTime(settings_option option, char* value){
  int hour = (value[0]-48)*10 + (value[1]-48);
  int minute = (value[3]-48)*10 + (value[4]-48);
#ifdef DEBUG_SETTINGS
  printf("settings_c: Setting time to %d:%d", hour, minute);
#endif 
  switch(option){
    case o_quiet_hours_start:
      setNum(o_quiet_hours_start_h,hour);
      setNum(o_quiet_hours_start_m,minute);
      break;
    case o_quiet_hours_end:
      setNum(o_quiet_hours_end_h,hour);
      setNum(o_quiet_hours_end_m,minute);
      break;
    default:
      break;
  }
}

void settings_deinit(){
  // Debug printout
#ifdef DEBUG_SETTINGS
  printf("settings_c: Deinitializing");
#endif
  settings_save();
}

static void settings_save(){
  // Debug printout
#ifdef DEBUG_SETTINGS
  printf("settings_c: Saving settings");
#endif
  
  persist_write_data(MESSAGE_KEY_SETTINGS_STORAGE, &m_settings, sizeof(settings_struct));
}

static void settings_load(){
  // Debug printout
#ifdef DEBUG_SETTINGS
  printf("settings_c: Checking persistent storage for settings data");
#endif
  
  if(persist_exists(MESSAGE_KEY_SETTINGS_STORAGE)){
#ifdef DEBUG_SETTINGS
    printf("settings_c: Found settings data, loading from storage");
#endif
    
    persist_read_data(MESSAGE_KEY_SETTINGS_STORAGE,&m_settings, sizeof(settings_struct));
  } else {
#ifdef DEBUG_SETTINGS
    printf("settings_c: No data found, loading default settings");
#endif
    
    //TODO, have default settings
  }
  
}