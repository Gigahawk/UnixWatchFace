#ifndef SETTINGS_H
#define SETTINGS_H

#include "debug.h"

#define QUIET_HOURS_NUM_SETTINGS 3
#define WEATHER_PROVIDER_LENGTH 10
// List of options

typedef enum settings_option
{
  o_weather_provider,
  o_weather_unit,
  o_weather_update_interval,
  o_weather_update_fail_indicator,
  o_quiet_hours_disabled_features
} settings_option;

typedef struct 
{
  char s_weather_provider[WEATHER_PROVIDER_LENGTH];
  char s_weather_unit[3];
  short s_weather_update_interval;
  bool s_weather_update_fail_indicator;
  
  bool s_quiet_hours_disabled_features[QUIET_HOURS_NUM_SETTINGS];
} settings_struct;

settings_struct m_settings;

void settings_init();
void settings_deinit();

void setNum(settings_option option, int value);
void setString(settings_option option, char* value);
void setArray(settings_option option, int* values, int size);

#endif //SETTINGS_H