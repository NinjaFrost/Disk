#include <pebble.h>

static Layer *layer_top_right;
static Layer *layer_top_left;
static Layer *layer_bottom_right;
static Layer *layer_bottom_left;

int top_left = 1;
int top_right = 2;
int bottom_right = 3;
int bottom_left = 4;

GSize corner_rect = (GSize) {
                            .w = 28
                            .h
}

static GPoint corner_alignment(Layer *layer, GSize rect_size, int8_t alignment){
  const GRect layer_bounds = layer_get_bounds(layer);
  // Top left
  if(alignment == 1){
    return (GPoint) {
                      0,
                      0
      };
    } 
  // Top right
  else if(alignment == 2){
    int x = (layer_bounds.size.w - rect_size.w);
    
    return (GPoint) {
                      (x),
                      0
      };
  }
  // Bottom right
  else if(alignment == 3){
    int x = (layer_bounds.size.w - rect_size.w);
    int y = (layer_bounds.size.h - rect_size.h);
    
    return (GPoint) {
                      (x),
                      (y),
      };
  }
  // Bottom left
  else{
    int y = (layer_bounds.size.h - rect_size.h);
    
    return (GPoint) {
                       0,
                      (y)
      };
    };
}

void corner_top_right(GContext *ctx, Layer *layer){
  graphics_draw_rect(ctx, (GRect) {corner_alignment(layer, (28, 28), top_right), .size = {28, 28}});
}

void corner_top_left(){
  
}

void corner_bottom_right(){
  
}

void corner_bottom_left(){
  
}
