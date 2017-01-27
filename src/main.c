#include <pebble.h>
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define PERSIST_KEY 2
#define VIBRATE_CYCLE 	10

static Window *s_main_window;

static TextLayer  *s_time_layer, *s_command1_layer, *s_command2_layer, *s_battery_layer, *s_command3_layer, *s_weather_layer, *s_command4_layer, *s_bluetooth_layer;

static char weather_layer_buffer[32];


static void battery_handler(BatteryChargeState new_state) {
  // Write to buffer and display
  static char s_battery_buffer[32];
  snprintf(s_battery_buffer, sizeof(s_battery_buffer), "Bat 0: %d%%", new_state.charge_percent);
  text_layer_set_text(s_battery_layer, s_battery_buffer);
}

static void bt_handler(bool connected) {
  // Show current connection state
  if (connected) {
    text_layer_set_text(s_bluetooth_layer, "19485e21    device");
  } else {
	for(int i = 0; i < VIBRATE_CYCLE; i++){
		vibes_short_pulse();
	}
    text_layer_set_text(s_bluetooth_layer, " ");
  }
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a term buffer
  static char buffer[30];

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style()) {
    // Use 24 hour format
    strftime(buffer, sizeof(buffer), "%a %b %d %H:%M %Y", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %I:%M:%S %p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  // Get weather update every 60 minutes
  if(!(tick_time->tm_min % 60)) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message
    app_message_outbox_send();
  }
}

static GFont s_time_font;
static GFont s_command_font;

static void main_window_load(Window *window) {

  window_set_background_color(window, GColorBlack);

  //Create command 1 TextLayer (date command)
  s_command1_layer = text_layer_create(GRect(0, 0, 144, 13));
  text_layer_set_background_color(s_command1_layer, GColorClear);
  text_layer_set_text_color(s_command1_layer, GColorWhite);
  text_layer_set_text(s_command1_layer, "root@pebble:~# date");
  
  // Create time TextLayer
  //s_time_layer = text_layer_create(GRect(0, 13, 144, 100));
  s_time_layer = text_layer_create(GRect(0, 11, 144, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  //text_layer_set_text(s_time_layer, "00:00");
  
  //Create Command2 Layer (battery info)
  s_command2_layer = text_layer_create(GRect(0, 44, 144, 13));
  text_layer_set_background_color(s_command2_layer, GColorClear);
  text_layer_set_text_color(s_command2_layer, GColorWhite);
  text_layer_set_text(s_command2_layer, "root@pebble:~# upower -i");
  
  // Create battery info TextLayer
  s_battery_layer = text_layer_create(GRect(0, 55, 144, 100));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  
  //Create Command3 TextLayer (Weather info)
  s_command3_layer = text_layer_create(GRect(0, 72, 144, 22));
  text_layer_set_background_color(s_command3_layer, GColorClear);
  text_layer_set_text_color(s_command3_layer, GColorWhite);
  text_layer_set_overflow_mode(s_command3_layer, GTextOverflowModeFill);
  text_layer_set_text(s_command3_layer, "root@pebble:~# ./weather.sh");
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 94, 144, 50));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);

  //Create Command4 TextLayer (Bluetooth Status)
  s_command4_layer = text_layer_create(GRect(0, 127, 144, 22));
  //Move Command4 based on size of weather string to prevent overlap
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  }
  text_layer_set_background_color(s_command4_layer, GColorClear);
  text_layer_set_text_color(s_command4_layer, GColorWhite);
  text_layer_set_overflow_mode(s_command4_layer, GTextOverflowModeFill);
  text_layer_set_text(s_command4_layer, "root@pebble:~# adb devices");
  
  //Create Bluetooth Status Layer
  s_bluetooth_layer = text_layer_create(GRect(0, 149, 144, 22));
  //Move Bluetooth Status based on size of weather string to prevent overlap
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  }
  text_layer_set_background_color(s_bluetooth_layer, GColorClear);
  text_layer_set_text_color(s_bluetooth_layer, GColorWhite);
  text_layer_set_overflow_mode(s_bluetooth_layer, GTextOverflowModeFill);
  
