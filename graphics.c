/* graphics.c
 * does all the drawing, also things
 * which require access to graphic
 * data (e.g. is_in_button)
 */

#include <math.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include "common.h"
#include "graphics.h"

int i, j;
int r_width, r_length;

void drawRuler(GtkWidget *widget) {
	/* get window dimensions */
	int w_width, w_height, w_x, w_y;
    gtk_window_get_size(GTK_WINDOW(widget), &w_width, &w_height);
    gtk_window_get_position(GTK_WINDOW(widget), &w_x, &w_y);
    
    int cursorMarkPosition;
    
    /* make for rotation-independence */
    if (rulerOrientation == HORIZONTAL) {
		r_width = w_height;
		r_length = w_width;
		cursorMarkPosition = cursor.x - w_x;
    }
    else {
    	r_width = w_width;
    	r_length = w_height;
    	cursorMarkPosition = cursor.y - w_y;
    }
    
    cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(widget));
    cairo_t *cr_overlay = gdk_cairo_create(gtk_widget_get_window(widget));
    /* rotate the ruler, if necessary */
    if (rulerOrientation == VERTICAL) {
    	cairo_translate(cr, r_width, 0);
    	cairo_rotate(cr, M_PI/2);
    	cairo_translate(cr_overlay, r_width, 0);
    	cairo_rotate(cr_overlay, M_PI/2);
   	}
    
    cairo_set_line_width(cr, 1);
    
    /* draw the components of the ruler */
    drawBackground(cr);
    drawLines(cr);
    drawNumbers(cr_overlay);
    drawCursorMark(cr, cursorMarkPosition);
    drawPositionNotifier(cr, cr_overlay, cursorMarkPosition);
    drawCap(cr);
    drawRotateButton(cr);
    drawMenuButton(cr);
    drawTranslucentOverlay(cr_overlay);
    drawOutline(cr);
    
    cairo_destroy(cr);
	cairo_destroy(cr_overlay);
}

void drawBackground(cairo_t *cr) {
	/* make transparent */
	cairo_set_source_rgba (cr, 1.0, 0.5, 1.0, 0.0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);
    
    /* add a translucent shade */
    cairo_rectangle(cr, 0, 0, r_length, r_width);
    cairo_set_source_rgba(cr, 0,0,0, .25);
    cairo_fill(cr);
    
}

/* pixel markers, the things that make it a ruler */
void drawLines(cairo_t *cr) {
	for (i = 0; i < r_length; i++) {
		int seglen = 3;
		if (i % 10 == 0)
			seglen = 6;
		if (i % 50 == 0)
			seglen = 9;
		if (i % 2 == 0)
			cairo_set_source_rgb(cr, 0,0,0);	// default line color
		else
			cairo_set_source_rgba(cr, 1,1,1, .4);	// in-between lines
			
		cairo_move_to(cr, i + 0.5, 1);
		cairo_line_to(cr, i + 0.5, seglen);
        cairo_move_to(cr, i + .5, r_width - 1);
        cairo_line_to(cr, i + .5, r_width - seglen);
		cairo_stroke(cr);
	}
}

/* so you don't have to count */
void drawNumbers(cairo_t *cr) {
	cairo_set_source_rgb(cr, 1,1,1);
	
    for (i = 0; i < r_length; i++) {
        if ( i % 50 == 0 ) {
            char buf[5];
            sprintf(buf, "%d", i);
            rendertext(cr, buf, i + 3, 6);
            rendertext(cr, buf, i + 3, r_width - 18);
        }
    }
}

/* tracks the mouse cursor position */
void drawCursorMark(cairo_t *cr, int pos) {
	if (pos > 0 && pos < r_length) {
		cairo_move_to(cr, pos+0.5, 0);
		cairo_line_to(cr, pos+0.5, r_width);
		
		cairo_set_source_rgba(cr, 1,1,1, 0.75);
		cairo_stroke(cr);
	}
}

