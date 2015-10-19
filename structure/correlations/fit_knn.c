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
 * Take a file which contains pairs in the form:
 *
 * k q
 *
 * compute qnn(k) and fit this function with a power-law
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

#include <gsl/gsl_fit.h>
#include "fit_utils.h"



int main(int argc, char *argv[]){

  double *k, *q, *x, *y;
  int N, num;
  double alpha;

  double c0, c1, cov00, cov01, cov11, sqsum;

  k=q=x=y=NULL;

  if (argc < 3){
    printf("%s <filein> <alpha>\n", argv[0]);
    exit(1);
  }
  
  alpha=atof(argv[2]);
  
  fprintf(stderr,"Alpha: %g\n", alpha);
  
  load_sequence_col(argv[1], &k, &N, 0);
  load_sequence_col(argv[1], &q, &N, 1);
  //exit(1);
  exp_bin_avg(k, q, N, alpha, &x, &y, &num);
  //normalize_distr(x, y, num);
  //dump_distr(x, y, num);
  compact_distr(x, y, &num);
  //  dump_distr(x,y,num);
  map_vec(x, num, log);
  map_vec(y, num, log);
  //dump_distr(x,y,num);

  gsl_fit_linear(x, 1, y, 1, num, &c0, &c1, &cov00, &cov01, &cov11, &sqsum);
  fprintf(stderr, "a: %g b: %g\n", exp(c0), c1);
}
