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
 * Tune the inter-layer degree correlation function \bar{q}(k) of a
 * two-layer multiplex, in order to make it as similar as possible to
 * a power law with exponent $\mu$, where $\mu$ is given as input
 *
 * This version of the program is (or, better, "shoud be") able to
 * tune also the value of the pre-factor "a"
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <strings.h>
#include <time.h>

#include "rank_utils.h"




inline double compute_delta(double q, double k, double mu, double a){
  
  return fabs(log(q) - mu * log(k) - log(a));
  //return fabs (q - a*pow(k,mu));
}






void tune_qnn_adaptive(double *R1, double *R2, int N, int *pairing, double eps, 
              double beta, double mu_target){
  
  double act_mu, test_mu, F, F_new, F_old;
  double delta1_old, delta2_old, delta1_new, delta2_new;
  double val, prob;
  double mu, a, err, dummy_a;
  int *new_pairing;
  int p1, p2, tmp_val;
  int tot;
  char swap = 0;
 
  new_pairing = malloc(N * sizeof(int));
  copy_pairing(pairing, new_pairing, N);

  a = 1.0;
  
  F = 10000;
      
  fit_current_trend(R1, R2, N, pairing, &mu, &a, &err);
  fprintf(stderr, "Initial mu: %g a: %g corr: %g\n", mu, a, err);
   
  //fprintf("%f %f %f %f %f\n", eps, beta, mu_target, act_mu, F);
  
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
    
    delta1_old = compute_delta(R2[pairing[p1]], R1[p1], mu_target, a);
    delta2_old = compute_delta(R2[pairing[p2]], R1[p2], mu_target, a);
    
    delta1_new = compute_delta(R2[new_pairing[p1]], R1[p1], mu_target, a);
    delta2_new = compute_delta(R2[new_pairing[p2]], R1[p2], mu_target, a);
    
    //F_new = log(delta1_new * delta2_new);
    //F_old = log(delta1_old * delta2_old);
    
    F_new = delta1_new + delta2_new;
    F_old = delta1_old + delta2_old; 
    
    
    if (F_new <= F_old){/* Accept this swap with probability 1 */
      tmp_val = pairing[p1];
      pairing[p1] = pairing[p2];
      pairing[p2] = tmp_val;
      //act_mu = test_mu;
      swap = 1;
    }
    else{/* Accept the swap with a certain probability */
      val = 1.0 * rand() / RAND_MAX;

      //prob = pow(fabs(F - F_new)/(F+F_new), beta);
      prob = exp(-(F_new - F_old)/beta);
      //fprintf(stderr, "-- %g %g %g -> %f \n", F_new, F_old, F_new - F_old, prob);
      if (val < prob){ /* Accept the swap */
        tmp_val = pairing[p1];
        pairing[p1] = pairing[p2];
        pairing[p2] = tmp_val;
        //act_mu = test_mu;
        swap = 1;
      }
      else{ /* Rollback the swap */
        tmp_val = new_pairing[p1];
        new_pairing[p1] = new_pairing[p2];
        new_pairing[p2] = tmp_val;
        swap = 0;
      }
      
    }
    

    ///F = fabs(act_mu - mu_target);
    ///if (tot % 200 == 0){
    //fprintf(stderr, "%d %g\n", tot, act_mu);

    //fprintf(stderr, "%d: %f %f ---- %f %f ---- %f %f ---- %d \n",
    //tot, delta1_old, delta2_old, delta1_new, delta2_new, F_old, F_new, swap);
    
    ///}
    tot += 1;
    if (tot %200 == 0){
      fit_current_trend(R1, R2, N, pairing, &mu, &a, &err);
      fprintf(stderr, "mu: %g a: %g corr: %g\n", mu, a, err);
      //a = a - 0.01 *(a - exp(a));
      a = exp(a);
    }
    fit_current_trend(R1, R2, N, pairing, &mu, &dummy_a, &err);
    F = fabs(mu - mu_target);
  }
  fit_current_trend(R1, R2, N, pairing, &mu, &a, &err);
  fprintf(stderr, "Final mu: %g a: %g corr: %g\n", mu, a, err);
 }

void dump_qnn(double *R1, double *R2, int N, int *pairing){
  
  int i;
  int *qnn, *num;

  qnn = malloc(N * sizeof(int));
  num = malloc(N * sizeof(int));

  for (i=0; i<N; i++){
    qnn[i]=0;
    num[i]=0;
  }
  
  for (i=0; i<N; i++){
    qnn[(int)(R1[i])] += R2[pairing[i]];
    num[(int)(R1[i])] += 1;
  }
  
  for(i=0; i<N; i++){
    if (num[i] >0){
      printf("%d %f\n", i, 1.0*qnn[i]/num[i]);
    }
  }
  free(num);
  free(qnn);
}


void dump_degs(double *R1, double *R2, int N, int *pairing){

  int i;

  for(i=0; i<N; i++){
    printf("%g %g\n", R1[i], R2[pairing[i]]);
  }

}

int main (int argc, char *argv[]){

  int N1, N2;
  double *R1 = NULL;
  double *R2 = NULL;
  double eps, beta, mu_target;
  
  int *pairing = NULL;

  srand(time(NULL));
  
  if (argc < 6){
    printf("Usage: %s <degs1> <degs2> <mu> <eps> <beta> [RND|NAT|INV]\n", argv[0]);
    exit(1);
  }
  
  mu_target = atof(argv[3]);
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
  
  tune_qnn_adaptive(R1, R2, N1, pairing, eps, beta, mu_target);

  dump_pairing(pairing, N1);
  //dump_qnn(R1, R2, N1, pairing);
  //dump_degs(R1, R2, N1, pairing);
  
}
