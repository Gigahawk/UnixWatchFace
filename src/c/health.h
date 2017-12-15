#pragma once
#include <pebble.h>
#include "window.h"

typedef void (*HealthEventCallback)(void);

bool Health_init(void);
void Health_deinit(void);
bool Health_isUserSleeping(void);
HealthValue Health_getSleepSeconds(void);
HealthValue Health_getRestfulSleepSeconds(void);
HealthValue Health_getDistanceWalked(void);
HealthValue Health_getSteps(void);
HealthValue Health_getHeartRate(void);