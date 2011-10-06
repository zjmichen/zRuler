typedef struct {
	int x;
	int y;
} Point;

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

extern Point cursor;
extern Point cursor_last_pressed;
extern enum Orientation ruler_orientation;
extern GtkWidget *window;
extern gboolean onTop;
