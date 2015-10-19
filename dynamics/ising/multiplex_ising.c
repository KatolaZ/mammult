#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "utils.h"
#include "iltree.h"



typedef struct{
  double T;
  double J;
  double Jcoup;
  double p0;
  double p1;
  double h0;
  double h1;
  unsigned int num_epochs;
} param_t;


typedef struct{
  unsigned int N;
  unsigned int K;
  unsigned int *J_slap;
  unsigned int *r_slap;
  int *s;
} net_t;

typedef struct{
  double m0;
  double m1;
  double C;
  double b0;
  double b1;
  double q0;
  double q1;
  double IFC0;
  double IFC1;
  double M;
} stats_t;

void init_spins(int *s, int N, double p){
  
  int i;
  double val;

  for (i=0; i<N; i ++){
    val = 1.0 * rand() / RAND_MAX;
    if (val < p){
      s[i] = 1;
    }
    else
      s[i] = -1;
  }

}


void init_spins_once(param_t *sys, net_t *layers) {
  int N, N2;
  N = layers[0].N;
  N2 = 2 * N;

  init_spins(layers[0].s, N, sys->p0);
  init_spins(layers[1].s, N, sys->p1);
	}

void shuffle_ids(int * v, int N){
  
  int tmp, val, i;

  for (i=N-1; i >=0; i --){
    val = rand() % (i+1);
    tmp = v[i];
    v[i] = v[val];
    v[val] = tmp;
  }
}

void compute_stats (net_t *layers, stats_t *stats, int *spinold0, int *spinold1){
  
  int i, N;
  
  N = layers[0].N;
  
  stats->M =stats->m0 = stats->m1 = stats->C = stats->b0 = stats->b1 = stats->q0 = stats->q1 = stats->IFC0 = stats->IFC1 = 0;
  
  double *bubblevec0, *bubblevec1;
  bubblevec0 = (double *)malloc(N * sizeof(double));
  bubblevec1 = (double *)malloc(N * sizeof(double));
	
  
  double bubble0, bubble1, deg;
  int j;
  for (i=0; i<N; i++){
    stats->m0 += layers[0].s[i];
    stats->m1 += layers[1].s[i];
    stats->C += layers[0].s[i] * layers[1].s[i];
    

    bubble0=0;
    for(j=layers[0].r_slap[i]; 
          j< layers[0].r_slap[i + 1]; 
          j ++){
          bubble0+= layers[0].s[i] * layers[0].s[ layers[0].J_slap[j] ] ;
	  stats->IFC0 += fabs(layers[0].s[i] * layers[0].s[ layers[0].J_slap[j] ]-1.)/2.;
          }
    deg = (layers[0].r_slap[i + 1] - layers[0].r_slap[i])*1.0;
    bubblevec0[i]=bubble0/deg;
    bubble1=0;
    for(j=layers[1].r_slap[i]; 
          j< layers[1].r_slap[i + 1]; 
          j ++){
          bubble1+= layers[1].s[i] * layers[1].s[ layers[1].J_slap[j] ] ;
	  stats->IFC1 += fabs(layers[1].s[i] * layers[1].s[ layers[1].J_slap[j] ]-1.)/2.;
          }
    deg = (layers[1].r_slap[i + 1] - layers[1].r_slap[i])*1.0;
    bubblevec1[i]=bubble1/deg;

    stats->q0 += layers[0].s[i]*spinold0[i];
    stats->q1 += layers[1].s[i]*spinold1[i];
  }

  stats->b0=0;
  for (i=0; i<N; i++) {
	stats->b0 = stats->b0 + bubblevec0[i];
	}
  stats->b0 /= N;
  stats->b1=0;
  for (i=0; i<N; i++) {
	stats->b1 = stats->b1 + bubblevec1[i];
	}
  stats->b1 /= N;

  stats->m0 /= N;
  stats->m1 /= N;
  stats->C /= N;
  stats->q0 /= N;
  stats->q1 /= N;
  stats->IFC0 /= layers[0].K;
  stats->IFC1 /= layers[1].K;
  
  stats->M = (fabs(stats->m0)+fabs(stats->m1))/2.0;
 }

void dump_spins(int *s1, int *s2, int N){
  
  int i;

  for(i=0; i<N; i++){
    printf("%d %d\n", s1[i], s2[i]);
  }
  
}


