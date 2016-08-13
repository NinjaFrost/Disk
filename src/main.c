// Copyright [2015] Pebble Technology

#include <pebble.h>
#include "./layers/time_display.h"
#include "./layers/corners.h"

// Define struct to store colors for each

static Window *s_window;
static Layer *s_layer;

static uint8_t s_hour;
static uint8_t s_minute;
static uint8_t s_second;


// Update the watchface display
static void update_display(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  draw_seconds(ctx, s_second, layer);

  draw_marker_face(ctx, layer);
  
  corner_top_right(ctx);
}

// Update the current time values for the watchface
static void update_time(struct tm *tick_time) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  s_second = tick_time->tm_sec;
  layer_mark_dirty(s_layer);
  update_time_text(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void window_load(Window *window) {
  s_layer = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  layer_add_child(window_get_root_layer(s_window), s_layer);
  layer_set_update_proc(s_layer, update_display);
  
  create_time_face(s_window);
}

static void window_unload(Window *window) {
  layer_destroy(s_layer);
}

static void init(void) {
  s_window = window_create();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_set_background_color(s_window, GColorWhite);
  window_stack_push(s_window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  update_time(localtime(&start));  // NOLINT(runtime/threadsafe_fn)
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}