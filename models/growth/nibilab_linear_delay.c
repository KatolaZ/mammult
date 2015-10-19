/*
 * This file is part of MAMMULT: Metrics And Models for Multilayer Networks
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 *
 * NiBiLaB model (Nicosia, Bianconi, Latora, Barthelemy) of multiplex
 * linear preferential attachment
 *
 * A new node arrives on layer 1 at each time step, but its replica on
 * layer 2 arrives after a power-law distributed delay \tau
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


typedef struct{
  int size;
  int N;
  int *val;
} int_array_t;


typedef struct{
  int K;
  int N;
  int size;
  int *i;
  int *j;
  int *degrees;
  int *arrived;
  double *cumul;
  int_array_t *times;
} ij_net_t;



typedef struct{
  double a;
  double b;
  double c;
  double d;
} coupling_t;


typedef struct{
  int N;
  double x_min;
  double *distr;
  double gamma;
} distr_t;


int init_network(ij_net_t *G, int m0){
  
  int n;

  for(n=0; n<m0; n++){
    G->i[n] = n;
    G->j[n] = n+1 % m0;
    G->degrees[n] = 2;
    G->arrived[n] = n;
  }
  G->K = m0;
  G->N = m0;
  return n;
}



void dump_network(ij_net_t *G){

  int i;
  for(i=0; i< G->K; i ++){
    printf("%d %d\n",G->i[i], G->j[i]);
  }
}


void dump_network_to_file(ij_net_t *G, char *fname){


  FILE *f;
  int i;

  
  f = fopen(fname, "w+");
  
  for(i=0; i< G->K; i ++){
    fprintf(f, "%d %d\n",G->i[i], G->j[i]);
  }
  fclose(f);
}



double f1(double v1, double v2, coupling_t *matr){
  return v1 * matr->a + v2 * matr->b;
}

double f2(double v1, double v2, coupling_t *matr){
  return v1 * matr->c + v2 * matr->d;
}


void compute_cumul(ij_net_t *G1, ij_net_t *G2, coupling_t *matr){
  
  int i;
  double val;
  
  G1->cumul[0] =  f1(G1->degrees[ G1->arrived[0] ], G2->degrees[ G1->arrived[0] ], matr)  ;
  G2->cumul[0] =  f2(G1->degrees[ G2->arrived[0] ], G2->degrees[ G2->arrived[0] ], matr)  ;
  
  for(i=1; i<G1->N; i ++){
    val = f1(G1->degrees[ G1->arrived[i] ], G2->degrees[ G1->arrived[i] ],matr)  ;
    G1->cumul[i] = G1->cumul[i-1] + val;
  }
  for(i=1; i<G2->N; i ++){
    val = f2(G1->degrees[ G2->arrived[i] ], G2->degrees[ G2->arrived[i] ], matr)  ;
    G2->cumul[i] = G2->cumul[i-1] + val;
  }
}

void dump_cumul(ij_net_t *G){
  
  int i;

  for (i=0; i<G->N; i ++){
    printf("%d %2.6f\n", G->times[i], G->cumul[i]);
  }

}



int select_neigh_cumul(ij_net_t *G){

  double r;
  int j;
  
  r = (rand() * 1.0 / RAND_MAX) * G->cumul[ G->N-1 ];
  //printf(" r: %f ", r);
  
  j = 0;
  /* Change with a binary search ???!?!?!?! */
  while (r > G->cumul[j]){
    j ++;
  }
  return G->arrived[j];
}

int already_neighbour(ij_net_t *G, int j, int d, int offset){

   int i;
   
   for(i=G-> K + offset; i< G->K + offset + j; i ++){
     if (G->j[i] == d)
       return 1;
   }
   return 0;
}


void sample_edges(ij_net_t *G, int n, int m, int offset){

  int j;
  int d;
  
  //printf("----");
  for(j=0; j<m; j++){
    G->i[G->K + offset + j] = n;
    d = select_neigh_cumul(G);
    while(already_neighbour(G, j, d, offset)){
      d = select_neigh_cumul(G);
    }
    //printf(" link %d--%d\n", n, d);
    G->j[G->K + offset + j] = d;
    G->degrees[d] += 1;
  }
  //printf("\n");
}

void create_distr(double *v, double gamma, int x_min, int x_max){
  int n, i;
  double sum, new_sum;
  
  n = x_max-x_min + 1;
  
  sum = 0;
  for(i=0; i<n; i++){
    v[i] = pow((x_min + i), gamma);
    sum += v[i];
  }
  new_sum = 0;
  /* Now we normalize the array*/
  for(i=0; i<n; i++){
    v[i]/= sum;
    new_sum += v[i];
  }
  //printf("New_sum: %lf\n", new_sum);
  /* Now we compute the cumulative distribution*/
  for(i=1; i<n; i++){
    v[i] += v[i-1];
  }
}


inline int find_degree(double *v, int dim, double xi){
  int i;
  
  i=0;
  while(xi > v[i])
    i++;
  return i;
  
}


