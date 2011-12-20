/****************************************************************************\
* Copyright 2011 Zack Michener <zack@zackmichener.com>                       *
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

#include <gtk/gtk.h>
#include "common.h"
#include "mouse.h"
#include "graphics.h"
#include "window.h"

gboolean mouseButtonFilter(GtkWidget *widget, GdkEvent *event, gpointer userdata) {
	cursorLastPressed.x = event->button.x;
	cursorLastPressed.y = event->button.y;
	
	/* set up x,y relative to rotation so I can use them in funcs easier */
	int x, y;
	if (rulerOrientation == HORIZONTAL) {
		x = cursorLastPressed.x;
		y = cursorLastPressed.y;
	}
	else {
		x = cursorLastPressed.y;
		y = cursorLastPressed.x;	
	}
	
	/* right click = context menu */
	if (event->button.button == 3) {
		viewPopupMenu(widget, (GdkEventButton*)event, userdata);
		return TRUE;
	}
	
	/* rotate button is clicked */
	if (isInRotateButton(x, y))
		rotateRuler(widget);
		
	if (isInMenuButton(x,y))
		viewPopupMenu(widget, (GdkEventButton*)event, userdata);
	
	return TRUE;
}

gboolean mouseMotionFilter(GtkWidget *widget, GdkEventMotion *event, gpointer userdata) {    
	if (resizingRuler(widget)) {
		if (rulerOrientation == HORIZONTAL) {
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

gboolean resizingRuler(GtkWidget *widget) {
	int w_width, w_height;	// window dimensions
    gtk_window_get_size(GTK_WINDOW(widget), &w_width, &w_height);
    
	if (cursorLastPressed.x >= w_width - 10 && rulerOrientation == HORIZONTAL)
		return TRUE;
	if (cursorLastPressed.y >= w_height - 10 && rulerOrientation == VERTICAL)
		return TRUE;

	return FALSE;
}
