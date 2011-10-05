void draw_ruler(GtkWidget*);
void draw_background(cairo_t*);
void draw_lines(cairo_t*);
void draw_numbers(cairo_t*);
void draw_cursor_mark(cairo_t*, int);
void draw_position_notifier(cairo_t*, int);
void draw_cap(cairo_t*);
void draw_rotate_button(cairo_t*);
void draw_translucent_overlay(cairo_t*);
void draw_outline(cairo_t*);
void rendertext(cairo_t*, char*, int, int);
gboolean is_in_button(int, int);
