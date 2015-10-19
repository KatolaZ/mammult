#include "iltree.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void* alloc_double(){
  return malloc(sizeof(long double));
}

void dealloc_double(void *elem){
  free(elem);
}

void copy_double(void *elem1, void *elem2){
  *((long double*)elem2) = *((long double*)elem1);
}

int compare_double(void *elem1, void *elem2){
  return *((long double*)elem1) - *((long double*)elem2);
}

void print_double(void *elem, void *fileout){
  
  long double k, i, j;
  long double x;

  k = *((long double*)elem);

  x = (1 + sqrtl(1 + 8 * (k-1))) / 2;
  i = floorl(x) + 1;
  j = k - ( (i-1)*1.0 * (i-2) ) /2;
  //printf("x: %Lf\n i: %0.0Lf j: %0.0Lf\n", x, i, j);
  fprintf((FILE*)fileout, "%0.0Lf %0.0Lf\n", i-1, j-1);
}

iltree_t init_tree(iltree_t t, void *fileout){
  
  ilfunc_t funs= {
    .alloc = alloc_double,
    .dealloc = dealloc_double, 
    .copy = copy_double,
    .compare = compare_double, 
    .print = print_double,
    .fileout = fileout
  };
  
  t = iltree_create(t);
  iltree_set_funs(t, &funs);
  return t;
}


/* @@@@ CAMBIARE IL PRIMO PARAMETRO IN UN FILE* PER RENDERLA COERENTE
   ALLE ALTRE FUNZIONI DI READ!!!!*/
int read_deg_distr(FILE *filein, unsigned int **degs, unsigned int **Nk, double **p){
  
  int n_degs = 0;
  int size = 10;
  char *line;
  char buff[256];
  int k_i, num_i;
  double p_i;
  char *ptr;

  line = NULL;

  *degs = realloc(*degs, size*sizeof(unsigned int));
  *Nk = realloc(*Nk, size*sizeof(unsigned int));
  *p = realloc(*p, size*sizeof(double));
  

  while(fgets(buff, 256, filein)){
    ptr = strtok(buff, " ");
    if (ptr[0] == '#')
      continue;
    k_i = atoi(ptr);
    ptr = strtok(NULL, " " );
    num_i = atoi(ptr);
    ptr = strtok(NULL, " \n");
    p_i = atof(ptr);
    if (n_degs == size){
      size += 10;
      *degs = realloc(*degs, size*sizeof(unsigned int));
      *Nk = realloc(*Nk, size*sizeof(unsigned int));
      *p = realloc(*p, size*sizeof(double));
    }
    (*degs)[n_degs] = k_i;
    (*Nk)[n_degs] = num_i;
    (*p)[n_degs] = p_i;
    n_degs += 1;
  }
  *degs = realloc(*degs, n_degs*sizeof(unsigned int));
  *Nk = realloc(*Nk, n_degs*sizeof(unsigned int));
  *p = realloc(*p, n_degs*sizeof(double));
  return n_degs;
}


int read_deg_seq(FILE *filein, unsigned int **nodes){

  int size, N, k;
  char buff[256];
  char *ptr;

  N = 0;
  size = 10;
  
  *nodes = (unsigned int*)malloc(size * sizeof(unsigned int));

  while(fgets(buff, 256, filein)){
    ptr = strtok(buff, " ");
    if (ptr[0] == '#')
      continue;
    k = atoi(ptr);
    
    if (N == size){
      size += 10;
      *nodes = realloc(*nodes, size*sizeof(unsigned int));
    }
    (*nodes)[N] = k;
    N += 1;
  }
  *nodes = realloc(*nodes, N * sizeof(unsigned int));
  return N;
}

int read_stubs(FILE *filein, unsigned int **S){
  
  int size, K;
  char buff[256];
  char *ptr;
  
  K=0;
  size = 20;
  *S = malloc(size * sizeof(unsigned int));
  
  while(fgets(buff, 256, filein)){
    if (K == size){
      size += 20;
      *S = realloc(*S, size*sizeof(unsigned int));
    }
    ptr = strtok(buff, " "); /* read the first node */
    (*S)[K++] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    (*S)[K++] = atoi(ptr);
  }
  *S = realloc(*S, K * sizeof(unsigned int));
  return K;
}

/*
 * Read a file in ij format 
 */
int read_ij(FILE *filein, unsigned int **I, unsigned int **J){
  
  unsigned int size, K;
  char buff[256];
  char *ptr;
  
  size = 20;
  K = 0;
  
  *I = malloc(size * sizeof(unsigned int));
  *J = malloc(size * sizeof(unsigned int));
  while(fgets(buff, 256, filein)){
    if (buff[0] == '#')
      continue;
    if (K == size){
      size += 20;
      *I = realloc(*I, size*sizeof(unsigned int));
      *J = realloc(*J, size*sizeof(unsigned int));
    }
    ptr = strtok(buff, " "); /* read the first node */
    (*I)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    (*J)[K] = atoi(ptr);
    K += 1;
  }
  
  *I = realloc(*I, K * sizeof(unsigned int));
  *J = realloc(*J, K * sizeof(unsigned int));
  return K;
}

/*funzione pesata di moreno*/

int read_ij_w(FILE *filein, unsigned int **I, unsigned int **J , double **W){
  
  unsigned int size, K;
  char buff[256];
  char *ptr;
  
  size = 20;
  K = 0;
  
  *I = malloc(size * sizeof(unsigned int));
  *J = malloc(size * sizeof(unsigned int));
  *W = malloc(size * sizeof(double));
  
  while(fgets(buff, 256, filein)){
    if (buff[0] == '#')
      continue;
    if (K == size){
      size += 20;
      *I = realloc(*I, size*sizeof(unsigned int));
      *J = realloc(*J, size*sizeof(unsigned int));
      *W = realloc(*W, size*sizeof(double));
      if (*W==NULL) {
	printf ("Errore");
	exit(-1);
	}
    }
    ptr = strtok(buff, " "); /* read the first node */
    (*I)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    (*J)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the weight  */
    (*W)[K] = atof(ptr);    
    K += 1;
  }
  
  *I = realloc(*I, K * sizeof(unsigned int));
  *J = realloc(*J, K * sizeof(unsigned int));
  *W = realloc(*W, K * sizeof(double));
  return K;
}



