/* window.c
 * does window-related things,
 * resizing, transparency, etc..
 */

#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include "window.h"
#include "common.h"
#include "graphics.h"
#include "mouse.h"

/* sets shit up to be transparent */
void screenChanged (GtkWindow *window, GdkScreen *old_screen, GtkWidget *label) {
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
	drawRuler(widget);
	
	return TRUE;
}

/* loads an image file as a pixbuf (used for icon) */
GdkPixbuf *createPixbuf(const gchar * filename) {
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
void rotateRuler(GtkWidget *widget) {	
	int width, height;
	gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
	gtk_window_resize(GTK_WINDOW(widget), height, width);
	
	if (rulerOrientation == HORIZONTAL) {
		rulerOrientation = VERTICAL;
		gtk_widget_set_size_request(widget, 50, 100);
	}
	else {
		rulerOrientation = HORIZONTAL;
		gtk_widget_set_size_request(widget, 100, 50);
	}
		
	drawRuler(widget);
}

gboolean getXCursor() {
	Display *dsp = XOpenDisplay( NULL );
	if( !dsp ){ return FALSE; }

	XEvent event;

	/* get info about current pointer position */
	XQueryPointer(dsp, RootWindow(dsp, DefaultScreen(dsp)),
		&event.xbutton.root, &event.xbutton.window,
		&event.xbutton.x_root, &event.xbutton.y_root,
		&event.xbutton.x, &event.xbutton.y,
		&event.xbutton.state);
	
	cursor.x = event.xbutton.x;
	cursor.y = event.xbutton.y;
	drawRuler(window);

	XCloseDisplay( dsp );
	
	return TRUE;
}

gboolean viewPopupMenu (GtkWidget *widget, GdkEventButton *event, gpointer userdata) {
	GtkWidget *menu, *menuitem_quit, *menuitem_stay_on_top;

	menu = gtk_menu_new();

	menuitem_stay_on_top = gtk_check_menu_item_new_with_label("Always on top");
	gtk_check_menu_item_set_active ((GtkCheckMenuItem*)menuitem_stay_on_top, onTop);
	menuitem_quit = gtk_menu_item_new_with_label("Quit");
	

	g_signal_connect(menuitem_stay_on_top, "activate",
					G_CALLBACK(stayOnTop), widget);
	g_signal_connect(menuitem_quit, "activate",
		             gtk_main_quit, widget);
	
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_stay_on_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_quit);

	gtk_widget_show_all(menu);

	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, userdata,
					(event != NULL) ? event->button : 0,
					gdk_event_get_time((GdkEvent*)event));
		           
	return TRUE;
}

gboolean stayOnTop(GtkWidget *widget, GdkEvent *event, gpointer userdata) {
	onTop = !onTop;
	gtk_window_set_keep_above(GTK_WINDOW(window), onTop);
	
	return TRUE;
}
