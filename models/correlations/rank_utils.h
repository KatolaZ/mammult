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
#ifndef __RANK_UTILS_H__
#define __RANK_UTILS_H__

/* Load a ranking */
void load_ranking(char *fname, int *N, double **R);

/* Compute the average of an array */
double avg_array(double *v, int N);

/* Compute the term "C" in the rho correlation coefficient */
double compute_C(double *R1, double *R2, int N);

/* Compute the term "D" in the rho correlation coefficient */
double compute_D(double *R1, double *R2, int N);

/* Compute the Spearman's rank correlation coefficient \rho */
double compute_rho(double *R1, double *R2, int N, int *pairing);

void dump_ranking(double *R, int N);

void init_pairing_natural(int *pairing, int N);

void init_pairing_inverse(int *pairing, int N);

void init_pairing_random(int *pairing, int N);

void select_pairing(int *pairing, int N, int argc, char *argv[], int pos);

void shuffle_sequence(int *s, int N);

void dump_pairing(int *pairing, int N);

void copy_pairing(int *pairing1, int *pairing2, int N);


#endif /* __RANK_UTILS_H__ */