void read_slap(FILE *filein, unsigned int *K, unsigned int *N, 
              unsigned int **J_slap, unsigned int **r_slap){
  
  unsigned int *I=NULL, *J=NULL;
  unsigned int i, k;
  
  k = read_ij(filein, &I, &J);
  *K = 2 * k;
  I = realloc(I, 2*k * sizeof(unsigned int));
  J = realloc(J, 2*k * sizeof(unsigned int));
  for (i=k; i<2*k; i ++){
    I[i] = J[i-k];
    J[i] = I[i-k];
  }
  
  *N = convert_ij2slap(I, J, 2*k, r_slap, J_slap);
  free(I);
  free(J);
  return;
}

/*funzione pesata di moreno*/

void read_slap_w(FILE *filein, unsigned int *K, unsigned int *N, 
              unsigned int **J_slap, unsigned int **r_slap, double **w_slap){
  
  unsigned int *I=NULL, *J=NULL;
  double *W=NULL;
  unsigned int i, k;
  
  k = read_ij_w(filein, &I, &J, &W);
  *K = 2 * k;
  I = realloc(I, 2*k * sizeof(unsigned int));
  J = realloc(J, 2*k * sizeof(unsigned int));
  W = realloc(W, 2*k * sizeof(double));
  
  for (i=k; i<2*k; i ++){
    I[i] = J[i-k];
    J[i] = I[i-k];
    W[i] = W[i-k];
  }
  
  *N = convert_ij2slap_w(I, J, W, 2*k, r_slap, J_slap, w_slap);
  free(I);
  free(J);
  free(W);
  return;
}

unsigned int find_max(unsigned int *v, unsigned int N){

  unsigned int i, max;
  
  max = v[0];
  i = 0;
  while(++i < N){
    if (v[i] > max)
      max = v[i];
  }
  return max;
}


int convert_ij2slap(unsigned int *I, unsigned int *J, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap){
  
  unsigned int tmp, max;
  unsigned int N;
  unsigned int i, pos;
  unsigned int *p;

  max = find_max(I, K) + 1;
  tmp = find_max(J, K) + 1;
  if (tmp > max){
    max = tmp ;
  }
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  //fprintf(stderr, "WARNING!!!! R_SLAP[0] NOW IS SET TO ZERO!!!!!\n");
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}

/*funzione pesata di moreno*/
int convert_ij2slap_w(unsigned int *I, unsigned int *J, double *W, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap,double **w_slap){
  
  unsigned int tmp, max;
  unsigned int N;
  unsigned int i, pos;
  unsigned int *p;

  max = find_max(I, K) + 1;
  tmp = find_max(J, K) + 1;
  if (tmp > max){
    max = tmp ;
  }
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  *w_slap = malloc(K * sizeof(double));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  //fprintf(stderr, "WARNING!!!! R_SLAP[0] NOW IS SET TO ZERO!!!!!\n");
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    
    (*w_slap)[pos] = W[i];
    
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}



int convert_ij2slap_N(unsigned int *I, unsigned int *J, unsigned int K,   
                      unsigned int N, unsigned int ** r_slap, 
                      unsigned int **J_slap){
  
  unsigned int tmp, max;
  unsigned int i, pos;
  unsigned int *p;

  max = N;
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  //fprintf(stderr, "WARNING!!!! R_SLAP[0] NOW IS SET TO ZERO!!!!!\n");
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}



/* RIVEDERE QUESTA  FUNZIONE...... PASSARE UN FILE COME ARGOMENTO E
   USARE fprintf */
void dump_deg_distr(unsigned int *degs, double *p, int n){

  int i;

  for(i=0; i<n; i++){
    printf("%d %2.6f\n", degs[i], p[i]);
  }
}



/* RIVEDERE QUESTA  FUNZIONE...... PASSARE UN FILE COME ARGOMENTO E
   USARE fprintf */
void dump_deg_seq(unsigned int *nodes, int N){

  int i;
  for(i=0; i<N; i++){
    printf("%d: %d\n", i, nodes[i]);
  }
}

void dump_edges(iltree_t t){

  iltree_view_pre(t);
}

FILE* openfile_or_exit(char *filename, char *mode, int exitcode){
  
  FILE *fileout;
  char error_str[256];
  
  fileout = fopen(filename, mode);
  if (!fileout){
    sprintf(error_str, "Error opening file %s", filename);
    perror(error_str);
    exit(exitcode);
  }
  return fileout;
}

inline int compare_int(const void *x1, const void *x2){
  return *((unsigned int*)x1) - *((unsigned int*)x2);
}

void write_edges(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N){
  
  unsigned int i, j;
  
  for(i=0; i<N; i++){
    for (j=r_slap[i]; j<r_slap[i+1]; j++){
      if (J_slap[j] > i){
        fprintf(fileout, "%d %d\n", i, J_slap[j]);
      }
    }
  }
}
  

/* Check if j is a neighbour of i */
int is_neigh(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
             unsigned int i, unsigned int j){
  
  unsigned int l;
  unsigned int count;
  count = 0;
  for(l=r_slap[i]; l<r_slap[i+1]; l++){
    if (J_slap[l] == j)
      count ++;
  }
  return count;
}
