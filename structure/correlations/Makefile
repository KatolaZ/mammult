CFLAGS="-O3"
CC="gcc"
GSL_FLAGS=-lgsl -lgslcblas -lm
MFLAG=-lm

all: dump_k_q fit_knn

fit_knn: fit_knn.c fit_utils.c
	$(CC) $(CFLAGS) -o fit_knn fit_knn.c fit_utils.c $(GSL_FLAGS)

dump_k_q: dump_k_q.c rank_utils.c
	$(CC) $(CFLAGS) -o dump_k_q dump_k_q.c rank_utils.c $(MFLAG) 

clean:
	rm dump_k_q fit_knn
