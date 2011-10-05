/* window.c
 * does window-related things,
 * resizing, transparency, etc..
 */

#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include "common.h"
#include "graphics.h"
#include "mouse.h"

/* sets shit up to be transparent */
void screen_changed (GtkWindow *window, GdkScreen *old_screen, GtkWidget *label) {
	GdkScreen *screen = gtk_widget_get_screen (GTK_WIDGET (window));
	GdkVisual *visual = gdk_screen_get_rgba_visual (screen);

	if (visual == NULL) {
		fprintf(stderr, "Alpha not supported, using fallback.\n");
		visual = gdk_screen_get_system_visual (screen);
	}

	gtk_widget_set_visual (GTK_WIDGET(window), visual);
	
	if (!gtk_widget_is_composited(GTK_WIDGET(window)))
		fprintf(stderr, "Window not composited, transparency disabled.\n");
}

gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer userdata) {
	draw_ruler(widget);
	
	return TRUE;
}

/* loads an image file as a pixbuf (used for icon) */
GdkPixbuf *create_pixbuf(const gchar * filename) {
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

/* rotates the ruler */
void rotate_ruler(GtkWidget *widget) {	
	int width, height;
	gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
	gtk_window_resize(GTK_WINDOW(widget), height, width);
	
	if (ruler_orientation == HORIZONTAL) {
		ruler_orientation = VERTICAL;
		gtk_widget_set_size_request(widget, 50, 100);
	}
	else {
		ruler_orientation = HORIZONTAL;
		gtk_widget_set_size_request(widget, 100, 50);
	}
		
	draw_ruler(widget);
}

GSourceFunc getXCursor() {
	Display *dsp = XOpenDisplay( NULL );
	if( !dsp ){ return FALSE; }

/*	int screenNumber = DefaultScreen(dsp);*/

	XEvent event;

	/* get info about current pointer position */
	XQueryPointer(dsp, RootWindow(dsp, DefaultScreen(dsp)),
	&event.xbutton.root, &event.xbutton.window,
	&event.xbutton.x_root, &event.xbutton.y_root,
	&event.xbutton.x, &event.xbutton.y,
	&event.xbutton.state);
	
	cursor.x = event.xbutton.x;
	cursor.y = event.xbutton.y;
	
	
/*	fprintf(stderr, "Mouse Coordinates: %d %d\n", cursor.x, cursor.y);*/

	XCloseDisplay( dsp );
	
	return TRUE;
}
