CFLAGS="-O3"
CC="gcc"
GSL_FLAGS=-lgsl -lgslcblas -lm
MFLAG=-lm

all: tune_qnn_adaptive tune_rho 

tune_qnn_adaptive: tune_qnn_adaptive.c rank_utils.c fit_utils.c
	$(CC) $(CFLAGS) -o tune_qnn_adaptive tune_qnn_adaptive.c rank_utils.c fit_utils.c $(GSL_FLAGS)

tune_rho: tune_rho.c rank_utils.c 
	$(CC) $(CFLAGS) -o tune_rho tune_rho.c rank_utils.c $(MFLAG) 

clean:
	rm tune_rho tune_qnn_adaptive
