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
 * Some utility functions to be used with tune_rho, compute_rho and
 * the like
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "rank_utils.h"

void load_ranking(char *fname, int *N, double **R){
  
  char buff[256];
  int size = 10;
  FILE *f;
  
  if (*R == NULL){
    *R = malloc(size * sizeof (double));
  }
  f = fopen(fname, "r");
  if (!f){
    printf("Unable to open file: %s!!! Exiting\n");
    exit(1);
  }
  
  *N = 0;
 
  while (fgets(buff, 255, f)){
    if (* N == size){
      size += 10;
      *R = realloc(*R, size * sizeof(double));
    }
    (*R)[*N] = atof(buff);
    *N += 1;
  }
}

double avg_array(double *v, int N){
  
  double sum = 0.0;
  int i;
  
  for (i = 0; i < N; i ++){
    sum += v[i];
  }
  return sum/N;
}

double compute_C(double *R1, double *R2, int N){
  double mu1, mu2, sum1, sum2;

  mu1 = avg_array(R1, N);
  mu2 = avg_array(R2, N);
  
  sum1 = mu1 * N;
  sum2 = mu2 * N;
  
  return N * mu1 * mu2 - mu2 * sum1 - mu1 * sum2;
}


double compute_D(double *R1, double *R2, int N){
  
  double mu1, mu2, s1, s2;
  int i;

  mu1 = avg_array(R1, N);
  mu2 = avg_array(R1, N);
  
  s1 = s2 = 0.0;
  
  for (i=0 ; i < N; i ++){
    s1 += pow((R1[i] - mu1), 2);
    s2 += pow((R2[i] - mu2), 2);
  }
  
  return sqrt(s1 * s2);
}


double compute_rho(double *R1, double *R2, int N, int *pairing){
  
  double rho = 0;
  int i;
  
  for (i=0; i < N; i ++){
    rho += R1[i] * R2[ pairing[i] ];
  }

  rho = (rho + compute_C(R1, R2, N))/ compute_D(R1, R2, N);
  return rho;
}

void dump_ranking(double *R, int N){
  int i;

  for (i=0; i < N; i ++){
    printf("%d: %f\n", i, R[i] );
  }
}


void init_pairing_natural(int *pairing, int N){
  int i;

  for (i = 0; i< N; i ++){
    pairing[i] = i;
  }
}

void init_pairing_inverse(int *pairing, int N){
  int i;

  for (i = 0; i< N; i ++){
    pairing[i] = N-i-1;
  }
}

void select_pairing(int *pairing, int N, int argc, char *argv[], int pos){

  if (argc < pos + 1 || !strncasecmp("rnd", argv[pos], 3)){
    init_pairing_random(pairing, N);
  }
  else if (!strncasecmp("nat", argv[pos], 3)){
    init_pairing_natural(pairing, N);
  }
  else if (!strncasecmp("inv", argv[pos], 3)){
    init_pairing_inverse(pairing, N);
  }
  else{
    printf ("Pairing strategy \"%s\" unknown!!! Exiting...\n", argv[pos]);
    exit(1);
  }

}


void shuffle_sequence(int *s, int N){
  
  int i, j, tmp;

  for (i=N-1; i>=0; i--){
    j = rand() % (i+1);
    tmp = s[j];
    s[j] = s[i];
    s[i] = tmp;
  }
}



void init_pairing_random(int *pairing, int N){

  init_pairing_natural(pairing, N);
  shuffle_sequence(pairing, N);

}

/* Loads a pairing from a file, in the format:
 * 
 * rank1 rank2
 * ...........
 */
void load_pairing(int **pairing, int N, char *fname){
  
  FILE *f;
  int i, j, num;
  char buff[256];
  char *ptr;

  f = fopen(fname, "r");
  if (!f){
    printf("Error opening file \"%s\"!!!! Exiting....\n", fname);
    exit(2);
  }

  if (*pairing == NULL){
    *pairing = malloc(N * sizeof(int));
    init_pairing_natural(*pairing, N);
  }
  
  num = 0;
  while(num < N){
    fgets(buff, 255, f);
    if (buff[0] == '#')
      continue;
    ptr = strtok(buff, " "); /* read the first node */
    i = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    j = atoi(ptr);
    (*pairing)[i] = j;
    num += 1;
  }
}


void dump_pairing(int *pairing, int N){
  int i;

  for(i=0; i< N; i ++){
    printf("%d %d\n", i, pairing[i]);
  }
}

void copy_pairing(int *p1, int *p2, int N){
  int i;
  
  for (i=0 ; i < N; i ++){
    p2[i] = p1[i];
  }
}