#ifdef DEBUG
  //Create sizeof Layer
  s_sizeof_layer = text_layer_create(GRect(50, 150, 144, 22));
  text_layer_set_background_color(s_sizeof_layer, GColorClear);
  text_layer_set_text_color(s_sizeof_layer, GColorWhite);
  text_layer_set_overflow_mode(s_sizeof_layer, GTextOverflowModeFill);
#endif //DEBUG
  
  // Get the current battery level
  battery_handler(battery_state_service_peek());
  
  // Show current connection state
  bt_handler(bluetooth_connection_service_peek());
  
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_16));
  s_command_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10));
  
  // Apply to TextLayer
  text_layer_set_font(s_command1_layer, s_command_font);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_command2_layer, s_command_font);
  text_layer_set_font(s_battery_layer, s_time_font);
  text_layer_set_font(s_command3_layer, s_command_font);
  text_layer_set_font(s_weather_layer, s_time_font);
  text_layer_set_font(s_command4_layer, s_command_font);
  text_layer_set_font(s_bluetooth_layer, s_command_font);
  
#ifdef DEBUG
  text_layer_set_font(s_sizeof_layer, s_command_font);
#endif //DEBUG

  // Add all layers to Window root
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command3_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command4_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bluetooth_layer));
#ifdef DEBUG
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_sizeof_layer));
#endif //DEBUG
}


static void main_window_unload(Window *window) {
  
  // Unload GFonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_command_font);
  
  //Destroy textlayers
  text_layer_destroy(s_command1_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_command2_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_command3_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_command4_layer);
  text_layer_destroy(s_bluetooth_layer);
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[32];
  static char conditions_buffer[32];
  static char sizeof_buffer[32];

  // Read first item
  Tuple *t = dict_read_first(iterator);
  
  // Iterate through all items
  while(t != NULL) {
    // Check which key was recieved 
    switch(t->key) {
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
		snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
		break;
	  default:
		APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
		break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
#ifdef DEBUG
  snprintf(conditions_buffer, sizeof(conditions_buffer), "PENIS SHUTUP IM MATURE OK");
#endif //DEBUG

  //Handle edge case, there isn't enough room to fit this string.
  if(!strcmp(conditions_buffer,"Showers in the Vicinity")) 
    snprintf(conditions_buffer, sizeof(conditions_buffer), "Showers Nearby");
  
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
  
#ifdef DEBUG
  snprintf(sizeof_buffer, sizeof(sizeof_buffer), "%i", strlen(weather_layer_buffer));
  text_layer_set_text(s_sizeof_layer, sizeof_buffer);
#endif //DEBUG

  //Move lower textlayers based on size of weather string
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
	layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
	layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
	layer_mark_dirty(text_layer_get_layer(s_command4_layer));
	layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  if(clock_is_24h_style() == true) {
    // Use 24 hour format (no seconds, saves battery)
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  } else {
    // Use 12 hour format (seconds)
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  }
  
  // Subscribe to the Battery State Service
  battery_state_service_subscribe(battery_handler);
  
  // Subscribe to Bluetooth updates
  bluetooth_connection_service_subscribe(bt_handler);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  

  //Check persistent storage for weather
  if (persist_exists(PERSIST_KEY)) {
    persist_read_string(PERSIST_KEY, weather_layer_buffer, sizeof(weather_layer_buffer));
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
    
    if(strlen(weather_layer_buffer)<15){
      layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_command4_layer));
    }
    else{
      layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_command4_layer));
    }
  
    if(strlen(weather_layer_buffer)<15){
      layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
    }
    else{
      layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
    }
  }
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
    
	// Write weather data to storage
	persist_write_string(PERSIST_KEY, weather_layer_buffer);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}