/**
 *
 * Compute the cartography of a multiplex network (participation
 * coefficient vs average degree) starting from the layers
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "utils.h"


int main(int argc, char *argv[]){
  
  
  int i, val, M;
  unsigned int N, K, *J_slap=NULL, *r_slap=NULL;
  unsigned int maxN;

  double part;

  unsigned int *k=NULL, *k2=NULL, *tmp;
  
  FILE *fin;

  maxN = 0;

  M = 0;
  while(argc-- > 1){
    fin = openfile_or_exit(argv[argc], "r", 17);
    M += 1;
    if (J_slap)
      free(J_slap);
    if (r_slap)
      free(r_slap);
    J_slap = r_slap = NULL; /* This is somehow inefficient, since we
                               are going to reallocate that memory in
                               a moment....*/
    read_slap(fin, &K, &N, &J_slap, &r_slap);
    if (N > maxN){
      tmp = realloc(k, N * sizeof(unsigned int));
      VALID_PTR_OR_EXIT(tmp, 7);
      k = tmp;
      tmp = realloc(k2, N * sizeof(unsigned int));
      VALID_PTR_OR_EXIT(tmp, 7);
      k2 = tmp;
      
      memset(k+maxN, 0, (N-maxN) * sizeof(unsigned int));
      memset(k2+maxN, 0, (N-maxN) * sizeof(unsigned int));
      maxN = N;
    }
    for(i=0; i<N; i++){
      val = r_slap[i+1] - r_slap[i];
      k[i] += val;
      k2[i] += val * val;
    }
  }
  
  for(i=0; i<maxN; i++){
    part = 1.0 * M / (M-1) * (1 - 1.0 * k2[i] / (k[i] * k[i]));
    printf("%d %f %f %d %d\n", i, 1.0 * k[i]/M, part, k[i], k2[i]);
  }
}

