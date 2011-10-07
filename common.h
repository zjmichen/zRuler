typedef struct {
	int x;
	int y;
} Point;

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

extern const char *VERSION;
extern Point cursor;
extern Point cursorLastPressed;
extern enum Orientation rulerOrientation;
extern GtkWidget *window;
extern gboolean onTop;
