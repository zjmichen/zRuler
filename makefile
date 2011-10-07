NAME=zruler
GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`
CFLAGS= -c -Wall $(GTKFLAGS)
OBJS=main.o window.o graphics.o mouse.o
CC=gcc
INSTALL_PATH=/usr/local/bin/
DESKTOP_PATH=/usr/share/applications/
ICON_PATH=/usr/share/pixmaps/
INSTALL_FILES="  $(INSTALL_PATH)$(NAME)\n  $(DESKTOP_PATH)$(NAME)\n  $(ICON_PATH)$(NAME)"

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
	
install: ruler
	mkdir -p $(INSTALL_PATH)
	cp $(NAME) $(INSTALL_PATH)
	cp $(NAME).desktop $(DESKTOP_PATH)
	chmod 0755 $(DESKTOP_PATH)$(NAME).desktop
	cp $(NAME).png $(ICON_PATH)
	@echo
	@echo Installed the following files:
	@echo $(INSTALL_FILES)
	@echo
	
