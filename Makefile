vpath %.o .

CC = gcc
CFLAGS = -Wall
LIB_OBJ = 
LIB_H = 

.PHONY: clean cleanall all
all: gim clean


$(LIB_OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

gim: gim.o $(LIB_OBJ) $(LIB_H)
	$(CC) -o gim $(LIB_OBJ) gim.o

gimd: daemon.o $(LIB_OBJ) $(LIB_H)
	$(CC) -o gimd $(LIB_OBJ) daemon.o

cleanall: clean
	-rm -f gim gimd

clean:
	-rm -f *.o

