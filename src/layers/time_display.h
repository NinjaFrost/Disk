// Time text
void create_time_face(Window *main_window);
void update_time_text(struct tm *tick_time);


// Time arcs 
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer);
void draw_marker_face(GContext *ctx, Layer *layer);