#include <pebble.h>

#define ANGLE(n) (TRIG_MAX_ANGLE / n)
#define TIME_START_ANGLE(time) (time * (TRIG_MAX_ANGLE / 60))
#define HOUR_START_ANGLE(hour) (hour * (TRIG_MAX_ANGLE / 12))
#define TIME_ANGLE(time) (time * (TRIG_MAX_ANGLE / 60))+(TRIG_MAX_ANGLE / 60)
#define HOUR_ANGLE(hour) (hour * (TRIG_MAX_ANGLE / 12))+(TRIG_MAX_ANGLE / 12)

int marker_length = 6;
int angle_time;
int angle_hour;
int angle_clock_time;
int angle_clock_hour;

static TextLayer *s_time_layer;
static char buffer[] = "00:00";

// Gets layer from text_layer
Layer *text_layer_to_layer(TextLayer* text_layer) {
    return (Layer*)text_layer;
}

// Centers rect to screen_center
static GPoint rect_centered_origin(GPoint screen_center, GSize box_size) {
  // Corrects rect origin to center of rect
  int  x = (screen_center.x - (box_size.w / 2));
  int  y = (screen_center.y - (box_size.h / 2));
  
    return (GPoint) {
                    (x),
                    (y),
    };
}

void create_time_face(Window *main_window){
  // Finds bounds of display and finds center point
  const GRect screen_bounds = layer_get_bounds(window_get_root_layer(main_window));
  const GPoint center = grect_center_point(&screen_bounds);
  
  // Finds bounds of text displayed
  GSize bounds = graphics_text_layout_get_content_size(buffer, 
                                                       fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS), 
                                                       layer_get_bounds(window_get_root_layer(main_window)), 
                                                       GTextOverflowModeWordWrap, GTextAlignmentCenter);
  
  // Create time TextLayer
  s_time_layer = text_layer_create((GRect) {rect_centered_origin(center, bounds), .size = { bounds.w, bounds.h}} );
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Adds s_time_layer to main layer
  layer_add_child(window_get_root_layer(main_window), text_layer_get_layer(s_time_layer));
}

void update_time_text(struct tm *tick_time){
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true){
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    // Removes leading zero
    if (buffer[0] == '0') {
			memmove(&buffer[0], &buffer[1], sizeof(buffer) - 1);
		}

  }
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}


/*     */



// Draw an arc with given inner/outer radii
static void draw_arc(GContext *ctx,
                     GRect rect,
                     uint16_t thickness,
                     uint32_t start_angle,
                     uint32_t end_angle) {

    graphics_fill_radial(ctx,
                         rect,
                         GOvalScaleModeFitCircle,
                         thickness,
                         start_angle,
                         end_angle);
  
}


static GRect calculate_rect(Layer *layer, uint8_t padding) {
  return grect_inset(layer_get_bounds(layer), GEdgeInsets(padding));
}

void draw_marker_face(GContext *ctx, Layer *layer) {
  for(angle_hour=0;angle_hour<=ANGLE(1);angle_hour+=ANGLE(12)){
    for(angle_time=0;angle_time<=ANGLE(1);angle_time+=ANGLE(60)){
      graphics_draw_line(ctx, gpoint_from_polar(calculate_rect(layer, 1), GOvalScaleModeFitCircle, angle_time), gpoint_from_polar(calculate_rect(layer, 1), GOvalScaleModeFitCircle, angle_time));
      angle_clock_time = angle_time;
    }
    
    graphics_draw_line(ctx, gpoint_from_polar(calculate_rect(layer, 0), GOvalScaleModeFitCircle, angle_hour), gpoint_from_polar(calculate_rect(layer, 3), GOvalScaleModeFitCircle, angle_hour));
  }
}


// Handle representation for seconds
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer) {
  draw_arc(ctx, calculate_rect(layer, 0), 3, TIME_START_ANGLE(seconds), TIME_ANGLE(seconds));
}