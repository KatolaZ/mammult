/**
 *
 * Tune the Spearman's \rho correlation coefficient between two rankings
 * given as input, using a simulated-anneling procedure
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <strings.h>
#include <time.h>

#include "rank_utils.h"

//void select_pairing(int *pairing, int N, int argc, char *argv[]){
//
//  if (argc < 7 || !strncasecmp("rnd", argv[6], 3)){
//    init_pairing_random(pairing, N);
//  }
//  else if (!strncasecmp("nat", argv[6], 3)){
//    init_pairing_natural(pairing, N);
//  }
//  else if (!strncasecmp("inv", argv[6], 3)){
//    init_pairing_inverse(pairing, N);
//  }
//  else{
//    printf ("Pairing strategy \"%s\" unknown!!! Exiting...\n", argv[6]);
//    exit(1);
//  }
//
//}

void tune_rho(double *R1, double *R2, int N, int *pairing, double eps, 
              double beta, double rho_target){
  
  double act_rho, test_rho, F, F_new;
  double val, prob;
  int *new_pairing;
  int p1, p2, tmp_val;
  int tot;
 
  new_pairing = malloc(N * sizeof(int));
  copy_pairing(pairing, new_pairing, N);
  
  act_rho = compute_rho(R1, R2, N, pairing);
  
  F = fabs(act_rho - rho_target);
  
  //fprintf("%f %f %f %f %f\n", eps, beta, rho_target, act_rho, F);
  
  tot = 0;
  while (F > eps){
    /* sample two positions of "pairing" and shuffle them in "new_pairing" */
    p1 = rand() % N;
    p2 = rand() % N;
    while (p2 == p1){
      p2 = rand() % N;
    }
    tmp_val = new_pairing[p1];
    new_pairing[p1] = new_pairing[p2];
    new_pairing[p2] = tmp_val;
    test_rho = compute_rho(R1, R2, N, new_pairing);
    F_new = fabs(test_rho - rho_target);
    if (F_new < F){/* Accept this swap with probability 1 */
      tmp_val = pairing[p1];
      pairing[p1] = pairing[p2];
      pairing[p2] = tmp_val;
      act_rho = test_rho;
    }
    else{/* Accept the swap with a certain probability */
      val = 1.0 * rand() / RAND_MAX;

      //prob = pow(fabs(F - F_new)/(F+F_new), beta);
      prob = exp(-(F_new - F)/beta);
      //fprintf(stderr, "-- %f\n", prob);
      if (val < prob){ /* Accept the swap */
        tmp_val = pairing[p1];
        pairing[p1] = pairing[p2];
        pairing[p2] = tmp_val;
        act_rho = test_rho;
      }
      else{ /* Rollback the swap */
        tmp_val = new_pairing[p1];
        new_pairing[p1] = new_pairing[p2];
        new_pairing[p2] = tmp_val;
      }
      
    }
    F = fabs(act_rho - rho_target);
    if (tot % 200 == 0){
      fprintf(stderr, "%d %g\n", tot, act_rho);
    }
    tot += 1;
  }
}




int main (int argc, char *argv[]){

  int N1, N2;
  double *R1 = NULL;
  double *R2 = NULL;
  double eps, beta, rho, rho_target;
  
  int *pairing = NULL;

  srand(time(NULL));
  
  if (argc < 6){
    printf("Usage: %s <rank1> <rank2> <rho> <eps> <beta> [RND|NAT|INV]\n", argv[0]);
    exit(1);
  }
  
  rho_target = atof(argv[3]);
  eps = atof(argv[4]);
  beta = atof(argv[5]);
  
  load_ranking(argv[1], &N1, &R1);
  load_ranking(argv[2], &N2, &R2);
  
  if (N1 != N2){
    printf("Error!!!! The two files must have the same number of lines!!!! Exiting...\n");
    exit(1);
  }
  
  pairing = malloc(N1 * sizeof(int));

  select_pairing(pairing, N1, argc, argv, 6);
  
  rho = compute_rho(R1, R2, N1, pairing);
  
  fprintf(stderr, "%g\n", rho);
  
  tune_rho(R1, R2, N1, pairing, eps, beta, rho_target);

  dump_pairing(pairing, N1);
  
}
