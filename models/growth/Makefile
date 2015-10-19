CFLAGS="-O3"
CC="gcc"
MFLAG=-lm

all: nibilab_linear_delta nibilab_linear_delay nibilab_linear_delay_mix \
	nibilab_linear_random_times nibilab_nonlinear 

nibilab_linear_delta: nibilab_linear_delta.c
	$(CC) $(CFLAGS) -o nibilab_linear_delta  nibilab_linear_delta.c $(MFLAG)

nibilab_linear_delay: nibilab_linear_delay.c
	$(CC) $(CFLAGS) -o nibilab_linear_delay  nibilab_linear_delay.c $(MFLAG)

nibilab_linear_delay_mix: nibilab_linear_delay_mix.c
	$(CC) $(CFLAGS) -o nibilab_linear_delay_mix  nibilab_linear_delay_mix.c $(MFLAG)

nibilab_linear_random_times: nibilab_linear_random_times.c
	$(CC) $(CFLAGS) -o nibilab_linear_random_times  nibilab_linear_random_times.c $(MFLAG)

nibilab_nonlinear: nibilab_nonlinear.c
	$(CC) $(CFLAGS) -o nibilab_nonlinear  nibilab_nonlinear.c $(MFLAG)


clean:
	rm nibilab_linear_delta nibilab_linear_delay nibilab_linear_delay_mix \
		nibilab_linear_random_times nibilab_nonlinear 
