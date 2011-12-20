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

void drawRuler(GtkWidget*);
void drawBackground(cairo_t*);
void drawLines(cairo_t*);
void drawNumbers(cairo_t*);
void drawCursorMark(cairo_t*, int);
void drawPositionNotifier(cairo_t *, cairo_t*, int);
void drawCap(cairo_t*);
void drawRotateButton(cairo_t*);
void drawMenuButton(cairo_t*);
void drawTranslucentOverlay(cairo_t*);
void drawOutline(cairo_t*);
void rendertext(cairo_t*, char*, int, int);
gboolean isInRotateButton(int, int);
gboolean isInMenuButton(int, int);
