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








