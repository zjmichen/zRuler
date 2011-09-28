typedef struct {
	int x;
	int y;
} Point;

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

extern Point cursor;
extern enum Orientation ruler_orientation;
