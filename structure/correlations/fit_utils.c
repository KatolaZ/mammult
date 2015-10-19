/**
 *
 * Fit a given sequence with a power-law function
 *
 * a*x^{b}
 *
 * The fit is actually performed as a linear fit on the
 * exponential-binned log-log distribution
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gsl/gsl_fit.h>

/**
 * 
 * Load a sequence from a file, which contains one element on each line
 *
 */

void load_sequence(char *fname, double **v, int *N){

  int size;
  char buff[256];
  
  FILE *f;
  
  f = fopen(fname, "r");
  if (!f){
    fprintf(stderr, "Error opening file %s!!!! Exiting...\n", fname);
    exit(1);
  }
  
  *N =0;
  size = 10;
  if (*v == NULL)
    *v = malloc(size * sizeof(double));
  else{
    *v = realloc(*v, size * sizeof(double));
  }
  
  while (fgets(buff, 255, f)){
    (*v)[*N] = atof(buff);
    *N += 1;
    if (*N == size){
      size += 10;
      *v = realloc(*v, size * sizeof(double));
    }
  }
  *v = realloc(*v, (*N) * sizeof(double));
  fclose(f);
}


/**
 *
 * Load a sequence, getting the "col"-th column of the input file
 *
 */

void load_sequence_col(char *fname, double **v, int *N, int col){

  int size, n;
  char buff[256];
  char *ptr;

  FILE *f;
  
  f = fopen(fname, "r");
  if (!f){
    fprintf(stderr, "Error opening file %s!!!! Exiting...\n", fname);
    exit(1);
  }
  
  *N =0;
  size = 10;
  if (*v == NULL)
    *v = malloc(size * sizeof(double));
  else{
    *v = realloc(*v, size * sizeof(double));
  }
  
  while (fgets(buff, 255, f)){
    ptr = strtok(buff, " ");
    if (col > 0){
      n = 0;
      while (n<col){
        ptr = strtok(NULL, " ");
        n += 1;
      }
    }
    (*v)[*N] = atof(ptr);
    *N += 1;

    if (*N == size){
      size += 10;
      *v = realloc(*v, size * sizeof(double));
    }
  }
  *v = realloc(*v, (*N) * sizeof(double));
  fclose(f);
}


/**
 *
 * Make the exponential binning of a distribution, with a giving
 * exponent alpha. The value of y[i] is the number of elements of v
 * whose value lies between x[i-1] and x[i]...
 *
 */

void exp_bin_cnt(double *v, int N, double alpha, double **x, double **y, int *num){
  
  double min_v, max_v, val, last_val;
  int i, j, size, num_x;
  double last_size, new_size;
  
  min_v = max_v = v[0];
  
  for (i=1; i<N; i ++){
    if (v[i] > max_v)
      max_v = v[i];
    else if (v[i] > 0 && v[i] < min_v)
      min_v = v[i];
  }

  size = 10;
  if (*x == NULL){
    *x = malloc(size * sizeof(double));
  }
  else{
    *x = realloc(*x, size * sizeof(double));
  }
  
  val = min_v;
  last_size = min_v;
  (*x)[0] = min_v;
  num_x = 1;

  while(val < max_v){
    new_size = last_size * alpha;
    val = last_size + new_size;
    last_size = new_size;
    last_val = val;
    (*x)[num_x] = val;
    num_x += 1;
    if (num_x == size){
      size += 10;
      *x = realloc(*x, size * sizeof(double));
    }
  }
  
  if (*y == NULL){
    *y = malloc(num_x * sizeof(double));
  }
  else{
    *y = realloc(*y, num_x * sizeof(double));
  }
  for (i=0; i < num_x; i++){
    (*y)[i] = 0;
  }

  

  for(i=0; i <N; i ++){
    j = 0;
    while(v[i] > (*x)[j]){
      j ++;
    }
    (*y)[j] += 1;
  }
  *num = num_x;
}

/**
 *
 * Make the exponential binning of a distribution, with a giving
 * exponent alpha. The value of y[i] is the average of the values in
 * the vector "w" whose corresponding v lies between x[i-1] and  x[i]...
 *
 */


