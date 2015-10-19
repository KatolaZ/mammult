#ifndef __UTILS_H__
#define __UTILS_H__

#include "iltree.h"

iltree_t init_tree(iltree_t t, void *fileout);

int read_deg_distr(FILE *filein, unsigned int **degs, unsigned int **Nk, double **p);

int read_deg_seq(FILE *filein, unsigned int **nodes);

int read_stubs(FILE *filein, unsigned int **S);

int read_ij(FILE *filein, unsigned int **i, unsigned int **j);

/*funzione pesata di moreno*/
int read_ij_w(FILE *filein, unsigned int **i, unsigned int **j, double **w);

void read_slap(FILE *filein, unsigned int *K, unsigned int *N, 
               unsigned int **J_slap, unsigned int **r_slap);

/*funzione pesata di moreno*/
void read_slap_w(FILE *filein, unsigned int *K, unsigned int *N, 
               unsigned int **J_slap, unsigned int **r_slap, double **w_slap);

int convert_ij2slap(unsigned int *I, unsigned int *J, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap);

/*funzione pesata di moreno*/
int convert_ij2slap_w(unsigned int *I, unsigned int *J, double *W, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap,double **w_slap);

int convert_ij2slap_N(unsigned int *I, unsigned int *J, unsigned int K,   
                      unsigned int N, unsigned int ** r_slap, 
                      unsigned int **J_slap);
  

void write_edges(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N);
  

void dump_deg_distr(unsigned int *degs, double *p, int n);

void dump_deg_seq(unsigned int *nodes, int N);

void dump_edges(iltree_t t);

FILE* openfile_or_exit(char *filename, char *mode, int exitcode);

int compare_int(const void *x1, const void *x2);

unsigned int find_max(unsigned int *, unsigned int);

int is_neigh(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
             unsigned int i, unsigned int j);


#endif /*__UTILS_H__*/