int sample_power_law(distr_t *d){
  
  double xi, q, val;
  int k;
  
  while(1){
    xi = rand()* 1.0 / RAND_MAX;
    k = find_degree(d->distr, d->N, xi);
    val = rand()*1.0/RAND_MAX;
    q = d->x_min + xi * d->N;
    q = q / (floor(q) + 1);
    q = pow(q, d->gamma);
    if (val <= q){
      return k;
    }
  }
}




int grow_multi_net_delay(ij_net_t *G1,  ij_net_t *G2, int N, int m, int m0, coupling_t *matr){
  
  int i, j;
  int d;
  int i2;

  i2 = m0;
  
  for(i=m0; i<N; i++){
    compute_cumul(G1, G2, matr);
    //dump_cumul(G1);
    //dump_cumul(G2);
    //n = m0 + i; /* This is the id of the new node */
    G1->arrived[i] = i;
    sample_edges(G1, G1->arrived[i], m, 0);
    G1->degrees[G1->arrived[i]] += m;
    
    for (j=0; j < G2->times[i].N; j++){
      G2->arrived[i2] = G2->times[i].val[j];
      //printf("%d\n", G2->arrived[i2]);
      sample_edges(G2, G2->arrived[i2], m, m *j);
      G2->degrees[G2->arrived[i2]] += m;
      i2 += 1;
    }
    G1->N += 1;
    G1->K += m;
    G2->N += G2->times[i].N;
    G2->K += m * G2->times[i].N;
  }
  return 0;
}

void init_structure(ij_net_t *G, int N){

  int i;

  G->i = malloc(G->size * sizeof(int));
  G->j = malloc(G->size * sizeof(int));
  G->degrees = malloc(N * sizeof(int));
  G->arrived = malloc(N * sizeof(int));
  G->cumul = malloc(N * sizeof(double));
  G->times = malloc(N * sizeof(int_array_t));
  for (i=0; i<N; i++){
    G->times[i].size = 5;
    G->times[i].N = 0;
    G->times[i].val = malloc(5 * sizeof(int));
  }
  memset(G->degrees, 0, N*sizeof(int));
  G->N = 0;
  
}

void add_node_to_time(ij_net_t *G, int node, int t){
  
  if (G->times[t].size == G->times[t].N){
    G->times[t].size += 5;
    G->times[t].val = realloc(G->times[t].val, G->times[t].size);
  }
  G->times[t].val[G->times[t].N] = node;
  G->times[t].N += 1;
}


void init_times_delta(ij_net_t *G, int N){
  
  int i;

  for (i=0; i<N; i ++){
    add_node_to_time(G,i,i);
  }
  
}



void init_times_delay(ij_net_t *G, distr_t *d, int m0, int N){
  int i;
  int t;
  
  for (i=m0; i<N; i ++){
    t = sample_power_law(d);/* So t is in the interval [1,N] */
    //printf(" %d", t);
    if (i+t < N){
      add_node_to_time(G, i, t+i);
    }
  }
  //printf("\n");
}



void dump_times(ij_net_t *G, int N){

  int i, j;
  
  for (i=0; i<N; i++){
    for (j=0; j< G->times[i].N; j++){
      printf("%d %d\n", i,G->times[i].val[j]);
    }
    //printf("\n");
  }
  
}


int main(int argc, char *argv[]){
  
  ij_net_t G1, G2;
  
  int N, m, m0, k_max;
  coupling_t matr;
  double gamma;
  distr_t delay_distr;

  char str[256];
  
  if (argc < 10){
    printf("Usage: %s <N> <m> <m0> <outfile> <a> <b> <c> <d> <gamma>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL));
  
  /* Diagonal coupling */
  matr.a = atof(argv[5]);
  matr.b = atof(argv[6]);
  matr.c = atof(argv[7]);
  matr.d = atof(argv[8]);
  gamma = atof(argv[9]);

  N = atoi(argv[1]);
  m = atoi(argv[2]);
  m0 = atoi(argv[3]);

  G1.size = (N+m0) * m;
  G2.size = (N+m0) * m;

  init_structure(&G1, N);
  init_structure(&G2, N);
  
  
  G1.K = init_network(&G1, m0);
  G2.K = init_network(&G2, m0);

  delay_distr.N = N;
  delay_distr.gamma = gamma;
  delay_distr.x_min = 1;
  delay_distr.distr = malloc(N * sizeof(double));

  create_distr(delay_distr.distr, delay_distr.gamma, delay_distr.x_min, N);

  init_times_delay(&G2, &delay_distr, m0, N);

  dump_times(&G2, N);
  
  fprintf(stderr, "Init finished!\n");
  
  grow_multi_net_delay(&G1, &G2, N, m, m0, &matr);
  
  //printf("### G1\n");
  sprintf(str, "%s_layer1.txt", argv[4]);
  dump_network_to_file(&G1, str);
  
  //printf("### G2\n");
  sprintf(str, "%s_layer2.txt", argv[4]);
  dump_network_to_file(&G2, str);
  
  /* dump_network_to_file(S, S_num, argv[4]); */
  /* printf("Network dumped!\n"); */
  /* k_max = degree_distr(S, S_num, &distr); */
  /* printf("k_max is: %d\n", k_max); */
  /* dump_distr_to_file(distr, k_max, argv[5]); */
  
}
