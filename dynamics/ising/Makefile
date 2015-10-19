CFLAGS="-O3"
CC="gcc"
MFLAG=-lm

all: multiplex_ising 

multiplex_ising: multiplex_ising.c 
	$(CC) $(CFLAGS) -o multiplex_ising multiplex_ising.c utils.c iltree.c $(MFLAG)

clean:
	rm multiplex_ising 
