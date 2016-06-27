#include <pebble.h>
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define PERSIST_KEY 2

static Window *s_main_window;

static TextLayer  *s_time_layer, *s_command1_layer, *s_command2_layer, *s_battery_layer, *s_command3_layer, *s_weather_layer, *s_command4_layer, *s_bluetooth_layer/**, *s_sizeof_layer**/;

static char weather_layer_buffer[32];

//OBSELETE static int charlen;

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
    static const uint32_t segments[] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
    VibePattern pat = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
    };
    vibes_enqueue_custom_pattern(pat);
    text_layer_set_text(s_bluetooth_layer, " ");
  }
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "It is currently 00:0000000";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("It is currently 00:0000000"), "%a %b %d %H:%M %Y", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("It is currently 00:0000000"), "%a %m/%d/%Y %I:%M:%S %p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  // Get weather update every 60 minutes
  if(tick_time->tm_min % 60 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

static GFont s_time_font;
static GFont s_command_font;

/** BACKGROUND IMAGE (OBSELETE)
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
**/

static void main_window_load(Window *window) {

/**  BACKGROUND IMAGE (OBSELETE)
// Create GBitmap, then set to created BitmapLayer
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WHAT_THE_FUCK);
s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
**/

  window_set_background_color(window, GColorBlack);
/** Create time TextLayer
s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
text_layer_set_background_color(s_time_layer, GColorClear);
text_layer_set_text_color(s_time_layer, GColorBlack);
text_layer_set_text(s_time_layer, "00:00");
**/
  //Create command 1 TextLayer
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
  
  //Create Command2 Layer
  s_command2_layer = text_layer_create(GRect(0, 44, 144, 13));
  text_layer_set_background_color(s_command2_layer, GColorClear);
  text_layer_set_text_color(s_command2_layer, GColorWhite);
  text_layer_set_text(s_command2_layer, "root@pebble:~# upower -i");
  
  // Create output TextLayer
  s_battery_layer = text_layer_create(GRect(0, 55, 144, 100));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  
  //Create Command3 Layer
  s_command3_layer = text_layer_create(GRect(0, 72, 144, 22));
  text_layer_set_background_color(s_command3_layer, GColorClear);
  text_layer_set_text_color(s_command3_layer, GColorWhite);
  text_layer_set_overflow_mode(s_command3_layer, GTextOverflowModeFill);
  text_layer_set_text(s_command3_layer, "root@pebble:~# ./weather.sh");
  
  // Create temperature Layer
s_weather_layer = text_layer_create(GRect(0, 94, 144, 50));
text_layer_set_background_color(s_weather_layer, GColorClear);
text_layer_set_text_color(s_weather_layer, GColorWhite);

/**  trying to keep weather variable alive
text_layer_set_text(s_weather_layer, " ");
**/
  
  //Check length of weather
  //OBSELETE charlen = strlen(weather_layer_buffer);
  
  s_command4_layer = text_layer_create(GRect(0, 127, 144, 22));
  //Check length of weather for command4
  //OBSELETE if(charlen<15){
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  //s_command4_layer = text_layer_create(GRect(0, 111, 144, 22));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  //s_command4_layer = text_layer_create(GRect(0, 127, 144, 22));
  }
  //create command4 layer
  text_layer_set_background_color(s_command4_layer, GColorClear);
  text_layer_set_text_color(s_command4_layer, GColorWhite);
  text_layer_set_overflow_mode(s_command4_layer, GTextOverflowModeFill);
  text_layer_set_text(s_command4_layer, "root@pebble:~# adb devices");
  
  s_bluetooth_layer = text_layer_create(GRect(0, 149, 144, 22));
  
  //Check length of weather for bluetooth
  //OBSELETE if(charlen<15){
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  //s_bluetooth_layer = text_layer_create(GRect(0, 132, 144, 22));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  //s_bluetooth_layer = text_layer_create(GRect(0, 149, 144, 22));
  }
  //Create bluetooth Layer
  text_layer_set_background_color(s_bluetooth_layer, GColorClear);
  text_layer_set_text_color(s_bluetooth_layer, GColorWhite);
  text_layer_set_overflow_mode(s_bluetooth_layer, GTextOverflowModeFill);
  
  /**BEGIN DEBUG CODE
  //Create sizeof Layer
  s_sizeof_layer = text_layer_create(GRect(50, 150, 144, 22));
  text_layer_set_background_color(s_sizeof_layer, GColorClear);
  text_layer_set_text_color(s_sizeof_layer, GColorWhite);
  text_layer_set_overflow_mode(s_sizeof_layer, GTextOverflowModeFill);
  END DEBUG CODE **/
  

  // Get the current battery level
  battery_handler(battery_state_service_peek());
  
  // Show current connection state
  bt_handler(bluetooth_connection_service_peek());
  
  
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_16));
  s_command_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10));
  
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_battery_layer, s_time_font);
  text_layer_set_font(s_weather_layer, s_time_font);
  text_layer_set_font(s_command1_layer, s_command_font);
  text_layer_set_font(s_command2_layer, s_command_font);
  text_layer_set_font(s_command3_layer, s_command_font);
  text_layer_set_font(s_command4_layer, s_command_font);
  text_layer_set_font(s_bluetooth_layer, s_command_font);
  //DEBUG CODE text_layer_set_font(s_sizeof_layer, s_command_font);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command3_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_command4_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bluetooth_layer));
  //DEBUG CODE layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_sizeof_layer));
}