void exp_bin_avg(double *v, double *w, int N, double alpha, double **x, double **y, int *num){
  
  double min_v, max_v, val, last_val;
  int i, j, size, num_x;
  double last_size, new_size;
  int *cnt;


  min_v = max_v = v[0];
  
  for (i=1; i<N; i ++){
    if (v[i] > max_v)
      max_v = v[i];
    else if (v[i] > 0 && v[i] < min_v)
      min_v = v[i];
  }

  size = 10;
  if (*x == NULL){
    *x = malloc(size * sizeof(double));
  }
  else{
    *x = realloc(*x, size * sizeof(double));
  }
  
  val = min_v;
  last_size = min_v;
  (*x)[0] = min_v;
  num_x = 1;

  while(val < max_v){
    new_size = last_size * alpha;
    val = last_size + new_size;
    last_size = new_size;
    last_val = val;
    (*x)[num_x] = val;
    num_x += 1;
    if (num_x == size){
      size += 10;
      *x = realloc(*x, size * sizeof(double));
    }
  }
  
  
  cnt = malloc(num_x * sizeof(int));
  
  if (*y == NULL){
    *y = malloc(num_x * sizeof(double));
  }
  else{
    *y = realloc(*y, num_x * sizeof(double));
  }
  for (i=0; i < num_x; i++){
    (*y)[i] = 0;
    cnt[i] = 0;
  }

  for(i=0; i <N; i ++){
    j = 0;
    while(j < num_x && v[i] > (*x)[j]){
      j ++;
    }
    if(j == num_x){
      printf("Error!!!!! Trying to assing a non-existing bin!!! -- fit_utils.c:exp_bin_avg!!!\n");
      exit(37);
    }
    (*y)[j] += w[i];
    cnt[j] += 1;
  }
  *num = num_x;
  
  for(i=0; i< num_x; i++){
    if (cnt[i] > 0){
      (*y)[i] = (*y)[i] / cnt[i];
    }
  }
  free(cnt);
}


/**
 *
 * Print a distribution on stdout
 *
 */

void dump_distr(double *x, double *y, int N){
  int i;
  
  for(i=0; i<N; i ++){
    printf("%g %g\n", x[i], y[i]);
  }
  
}


/**
 * Compact a distribution, leaving only the pairs (x_i, y_i) for which
 * y_i > 0
 *
 */

void compact_distr(double *x, double *y, int *num){
  
  int i, j;
  
  i = j = 0;
  while(j < *num){
    while(j < *num && y[j] == 0){
      j ++;
    }
    if (j==*num){
      break;
    }
    x[i] = x[j];
    y[i] = y[j];
    i ++;
    j ++;
  }
  *num = i;
}


/**
 *
 * Apply the function "f" on all the elemnts of a vector, in-place
 *
 */

void map_vec(double *v, int N, double (*f)(double)){
  int i;
  
  for (i=0; i<N; i ++){
    v[i] = f(v[i]);
  }
}


/**
 *
 * Normalize a distribution, dividing each y[i] for the width of the
 * corresponding bin (i.e., x[i] - x[i-1])
 *
 */
void normalize_distr(double *x, double *y, int num){
  
  int i;
  
  for(i=1; i<num; i ++){
    y[i] /= (x[i] - x[i-1]);
  }
}

/**
 *
 * take two vectors (k and q) and a pairing, and compute the best
 * power-law fit "a*k^{mu}" of qnn(k)
 *
 */

void fit_current_trend(double *k, double *q, int N, int *pairing, double *mu, double *a, 
                       double *corr){

  static int  *num;
  static double *q_pair, *x, *y;

  int i;
  int fit_num;
  double c0, c1, cov00, cov01, cov11, sqsum;
  
  if (q_pair == NULL){
    q_pair = malloc(N * sizeof(double));
  }

  for(i=0; i<N; i++){
    q_pair[i] = q[pairing[i]];
  }

  exp_bin_avg(k, q_pair, N, 1.3, &x, &y, &fit_num);
  //normalize_distr(x,y, fit_num);
  compact_distr(x, y, &fit_num);
  
  map_vec(x, fit_num, log);
  map_vec(y, fit_num, log);
  gsl_fit_linear(x, 1, y, 1, fit_num, &c0, &c1, &cov00, &cov01, &cov11, &sqsum);

  //fprintf(stderr,"cov00: %g cov01: %g cov11: %g\n", cov00, cov01, cov11);
  //fprintf(stderr, "corr: %g ", cov01 / sqrt(cov00 * cov11));
  *a = c0;
  *mu = c1;
  *corr = cov01/sqrt(cov00 * cov11);
}

