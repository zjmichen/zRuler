/* zRuler
 * Created by Zack Michener
 * September 27, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include "common.h"
#include "window.h"
#include "graphics.h"
#include "mouse.h"

Point cursor;
Point cursor_last_pressed;
enum Orientation ruler_orientation = HORIZONTAL;
GtkWidget *window;
gboolean onTop = FALSE;

int main(int argc, char** argv) {
	gtk_init(&argc, &argv);
	
	/* make a window! */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Screen Ruler");
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
	gtk_widget_set_app_paintable(window, TRUE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 50);
	gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
	gtk_window_set_default_icon(create_pixbuf("zruler.png"));
	
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK | 
									GDK_BUTTON_MOTION_MASK);
	
	/* register me some callbacks */
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
							G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "draw",
							G_CALLBACK(expose), NULL);
	g_signal_connect(G_OBJECT(window), "screen-changed", 
							G_CALLBACK(screen_changed), NULL);
	g_signal_connect(G_OBJECT(window), "button-press-event", 
							G_CALLBACK(mouse_button), NULL);
	g_signal_connect(G_OBJECT(window), "motion-notify-event", 
							G_CALLBACK(mouse_motion), NULL);
	
	/* gets the _global_ cursor position constantly */
	g_timeout_add(100, getXCursor, NULL);
	
	/* get things going */
	screen_changed((GtkWindow*)window, NULL, NULL);	
	gtk_widget_show_all(window);
	gtk_main();

	return EXIT_SUCCESS;
}
