/**
 *
 * Get the degree distributions of two layers and a pairing, and dump
 * on output the pairs k-q 
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

void dump_k_q(double *R1, double *R2, int N, int *pairing){

  int i;

  for (i=0; i<N; i++){
    printf("%g %g\n", R1[i], R2[pairing[i]]);
  }
}


int main(int argc, char *argv[]){

  int N1, N2;
  double *R1 = NULL;
  double *R2 = NULL;
  
  int *pairing = NULL;


  if (argc < 4){
    printf("Usage: %s <degs1> <degs2> <pairing>\n", argv[0]);
    exit(1);
  }

  load_ranking(argv[1], &N1, &R1);
  load_ranking(argv[2], &N2, &R2);
  
  if (N1 != N2){
    printf("Error!!!! The two files must have the same number of lines!!!! Exiting...\n");
    exit(1);
  }
  
  pairing = malloc(N1 * sizeof(int));
  
  load_pairing(&pairing, N1, argv[3]);
  
  dump_k_q(R1, R2, N1, pairing);

}
