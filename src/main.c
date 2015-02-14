#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

// Yadonring
static int s_yadonring_frame_counter = 0;
static BitmapLayer *s_yadonring_layer;
static GBitmap *s_yadonring_bitmap;

static void update_yadonring() {
  GBitmap *old_bitmap = s_yadonring_bitmap;
  switch(s_yadonring_frame_counter) {
    case 0:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_01_WHITE); break;
    case 1:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_02_WHITE); break;
    case 2:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_03_WHITE); break;
    case 3:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_04_WHITE); break;
    case 4:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_05_WHITE); break;
    case 5:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_06_WHITE); break;
    case 6:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_07_WHITE); break;
    case 7:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_08_WHITE); break;
    case 8:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_09_WHITE); break;
    case 9:   s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_10_WHITE); break;
    case 10:  s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_11_WHITE); break;
    case 11:  s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_00_WHITE); break;
  }
  
  bitmap_layer_set_bitmap(s_yadonring_layer, s_yadonring_bitmap);
  gbitmap_destroy(old_bitmap);
  
  s_yadonring_frame_counter++;
  if (s_yadonring_frame_counter == 12)
    s_yadonring_frame_counter = 0;
  
  app_timer_register(1000, update_yadonring, NULL);
}

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  if(clock_is_24h_style() == true)
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  else
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Time layer
  s_time_layer = text_layer_create(GRect(0, 62, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Slowpoke Ring
  s_yadonring_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FRAME_00_WHITE);
  s_yadonring_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_background_color(s_yadonring_layer, GColorBlack);
  bitmap_layer_set_bitmap(s_yadonring_layer, s_yadonring_bitmap);

  // Add children
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_yadonring_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(s_yadonring_bitmap);
  bitmap_layer_destroy(s_yadonring_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  app_timer_register(1000, update_yadonring, NULL);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}