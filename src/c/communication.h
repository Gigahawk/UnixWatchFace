#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <pebble.h>

#ifdef DEBUG_ENABLED
   #define DEBUG_COMMUNICATION
#endif

// Functions used in other files 

bool is_com_active;
bool is_message_building;

void communication_send(uint32_t key);
void communication_init();
void communication_deinit();

#endif