static void main_window_unload(Window *window) {
  
  /** BACKGROUND IMAGE (OBSELETE)
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  **/
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  text_layer_destroy(s_battery_layer);
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[32];
  static char conditions_buffer[32];
  static char sizeof_buffer[32];

  
  // Read first item
  Tuple *t = dict_read_first(iterator);
  
  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
  case KEY_TEMPERATURE:
      //persist_write_int(KEY_TEMPERATURE, (int)t->value->int32);
  snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
  break;
  case KEY_CONDITIONS:
          //persist_write_string(KEY_CONDITIONS, t->value->cstring);
  snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
  break;
  default:
  APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
  break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  /**BEGIN DEBUG CODE
  snprintf(conditions_buffer, sizeof(conditions_buffer), "PENIS SHUTUP IM MATURE OK");
  **/
  // Assemble full string and display
  //int persisttemp = persist_read_int(KEY_TEMPERATURE);
  //char persistcond[32];
  //persist_read_string(KEY_CONDITIONS, persistcond, sizeof(persistcond))
  if(!strcmp(conditions_buffer,"Showers in the Vicinity")) 
    snprintf(conditions_buffer, sizeof(conditions_buffer), "Showers Nearby");
  
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "AAAAAAAAAAAAAAAAAAA");
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "A");
  //persist_write_string(PERSIST_WEATHER_KEY, 'test');
  
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
  
  /** BEGIN DEBUG CODE
  snprintf(sizeof_buffer, sizeof(sizeof_buffer), "%i", strlen(weather_layer_buffer));
  text_layer_set_text(s_sizeof_layer, sizeof_buffer);
  END DEBUG CODE **/
  
  //set length of charlen
  //OBSELETEcharlen = strlen(weather_layer_buffer);
  
  //Update command4 position
  //OBSELETE if(charlen<15){
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  //s_command4_layer = text_layer_create(GRect(0, 111, 144, 22));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_command4_layer));
  //s_command4_layer = text_layer_create(GRect(0, 127, 144, 22));
  }
  
  //Update bluetooth position
  //OBSELETE if(charlen<15){
  if(strlen(weather_layer_buffer)<15){
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  //s_bluetooth_layer = text_layer_create(GRect(0, 132, 144, 22));
  }
  else{
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
    layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
  //s_bluetooth_layer = text_layer_create(GRect(0, 149, 144, 22));
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
    // Use 24 hour format (no seconds)
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
    
    //Update command4 position
    //OBSELETE if(charlen<15){
    if(strlen(weather_layer_buffer)<15){
      layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 111, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_command4_layer));
      //s_command4_layer = text_layer_create(GRect(0, 111, 144, 22));
    }
    else{
      layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 127, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_command4_layer));
      //s_command4_layer = text_layer_create(GRect(0, 127, 144, 22));
    }
  
    //Update bluetooth position
    //OBSELETE if(charlen<15){
    if(strlen(weather_layer_buffer)<15){
      layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 132, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
      //s_bluetooth_layer = text_layer_create(GRect(0, 132, 144, 22));
    }
    else{
      layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 149, 144, 22));
      layer_mark_dirty(text_layer_get_layer(s_bluetooth_layer));
      //s_bluetooth_layer = text_layer_create(GRect(0, 149, 144, 22));
    }
  }
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
  persist_write_string(PERSIST_KEY, weather_layer_buffer);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}