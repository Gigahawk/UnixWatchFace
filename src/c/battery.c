#include "battery.h"
#include "window.h"

// Battery vars
battery_struct m_battery_pebble;
battery_struct m_battery_phone;

// Textlayers
TextLayer* m_watchbattery_layer;
TextLayer* m_phonebattery_layer;


// Redraw all battery related things
void update_battery(battery_struct *Battery, TextLayer *batnum)
{
#ifdef DEBUG_BATTERY
  printf("battery_c: Updating battery text");
#endif
  if(Battery->level <= 100){
    printf("battery_c: valid battery level");
    snprintf(Battery->text, sizeof(Battery->text), "Bat%d:%c%d%%", Battery->id, Battery->is_charging? '+':' ', Battery->level);
  }else{
    printf("battery_c: invalid battery level");
    snprintf(Battery->text, sizeof(Battery->text), "Bat%d: --", Battery->id);
  }

#ifdef DEBUG_BATTERY
  printf("[BATT][Battery_Redraw] Redrawing battery text %s", text_layer_get_text(batnum));
#endif
   // Draw the battery's text
   text_layer_set_text(batnum, Battery->text);
#ifdef DEBUG_BATTERY
  printf("[BATT][Battery_Redraw] Textlayer is now  %s", text_layer_get_text(batnum));
#endif
}

// New battery data from the pebble is processed here
static void battery_handle_pebble(BatteryChargeState charge_state) 
{   
  // Debug printout
#ifdef DEBUG_BATTERY
  printf("[BATT][Battery_Handle_Pebble] Pebble battery handler: %d %d",charge_state.charge_percent,(int)charge_state.is_charging);
#endif
  // Save the battery's new data
  m_battery_pebble.level = charge_state.charge_percent;
   
  m_battery_pebble.is_charging = charge_state.is_charging;
  
  // And redraw its new data
  update_battery(&m_battery_pebble, m_watchbattery_layer);
}

void battery_handle_phone(int level,bool is_charging){
  // Debug printout
#ifdef DEBUG_BATTERY
  printf("[BATT][Battery_Handle_Phone] Phone battery handler: %d %d",level,(int)is_charging);
#endif
  m_battery_phone.level = level;
  m_battery_phone.is_charging = is_charging;
  
  update_battery(&m_battery_phone,m_phonebattery_layer);
}

static void battery_save(){
#ifdef DEBUG_BATTERY
  printf("battery_c: Saving phone battery data");
#endif
  persist_write_data(MESSAGE_KEY_BATTERY_STORAGE, &m_battery_phone, sizeof(battery_struct));
}

static void battery_load(){
#ifdef DEBUG_BATTERY
  printf("battery_c: Checking persistent storage for phone battery data");
#endif
  
  if(persist_exists(MESSAGE_KEY_BATTERY_STORAGE)){
#ifdef DEBUG_BATTERY
    printf("battery_c: Phone battery data found, updating display");
#endif
    
    persist_read_data(MESSAGE_KEY_BATTERY_STORAGE, &m_battery_phone, sizeof(battery_struct));
    
    update_battery(&m_battery_phone,m_phonebattery_layer);
  }else{
#ifdef DEBUG_BATTERY
    printf("battery_c: No data found, setting default values");
#endif
    m_battery_phone.level = -1;
    m_battery_phone.is_charging = false;
  }
}

// Init the battery layers and register to the pebbles battery service
void battery_init()
{
#ifdef DEBUG_BATTERY
  printf("battery_c: Initializing");
#endif
  
  m_watchbattery_layer = get_watchbattery_layer();
  m_phonebattery_layer = get_phonebattery_layer();
      
  battery_state_service_subscribe(battery_handle_pebble);
  battery_handle_pebble(battery_state_service_peek());
  
  battery_load();
  
  m_battery_phone.id = 1;
  m_battery_pebble.id = 0;
}

// Unregister from pebbles battery service
void battery_deinit()
{
#ifdef DEBUG_BATTERY
  printf("battery_c: Deinitializing");
#endif
  
  battery_save();
  battery_state_service_unsubscribe();      
}