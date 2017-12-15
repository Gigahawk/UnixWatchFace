#ifdef PBL_HEALTH
#include <pebble.h>
#include "health.h"

static bool s_sleeping;
static HealthValue s_sleep_seconds;
static HealthValue s_restful_sleep_seconds;
static HealthValue s_distance_walked;
static HealthValue s_steps;
static HealthValue s_heart_rate;

TextLayer* m_sleep_layer;
char m_sleep_layer_buffer[32];
int updates;

static bool is_health_metric_accessible(HealthMetric metric) {
    time_t start = time_start_of_today();
    time_t end = time(NULL);

    HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
    return mask & HealthServiceAccessibilityMaskAvailable;
}

static inline HealthValue get_health_value_sum_today(HealthMetric metric) {
    return is_health_metric_accessible(metric) ? health_service_sum_today(metric) : 0;
}

static void health_event_handler(HealthEventType event, void *context) {
    if (event == HealthEventSignificantUpdate) {
        health_event_handler(HealthEventSleepUpdate, NULL);
        health_event_handler(HealthEventMovementUpdate, NULL);
        health_event_handler(HealthEventHeartRateUpdate, NULL);
    } else if (event == HealthEventSleepUpdate) {
        HealthActivityMask mask = health_service_peek_current_activities();
        bool was_sleeping = s_sleeping;
        s_sleeping = (mask & HealthActivitySleep) || (mask & HealthActivityRestfulSleep);
        s_sleep_seconds = get_health_value_sum_today(HealthMetricSleepSeconds);
        s_restful_sleep_seconds = get_health_value_sum_today(HealthMetricSleepRestfulSeconds);
      
        int sleep_minutes = s_sleep_seconds/60;
        int r_sleep_minutes = s_restful_sleep_seconds/60;
      
        int sleep_hours = sleep_minutes/60;
        int r_sleep_hours = r_sleep_minutes/60;
        
        sleep_minutes = sleep_minutes % 60;
        r_sleep_minutes = r_sleep_minutes% 60;
        
        if(was_sleeping != s_sleeping)
          updates++;
        
        snprintf(m_sleep_layer_buffer, sizeof(m_sleep_layer_buffer), "Z%d,S=%d:%02d,RS=%d:%02d,U=%d", s_sleeping, sleep_hours, sleep_minutes, r_sleep_hours, r_sleep_minutes,updates);
        text_layer_set_text(m_sleep_layer, m_sleep_layer_buffer);
        
    } else if (event == HealthEventMovementUpdate) {
        HealthActivityMask mask = health_service_peek_current_activities();
        s_sleeping = (mask & HealthActivitySleep) || (mask & HealthActivityRestfulSleep);
        s_distance_walked = get_health_value_sum_today(HealthMetricWalkedDistanceMeters);
        s_steps = get_health_value_sum_today(HealthMetricStepCount);
    } else if (event == HealthEventHeartRateUpdate) {
        s_heart_rate = is_health_metric_accessible(HealthMetricHeartRateBPM) ? health_service_peek_current_value(HealthMetricHeartRateBPM) : 0;
    }

}

bool Health_init() {
    m_sleep_layer = get_sleep_layer();
    updates = 0;
    bool b = health_service_events_subscribe(health_event_handler, NULL);
//     if (b) {
//         health_event_handler(HealthEventSignificantUpdate, NULL);
//     }
    return b;
}

void Health_deinit(void) {
    health_service_events_unsubscribe();
}

bool Health_isUserSleeping(void) {
    return s_sleeping;
}

HealthValue Health_getSleepSeconds(void) {
    return s_sleep_seconds;
}

HealthValue Health_getRestfulSleepSeconds(void) {
    return s_restful_sleep_seconds;
}

HealthValue Health_getDistanceWalked(void) {
    return s_distance_walked;
}

HealthValue Health_getSteps(void) {
    return s_steps;
}

HealthValue Health_getHeartRate(void) {
    return s_heart_rate;
}

#endif // PBL_HEALTH