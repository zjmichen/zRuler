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

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include "common.h"
#include "window.h"
#include "graphics.h"
#include "mouse.h"

const char *VERSION = "v1.0";
Point cursor;						// current position of cursor, always up to date
Point cursorLastPressed;			// position of last mouse pressed event
enum Orientation rulerOrientation = HORIZONTAL;
GtkWidget *window;
gboolean onTop = FALSE;				// always on top flag

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
	gtk_window_set_default_icon(createPixbuf("zruler.png"));
	
	/* hey, gtk, you have events to listen for! */
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK | 
									GDK_BUTTON_MOTION_MASK);
	
	/* register me some callbacks */
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
							G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(window, "draw",
							G_CALLBACK(expose), NULL);
	g_signal_connect(G_OBJECT(window), "screen-changed", 
							G_CALLBACK(screenChanged), NULL);
	g_signal_connect(G_OBJECT(window), "button-press-event", 
							G_CALLBACK(mouseButtonFilter), NULL);
	g_signal_connect(G_OBJECT(window), "motion-notify-event", 
							G_CALLBACK(mouseMotionFilter), NULL);
	
	/* gets the _global_ cursor position every 100ms */
	g_timeout_add(100, getXCursor, NULL);
	
	/* get things going */
	screenChanged((GtkWindow*)window, NULL, NULL);	
	gtk_widget_show_all(window);
	gtk_main();

	return EXIT_SUCCESS;
}
