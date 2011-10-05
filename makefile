NAME=zruler
GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`
CFLAGS= -c -Wall $(GTKFLAGS)
OBJS=main.o window.o graphics.o mouse.o
CC=gcc

all: ruler

ruler: $(OBJS)
	$(CC) $(GTKFLAGS) $(OBJS) -o $(NAME)

main.o: main.c
	$(CC) $(CFLAGS) main.c

window.o: window.c
	$(CC) $(CFLAGS) window.c

graphics.o: graphics.c
	$(CC) $(CFLAGS) graphics.c

mouse.o: mouse.c
	$(CC) $(CFLAGS) mouse.c

clean:
	rm -f $(NAME)
	rm -f *.o

run: ruler
	./$(NAME)