/* text displaying cursor position */
void drawPositionNotifier(cairo_t *cr_bg, cairo_t *cr_num, int pos) {
	if (rulerOrientation==VERTICAL) {
		cairo_rotate(cr_num, -M_PI/2);
		cairo_translate(cr_num, -r_width, 0);
		cairo_rotate(cr_bg, -M_PI/2);
		cairo_translate(cr_bg, -r_width, 0);
	}

	if (rulerOrientation==HORIZONTAL) {
		cairo_move_to(cr_bg, 13.5, r_width/2);
		cairo_curve_to(cr_bg, 13.5, r_width/2 - 12, 44.5, r_width/2 - 12, 44.5, r_width/2);
		cairo_move_to(cr_bg, 13.5, r_width/2);
		cairo_curve_to(cr_bg, 13.5, r_width/2 + 12, 44.5, r_width/2 + 12, 44.5, r_width/2);
	}
	else {
		cairo_move_to(cr_bg, r_width/2 + 3, 24.5);
		cairo_curve_to(cr_bg, r_width/2 - 17, 24.5, r_width/2 - 17, 39.5, r_width/2 + 3, 39.5);
		cairo_move_to(cr_bg, r_width/2 + 3, 24.5);
		cairo_curve_to(cr_bg, r_width/2 + 23, 24.5, r_width/2 + 23, 39.5, r_width/2 + 3, 39.5);
	}
	cairo_set_source_rgba(cr_bg, 1,1,1, 0.8);
	cairo_fill(cr_bg);
		
	cairo_set_source_rgb(cr_num, 0,0,0);
	int numLen = -4*(int)log10(pos)+1;
	if (pos < 0)
		numLen -= 8;
	int numOffset = 26;
	char buf[5];
	sprintf(buf, "%d", pos);
/*	rendertext(cr_num, buf, 26 + numLen, r_width/2 - 6);*/

	
	if (rulerOrientation==VERTICAL) {
		rendertext(cr_num, buf, r_width/2 + numLen, numOffset);
		cairo_translate(cr_num, r_width, 0);
		cairo_rotate(cr_num, M_PI/2);
		cairo_translate(cr_bg, r_width, 0);
		cairo_rotate(cr_bg, M_PI/2);
	}
	else {
		rendertext(cr_num, buf, numOffset + numLen, r_width/2 - 6);
	}
}

/* thing on the end you resize it with */
void drawCap(cairo_t *cr) {
	cairo_rectangle(cr, r_length - 10, 0, 10, r_width);
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_fill(cr);
    cairo_set_source_rgba(cr, 0,0,0,0);
    cairo_move_to(cr, r_length - 3.5, r_width/2 - 6);
    cairo_line_to(cr, r_length - 3.5, r_width/2 + 6);
    cairo_move_to(cr, r_length - 5.5, r_width/2 - 6);
    cairo_line_to(cr, r_length - 5.5, r_width/2 + 6);
    cairo_stroke(cr);
}

/* small button to rotate it 90 degrees */
void drawRotateButton(cairo_t *cr) {
	cairo_arc(cr, r_length - 20.5, r_width/2 + 0.5, 5, 0.5, 2*M_PI);
	cairo_move_to(cr, r_length - 15.5, r_width/2 + 0.5);
	cairo_line_to(cr, r_length - 19.5, r_width/2 - 0.5);
	cairo_move_to(cr, r_length - 15.5, r_width/2 + 0.5);
	cairo_line_to(cr, r_length - 14.5, r_width/2 - 3.5);
	
    cairo_set_source_rgba(cr, 1,1,1, 1);
    cairo_stroke(cr);

}

/* small button to open the context menu */
void drawMenuButton(cairo_t *cr) {
/*	cairo_arc(cr, r_length - 20.5, r_width/2 + 0.5, 5, 0.5, 2*M_PI);*/
/*	cairo_move_to(cr, r_length - 15.5, r_width/2 + 0.5);*/
/*	cairo_line_to(cr, r_length - 19.5, r_width/2 - 0.5);*/
/*	cairo_move_to(cr, r_length - 15.5, r_width/2 + 0.5);*/
/*	cairo_line_to(cr, r_length - 14.5, r_width/2 - 3.5);*/

	cairo_rectangle(cr, r_length - 45.5, r_width/2 - 5.5, 11, 11);
	
    cairo_set_source_rgba(cr, 1,1,1, 1);
    cairo_stroke(cr);

}

/* makes it shiny and 3Dish */
void drawTranslucentOverlay(cairo_t *cr) {
    cairo_pattern_t *hor_pat = cairo_pattern_create_linear(0, 0, 0, r_width);
    cairo_pattern_add_color_stop_rgba(hor_pat, 0, 1,1,1, .7);
    cairo_pattern_add_color_stop_rgba(hor_pat, .2, 0,0,0, 0);
    cairo_rectangle(cr, 0, 0, r_length, r_width);
    cairo_set_source(cr, hor_pat);
    cairo_fill(cr);
}

/* 1px border */
void drawOutline(cairo_t *cr) {
	cairo_set_source_rgb(cr, 0,0,0);
	cairo_rectangle(cr, 0.5, 0.5, r_length - 1, r_width - 1);
	cairo_stroke(cr);
}

void rendertext(cairo_t *cr, char *s, int x, int y) {
    PangoLayout *layout;
    PangoFontDescription *desc;    
    
    layout = pango_cairo_create_layout(cr);
    
    pango_layout_set_text(layout, s, -1);
    desc = pango_font_description_from_string("Sans 7.5");
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);
    
    pango_cairo_update_layout(cr, layout);
    
    cairo_translate(cr, x, y);
    pango_cairo_show_layout(cr, layout);
    cairo_translate(cr, -x, -y);
    
    g_object_unref(layout);
    
}

/* checks if x,y is in the rotate button
 * note: this is relative to the rotation!
 */
gboolean isInRotateButton(int x, int y) {
	if (x > r_length - 25 && x < r_length - 15 && y > r_width/2 - 4.5 && y < r_width/2 + 4.5)
		return TRUE;
		
	return FALSE;
}
