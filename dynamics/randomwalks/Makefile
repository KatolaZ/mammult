CFLAGS="-O3"
CC="gcc"
MFLAG=-lm

all: statdistr2 entropyrate2add entropyrate2mult entropyrate2int 

statdistr2: statdistr2.c 
	$(CC) $(CFLAGS) -o statdistr2 statdistr2.c utils.c iltree.c $(MFLAG)

entropyrate2add: entropyrate2add.c 
	$(CC) $(CFLAGS) -o entropyrate2add entropyrate2add.c utils.c iltree.c $(MFLAG)

entropyrate2mult: entropyrate2mult.c 
	$(CC) $(CFLAGS) -o entropyrate2mult entropyrate2mult.c utils.c iltree.c $(MFLAG)

entropyrate2int: entropyrate2int.c 
	$(CC) $(CFLAGS) -o entropyrate2int entropyrate2int.c utils.c iltree.c $(MFLAG)

clean:
	rm statdistr2 entropyrate2add entropyrate2mult entropyrate2int 
