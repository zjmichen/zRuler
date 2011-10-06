/* mouse.c
 * handles mouse events
 */

#include <gtk/gtk.h>
#include "common.h"
#include "mouse.h"
#include "graphics.h"
#include "window.h"

gboolean mouse_button(GtkWidget *widget, GdkEvent *event, gpointer userdata) {
	cursor_last_pressed.x = event->button.x;
	cursor_last_pressed.y = event->button.y;
	
	if (event->button.button == 3) {
		view_popup_menu(widget, (GdkEventButton*)event, userdata);
		return TRUE;
	}
	
	if (ruler_orientation == HORIZONTAL) {
		if (is_in_button(cursor_last_pressed.x, cursor_last_pressed.y))
			rotate_ruler(widget);
	}
	else {
		/* the cursor positions are swapped when vertical */
		if (is_in_button(cursor_last_pressed.y, cursor_last_pressed.x))
			rotate_ruler(widget);
	}

	return TRUE;
}

gboolean mouse_motion(GtkWidget *widget, GdkEventMotion *event, gpointer userdata) {    
	if (resizing_ruler(widget)) {
		if (ruler_orientation == HORIZONTAL) {
			gtk_widget_set_size_request(widget, 100, 50);
			gdk_window_begin_resize_drag(gtk_widget_get_window(widget), 
										GDK_WINDOW_EDGE_EAST,
										1, event->x_root, event->y_root, event->time);
		}
		else {
			gtk_widget_set_size_request(widget, 50, 100);
			gdk_window_begin_resize_drag(gtk_widget_get_window(widget), 
										GDK_WINDOW_EDGE_SOUTH,
										1, event->x_root, event->y_root, event->time);
		}	
	}
	else {
		gdk_window_begin_move_drag(gtk_widget_get_window(widget),
									1, event->x_root, event->y_root, event->time);
	}
	
	return TRUE;
}

gboolean resizing_ruler(GtkWidget *widget) {
	int w_width, w_height;	// window dimensions
    gtk_window_get_size(GTK_WINDOW(widget), &w_width, &w_height);
    
	if (cursor_last_pressed.x >= w_width - 10 && ruler_orientation == HORIZONTAL)
		return TRUE;
	if (cursor_last_pressed.y >= w_height - 10 && ruler_orientation == VERTICAL)
		return TRUE;

	return FALSE;
}
