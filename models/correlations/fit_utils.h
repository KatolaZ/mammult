#ifndef __FIT_UTILS_H__
#define __FIT_UTILS_H__


void load_sequence(char *fname, double **v, int *N);

void load_sequence_col(char *fname, double **v, int *N, int col);

void exp_bin_cnt(double *v, int N, double alpha, double **x, double **y, int *num);

void exp_bin_avg(double *v, double *w, int N, double alpha, double **x, double **y, int *num);

void dump_distr(double *x, double *y, int N);

void compact_distr(double *x, double *y, int *num);

void map_vec(double *v, int N, double (*f)(double));

void normalize_distr(double *x, double *y, int num);

void fit_current_trend(double *k, double *q, int N, int *pairing, 
                       double *mu, double *a, double *corr);


#endif /* __FIT_UTILS_H__ */
