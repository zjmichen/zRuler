/****************************************************************************\
* Copyright 2011 Zack Michener <zjmichen@unca.edu>                           *
*                                                                            *
* This file is part of zRuler.                                               *
*                                                                            *
* zRuler is free software; you can redistribute it and/or modify             *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation; either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* zRuler is distributed in the hope that it will be useful,                  *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU Library General Public License for more details.                       *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with zRuler; if not, write to the Free Software                      *
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
\****************************************************************************/

#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include "window.h"
#include "common.h"
#include "graphics.h"
#include "mouse.h"

/* sets shit up to be transparent */
void screenChanged (GtkWindow *window, GdkScreen *old_screen, GtkWidget *label) {
	GdkScreen *screen = gtk_widget_get_screen (GTK_WIDGET (window));
	GdkVisual *visual = gdk_screen_get_rgba_visual (screen);	// ask for transparent visual

	if (visual == NULL) {
		fprintf(stderr, "Alpha not supported, using fallback.\n");
		visual = gdk_screen_get_system_visual (screen);
	}

	gtk_widget_set_visual (GTK_WIDGET(window), visual);			// apply transparent visual
	
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

/* rotates the ruler, logically and actually */
void rotateRuler(GtkWidget *widget) {	
	int width, height;
	
	widget = window;
	
	gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
	gtk_window_resize(GTK_WINDOW(widget), height, width);
	
	if (rulerOrientation == HORIZONTAL) {
		rulerOrientation = VERTICAL;
		gtk_widget_set_size_request(widget, 50, 100);	// minimum sizes
	}
	else {
		rulerOrientation = HORIZONTAL;
		gtk_widget_set_size_request(widget, 100, 50);
	}
		
	drawRuler(widget);
}

/* finds the root position of the cursor, stores it in cursor */
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

/* creates and displays the context menu */
gboolean viewPopupMenu (GtkWidget *widget, GdkEventButton *event, gpointer userdata) {
	GtkWidget *menu, 
			  *menuitem_quit, 
			  *menuitem_stay_on_top, 
			  *menuitem_about,
			  *menuitem_rotate;

	/* the actual menu */
	menu = gtk_menu_new();

	/* the items in the menu */
	menuitem_stay_on_top = gtk_check_menu_item_new_with_label("Always on top");
	gtk_check_menu_item_set_active ((GtkCheckMenuItem*)menuitem_stay_on_top, onTop);
	menuitem_rotate = gtk_menu_item_new_with_label("Rotate");
	menuitem_about = gtk_menu_item_new_with_label("About");
	menuitem_quit = gtk_menu_item_new_with_label("Quit");
	
	/* make the menu items do things */
	g_signal_connect(menuitem_stay_on_top, "activate",
					G_CALLBACK(stayOnTop), widget);
	g_signal_connect(menuitem_rotate, "activate",
					G_CALLBACK(rotateRuler), widget);
	g_signal_connect(menuitem_about, "activate",
					G_CALLBACK(viewAboutDialog), widget);
	g_signal_connect(menuitem_quit, "activate",
		             gtk_main_quit, widget);
	
	/* put it all together */
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_stay_on_top);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_rotate);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_about);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem_quit);

	gtk_widget_show_all(menu);

	/* boom, menu. */
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, userdata,
					(event != NULL) ? event->button : 0,
					gdk_event_get_time((GdkEvent*)event));
		           
	return TRUE;
}

/* creates and displays the about window */
void viewAboutDialog(GtkWidget *widget, gpointer data) {
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("zruler.png", NULL);
	
	GtkWidget *dialog  = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "zRuler");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), VERSION);
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "© Zack Michener 2011");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "A simple, yet stylish screen ruler.");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/zjmichen/ruler");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

/* toggles the always-on-top mode */
gboolean stayOnTop(GtkWidget *widget, GdkEvent *event, gpointer userdata) {
	onTop = !onTop;
	gtk_window_set_keep_above(GTK_WINDOW(window), onTop);
	
	return TRUE;
}
