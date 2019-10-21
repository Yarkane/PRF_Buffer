CC=gcc
CFLAGS=-Wall -g 

SRC=			\
	buffer.c	\
	packet.c	\
	fes.c	\
	fifo.c	\
	rng.c	

OBJ=$(SRC:.c=.o)
BIN=buffer

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ -lm

%.o:%.c
	$(CC) -c $(CFLAGS) $< 

clean:
	rm -f $(OBJ) $(BIN)

buffer.o: buffer.c functions.h
rng.o: rng.c 
packet.o: packet.c packet.h 
fes.o: fes.c fes.h 
fifo.o: fifo.c fifo.h 

