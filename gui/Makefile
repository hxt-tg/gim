CC = gcc
CFLAGS = -Wall
GUILIB = -lcurses -lpanel
LIB_OBJ = gui_main.o gui_misc.o gui_contact.o
LIB_H   = gui_main.h gui_misc.h gui_contact.h

.PHONY: clean cleanall all
all: demo keys_test clean

$(LIB_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(GUILIB)

demo: main.o $(LIB_OBJ) $(LIB_H)
	$(CC) $(CFLAGS) -o demo $(LIB_OBJ) main.o $(GUILIB)

keys_test: keys_test.c
	$(CC) $(CFLAGS) -o keys_test keys_test.c $(GUILIB)

cleanall: clean
	-rm -f demo key_test

clean:
	-rm -f *.o

