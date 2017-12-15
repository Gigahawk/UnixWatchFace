#include "window.h"

static Window *s_window;

static GFont s_time_font;
static GFont s_command_font;
static GFont s_battery_font;

static TextLayer *s_command1_layer;
static TextLayer *s_time_layer;
static TextLayer *s_command2_layer;
static TextLayer *s_watchbattery_layer;
static TextLayer *s_phonebattery_layer;
static TextLayer *s_command3_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_command4_layer;
static TextLayer *s_bluetooth_layer;

static TextLayer *s_sleep_layer;

void setup_text_layer(TextLayer* target_layer,GColor background_color, GColor text_color, char* text, GTextAlignment alignment,  GFont font)
{
  text_layer_set_background_color(target_layer, background_color);
  text_layer_set_text_color(target_layer, text_color);
  text_layer_set_text(target_layer, text);
  text_layer_set_text_alignment(target_layer, alignment);
  text_layer_set_font(target_layer, font);
}


static void initialise_ui(void)
{
  is_flashlight = false;
#ifdef DEBUG_MAINWINDOW
  printf("window_c: Creating window");
#endif
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
   
#ifdef DEBUG_MAINWINDOW
  printf("window_c: Loading fonts");
#endif

  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_16));
  s_command_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10));
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_12));
  
#ifdef DEBUG_MAINWINDOW
  printf("window_c: Creating layers");
#endif
  s_command1_layer = text_layer_create(GRect(0, 0, 144, 13));
  s_time_layer = text_layer_create(GRect(0, 11, 144, 100));
  s_command2_layer = text_layer_create(GRect(0, 44, 144, 13));
  s_watchbattery_layer = text_layer_create(GRect(0, 55, 72, 16));
  s_phonebattery_layer = text_layer_create(GRect(72, 55, 72, 16));
  s_command3_layer = text_layer_create(GRect(0, 67, 144, 22));
  s_weather_layer = text_layer_create(GRect(0, 88, 144, 50));
  s_command4_layer = text_layer_create(GRect(0, 88, 144, 22));
  s_bluetooth_layer = text_layer_create(GRect(0, 110, 144, 22));
  
  // Temporary health debug layers
  s_sleep_layer = text_layer_create(GRect(0, 145, 144, 22));
  
  
#ifdef DEBUG_MAINWINDOW
  printf("window_c: Setting up layers");
#endif

  setup_text_layer(s_command1_layer,GColorClear,GColorWhite,"root@pebble:~# date",GTextAlignmentLeft, s_command_font);
  setup_text_layer(s_time_layer,GColorClear,GColorWhite,"Loading...",GTextAlignmentLeft,s_time_font);
  setup_text_layer(s_command2_layer,GColorClear,GColorWhite,"root@pebble:~# upower -i",GTextAlignmentLeft,s_command_font);
  setup_text_layer(s_watchbattery_layer,GColorClear,GColorWhite,"Bat0: --",GTextAlignmentLeft,s_battery_font);
  setup_text_layer(s_phonebattery_layer,GColorClear,GColorWhite,"Bat1: --",GTextAlignmentLeft,s_battery_font);
  setup_text_layer(s_command3_layer,GColorClear,GColorWhite,"root@pebble:~# ./weather.sh --gps",GTextAlignmentLeft,s_command_font);
  setup_text_layer(s_weather_layer,GColorClear,GColorWhite," ",GTextAlignmentLeft,s_time_font);
  setup_text_layer(s_command4_layer,GColorClear,GColorWhite,"root@pebble:~# adb devices",GTextAlignmentLeft,s_command_font);
  setup_text_layer(s_bluetooth_layer,GColorClear,GColorWhite,"",GTextAlignmentLeft,s_command_font);
  
  setup_text_layer(s_sleep_layer,GColorClear,GColorWhite,"0",GTextAlignmentLeft,s_command_font);
  
#ifdef DEBUG_MAINWINDOW
  printf("[MAIN][initialise_ui] Add layers");
#endif

    
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_command1_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_command2_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_watchbattery_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_phonebattery_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_command3_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_command4_layer));
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_bluetooth_layer));
  
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_sleep_layer));

#ifdef DEBUG_MAINWINDOW
  printf("[MAIN][initialise_ui] Finished creating");
#endif
}

static void destroy_ui(void) 
{
  //Unload GFonts
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_battery_font);
  fonts_unload_custom_font(s_command_font);
  
  //Destroy textlayers
  text_layer_destroy(s_command1_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_command2_layer);
  text_layer_destroy(s_watchbattery_layer);
  text_layer_destroy(s_phonebattery_layer);
  text_layer_destroy(s_command3_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_command4_layer);
  text_layer_destroy(s_bluetooth_layer);
  
  // Destroy window
  window_destroy(s_window);  
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) 
{
  destroy_ui();
}

// Display the main window
void show_mainwindow(void) 
{
   initialise_ui();

   window_set_window_handlers(s_window, (WindowHandlers) {
      .unload = handle_window_unload,
   });
   window_stack_push(s_window, true);
}

// Hide the main window
void hide_mainwindow(void) 
{
   // Save vars to the storage
   //SaveConfigToStorage();
   
   // Kill the window
   window_stack_remove(s_window, true);
}

void refresh_display(void)
{
  if (strlen(text_layer_get_text(s_weather_layer)) < 15) {
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 105, 144, 22));
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 126, 144, 22));
  } else {
    //127, 149
    layer_set_frame(text_layer_get_layer(s_command4_layer), GRect(0, 121, 144, 22));
    layer_set_frame(text_layer_get_layer(s_bluetooth_layer), GRect(0, 143, 144, 22));
  }
    
  layer_mark_dirty(window_get_root_layer(s_window));
}

void toggle_flashlight(void){
  vibes_cancel();

  if (is_flashlight) {
    window_set_background_color(s_window, GColorBlack);
    light_enable(false);
    is_flashlight = false;
  } else {
    window_set_background_color(s_window, GColorWhite);
    light_enable(true);
    is_flashlight = true;
  }
  
  vibes_long_pulse();
}

TextLayer* get_time_layer(){ return s_time_layer; }
TextLayer* get_watchbattery_layer(){ return s_watchbattery_layer; }
TextLayer* get_phonebattery_layer(){ return s_phonebattery_layer; }
TextLayer* get_weather_command_layer() { return s_command3_layer; }
TextLayer* get_weather_layer(){ return s_weather_layer; }
TextLayer* get_bluetooth_layer(){ return s_bluetooth_layer; }

TextLayer* get_sleep_layer(){ return s_sleep_layer; }