void make_simulation(param_t *sys, net_t *layers, stats_t *stats){
  
 
  int  *ids;
  int e, i, j, num_flips, id, l;
  unsigned int N, N2;

  double E_old, E_new, val, exp_val;

  N = layers[0].N;
  N2 = 2 * N;
    
  ids = malloc(N2 * sizeof(int));
  for (i=0; i< N2; i++){
    ids[i] = i;
  }

  int *spinold0, *spinold1;
  spinold0 = (int *)malloc(N * sizeof(int));
  spinold1 = (int *)malloc(N * sizeof(int));


  
  for (e=0; e < sys->num_epochs; e++){
    num_flips = 0;
    shuffle_ids(ids, N2);
    for (i=0; i< N2; i++){
      id = ids[i] % N;
      l = ids[i]/N;
      //printf("i: %d id: %d l:%d\n", ids[i], id, l);
      E_old = 0;
      E_new = 0;
      for(j=layers[l].r_slap[id]; 
          j< layers[l].r_slap[id + 1]; 
          j ++){
        E_old -= layers[l].s[id] * layers[l].s[ layers[l].J_slap[j] ];
        E_new -= -layers[l].s[id] * layers[l].s[ layers[l].J_slap[j] ];
      }
      E_old *= sys->J;
      E_new *= sys->J;

      if (l==0) {
	E_old -= sys->h0 * layers[l].s[id];
	E_new -= sys->h0 * (-layers[l].s[id]);
	}
      else {
	E_old -= sys->h1 * layers[l].s[id];
	E_new -= sys->h1 * (-layers[l].s[id]);
	}

      E_old -= sys->Jcoup * layers[l].s[id] * layers[1-l].s[id];
      E_new -= sys->Jcoup * -(layers[l].s[id]) * layers[1-l].s[id];

      E_old = 2*E_old;
      E_new = 2*E_new;
      if (E_new <= E_old){ /* The new configuration has smaller energy -> flip! */
        layers[l].s[id] = - layers[l].s[id];
      }
      else if (sys->T > 0){ /* The new conf has higher energy -> flop with e-(\Delta E)/T */
        val = 1.0 * rand() / RAND_MAX;
        exp_val = exp( - (1.0*(E_new - E_old)) / sys->T);
        if (val < exp_val){
          layers[l].s[id] = - layers[l].s[id];
        }
      }
    }

  if (e==(sys->num_epochs-2)) {
	int u;
	for (u=0; u<N; u++) {
		spinold0[u]=layers[0].s[u];
		spinold1[u]=layers[1].s[u];
		}
	}



  }
  compute_stats(layers, stats, spinold0, spinold1);
  
  //free(spinold0);
  //free(spinold1);
  //dump_spins(layers[0].s, layers[1].s, N);
}

void dump_stats(param_t *sys, stats_t *s){
  
  printf("## T J gamma h0 h1 p1 p2 m1 m2 C\n");
  printf("%g %g %g %g %g %g %g %g %g %g\n", 
         sys->T, sys->J, sys->Jcoup, sys->h0, sys->h1, sys->p0, 
         sys->p1, s->m0, s->m1, s->C);
  fflush(stdout);
}


int main(int argc, char *argv[]){
  

  net_t layers[2];
  param_t sys;
  stats_t stats;
  unsigned int *J_slap, *r_slap;
  
  FILE *fin;

  if (argc < 10){
    printf("Usage: %s <layer1> <layer2> <T> <J> <gamma> <h1> <h2> <p1> <p2> <num_epochs>\n", argv[0]);
    exit(1);
  }

  sys.T = atof(argv[3]);
  sys.J = atof(argv[4]);
  sys.Jcoup = atof(argv[5]);
  sys.p0 = atof(argv[8]);
  sys.p1 = atof(argv[9]);
  sys.h0 = atof(argv[6]);
  sys.h1 = atof(argv[7]);
  sys.num_epochs = atoi(argv[10]);
  
  srand(time(NULL));

  J_slap = r_slap = NULL;
  
  fin = openfile_or_exit(argv[1], "r", 2);
  read_slap(fin, &(layers[0].K), &(layers[0].N), &J_slap, &r_slap);
  layers[0].J_slap = J_slap;
  layers[0].r_slap = r_slap;
  fclose(fin);

  J_slap = r_slap = NULL;

  fin = openfile_or_exit(argv[2], "r", 2);
  read_slap(fin, &(layers[1].K), &(layers[1].N), &J_slap, &r_slap);
  layers[1].J_slap = J_slap;
  layers[1].r_slap = r_slap;
  fclose(fin);

  if (layers[0].N != layers[1].N){
    printf("Error!!! Both layers must have the same number of nodes!!!!\n");
    exit(3);
  }

   /* allocate space for the spins on each layer */
  layers[0].s = malloc(layers[0].N * sizeof(double));
  layers[1].s = malloc(layers[1].N * sizeof(double));
  
  /* inizialize the spins */
  init_spins_once(&sys, layers);


  make_simulation(&sys, layers, &stats);
  
 
  
}
