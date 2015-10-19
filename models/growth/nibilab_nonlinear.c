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
 * non-linear preferential attachment.
 *
 * The probability for a newly arrived node $i$ to create a link to an
 * existing node $j$ is 
 * 
 *  p_{i->j} \propto k\lay{1}^{\alpha}/k\lay{2}^{\beta}
 *
 * where alpha and beta are two parameters.
 *
 * Nodes arrive at the same time on both layers.
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
  double *eta;
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


void compute_cumul_nlin(ij_net_t *G1, ij_net_t *G2, double alpha){
  
  int i;
  double val;
  
  /* The bias at layer 1 is k1/k2^2 */
  if (G1->degrees[G1->arrived[0]] > 0  &&  G2->degrees[ G1->arrived[0] ])
    G1->cumul[0] =  (G1->degrees[G1->arrived[0]]) * 1.0 / pow(G2->degrees[ G1->arrived[0] ],alpha) ;
  else
    G1->cumul[0] =  0.0;
  
  if (G2->degrees[G2->arrived[0]] > 0 && G1->degrees[ G2->arrived[0] ] > 0)
    G2->cumul[0] =  ( G2->degrees[G2->arrived[0]]) * 1.0/ pow(G1->degrees[ G2->arrived[0] ],alpha);
  else
    G2->cumul[0] =  0.0;
  
  for(i=1; i<G1->N; i ++){
    if (G1->degrees[G1->arrived[i]] > 0 && G2->degrees[ G1->arrived[i] ] > 0)
      val =  (G1->degrees[G1->arrived[i]]) * 1.0 / pow(G2->degrees[ G1->arrived[i] ],alpha) ;
    else
      val = 0;
    G1->cumul[i] = G1->cumul[i-1] + val;
  }
  for(i=1; i<G2->N; i ++){
    if (G2->degrees[G2->arrived[i]] > 0 && G1->degrees[ G2->arrived[i] ] )
      val =  (G2->degrees[G2->arrived[i]]) * 1.0/ pow(G1->degrees[ G2->arrived[i] ], alpha);
    else
      val = 0.0;
    G2->cumul[i] = G2->cumul[i-1] + val;
  }
}


void compute_cumul_nlin_2(ij_net_t *G1, ij_net_t *G2, double alpha, double beta){
  
  int i;
  double val;
  
  /* The bias at layer 1 is k1/k2^2 */
  if (G1->degrees[G1->arrived[0]] > 0  &&  G2->degrees[ G1->arrived[0] ])
    G1->cumul[0] =  pow(G1->degrees[G1->arrived[0]], alpha) / pow(G2->degrees[ G1->arrived[0] ],beta) ;
  else
    G1->cumul[0] =  0.0;
  
  if (G2->degrees[G2->arrived[0]] > 0 && G1->degrees[ G2->arrived[0] ] > 0)
    G2->cumul[0] =  pow(G2->degrees[G2->arrived[0]], alpha)/pow(G1->degrees[ G2->arrived[0] ], beta);
  else
    G2->cumul[0] =  0.0;
  
  for(i=1; i<G1->N; i ++){
    if (G1->degrees[G1->arrived[i]] > 0 && G2->degrees[ G1->arrived[i] ] > 0)
      val =  pow(G1->degrees[G1->arrived[i]], alpha)/pow(G2->degrees[ G1->arrived[i] ],beta) ;
    else
      val = 0;
    G1->cumul[i] = G1->cumul[i-1] + val;
  }
  for(i=1; i<G2->N; i ++){
    if (G2->degrees[G2->arrived[i]] > 0 && G1->degrees[ G2->arrived[i] ] )
      val =  pow(G2->degrees[G2->arrived[i]],alpha)/pow(G1->degrees[ G2->arrived[i] ], beta);
    else
      val = 0.0;
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




int grow_multi_net_nlin_2(ij_net_t *G1,  ij_net_t *G2, int N, int m, 
                          int m0, double alpha, double beta){
  
  int i, j;
  int d;
  int i2;

  
  for(i=m0; i<N; i++){
    compute_cumul_nlin_2(G1, G2, alpha, beta);
    //dump_cumul(G1);
    //dump_cumul(G2);
    //n = m0 + i; /* This is the id of the new node */
    G1->arrived[i] = i;
    sample_edges(G1, G1->arrived[i], m, 0);
    G1->degrees[G1->arrived[i]] += m;
    
    G2->arrived[i] = i;
    sample_edges(G2, G2->arrived[i], m, 0);
    G2->degrees[G2->arrived[i]] += m;
    G1->N += 1;
    G1->K += m;
    G2->N += 1;
    G2->K += m ;
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
  G->eta = malloc(N * sizeof(double));
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
    printf(" %d", t);
    if (i+t < N){
      add_node_to_time(G, i, t+i);
    }
  }
  printf("\n");
}



void dump_times(ij_net_t *G, int N){

  int i, j;
  
  for (i=0; i<N; i++){
    printf("%d: ", i);
    for (j=0; j< G->times[i].N; j++){
      printf(" %d", G->times[i].val[j]);
    }
    printf("\n");
  }
  
}


void init_eta_random(ij_net_t *G, int N){

  int i;
  double val;
  
  for (i=0; i<N; i ++){
    val = rand() * 1.0/RAND_MAX;
    G->eta[i] = val;
  }
}

void init_eta_ass(ij_net_t *G2, ij_net_t *G1, int N){
  int i;
  
  for (i=0; i<N; i ++){
    G2->eta[i] = G1->eta[i];
  }
  
}

void init_eta_dis(ij_net_t *G2, ij_net_t *G1, int N){
  int i;
  
  for (i=0; i<N; i ++){
    G2->eta[i] = 1 - G1->eta[i];
  }
  
}


int main(int argc, char *argv[]){
  
  ij_net_t G1, G2;
  
  int N, m, m0, k_max;
  coupling_t matr;
  double alpha, beta;
  distr_t delay_distr;

  char str[256];
  
  if (argc < 6){
    printf("Usage: %s <N> <m> <m0> <outfile> <alpha> <beta>\n", argv[0]);
    exit(1);
  }

  srand(time(NULL));
  
  /* Diagonal coupling */
  
  N = atoi(argv[1]);
  m = atoi(argv[2]);
  m0 = atoi(argv[3]);
  alpha=atof(argv[5]);
  beta = atof(argv[6]);

  G1.size = (N+m0) * m;
  G2.size = (N+m0) * m;

  init_structure(&G1, N);
  init_structure(&G2, N);
  
  
  G1.K = init_network(&G1, m0);
  G2.K = init_network(&G2, m0);

  //init_eta_random(&G1, N);
  //init_eta_random(&G2, N);
  
  

  //init_times_delta(&G1, N);
  //init_times_delta(&G2, N);
  
  //dump_times(&G2, N);
  
  fprintf(stderr, "Init finished!\n");
  
  grow_multi_net_nlin_2(&G1, &G2, N, m, m0, alpha, beta);
  
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
