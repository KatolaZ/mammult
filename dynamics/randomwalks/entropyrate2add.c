#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"


int main(int argc, char *argv[]){

	if (argc < 6){
    		printf("Usage: %s <layer1> <layer2> <overlapping network> <N nodes> <b1> <b2>\n", argv[0]);
    		exit(1);
  		}


	FILE *filein0,*filein1, *filein, *fileout;
 	unsigned int N0, K0,N1, K1, N, K;
 	unsigned int *J_slap0, *r_slap0, *J_slap1, *r_slap1, *J_slap, *r_slap;
	double *w_slap;
	
	
	int i, j;
	double c_i, d_i, f_i, f_i_2;
	double alpha = (atof(argv[5])); 
	double beta = (atof(argv[6]));
	

	int ov;
	int deg0, deg1;
	double degM, part, f_j, f_j_2;
	double degMrid, maxdegM = 200.0;
	double num1, num2, den, h;
	int number_nodes=(atoi(argv[4]));
	double M=2.0;
	
	filein0 = openfile_or_exit(argv[1], "r", 2);
  	read_slap(filein0, &K0, &N0, &J_slap0, &r_slap0);

	filein1 = openfile_or_exit(argv[2], "r", 2);
  	read_slap(filein1, &K1, &N1, &J_slap1, &r_slap1);

	
	filein = openfile_or_exit(argv[3], "r", 2);
  	read_slap_w(filein, &K, &N, &J_slap, &r_slap,&w_slap);

	int r_slap0_n[N+1],r_slap1_n[N+1];
	for (i=0; i<=N; i++) {
		if (i<=N0) {
			r_slap0_n[i]=r_slap0[i];
			}
		else {
			r_slap0_n[i]=r_slap0[N0];
			}
		if (i<=N1) {
			r_slap1_n[i]=r_slap1[i];
			}
		else {
			r_slap1_n[i]=r_slap1[N1];
			}
		
		}



	double c_i_vec[N];
	double d_i_vec[N];
	double f_i_vec[N];
	
	for (i=0; i<N; i++) {
		c_i=0;
		d_i=0;
		
		for (j=r_slap[i]; j<r_slap[i+1]; j++) {
			ov = w_slap[j];
			
			deg0=r_slap0_n[J_slap[j]+1]-r_slap0_n[J_slap[j]];
			
			deg1=r_slap1_n[J_slap[j]+1]-r_slap1_n[J_slap[j]];
			
			
			degM=(deg0+deg1)*1.0;
			
			part = (M/(M-1))*  (1-(pow((deg0/degM),2))-(pow((deg1/degM),2))   );
			if (deg0>0.0000000001) {
				f_j = pow (deg0, alpha);
				}
			else {
				f_j = 0;
				} 
			if (deg1>0.0000000001) {
				f_j_2 = pow (deg1, beta);
				}
			else {
				f_j_2=0;
				} 
			c_i+=ov*(f_j+f_j_2);
			
			d_i+=ov*(f_j+f_j_2)*log((ov*(f_j+f_j_2)));
			
			

		
			}
		c_i_vec[i]=c_i;
		d_i_vec[i]=d_i;
		deg0=r_slap0_n[i+1]-r_slap0_n[i];
		deg1=r_slap1_n[i+1]-r_slap1_n[i];
		
		degM=(deg0+deg1)*1.0;
		
		part = (M/(M-1))*  (1-(pow((deg0/degM),2))-(pow((deg1/degM),2)) ); 
		 
		if (deg0>0.0000000001) {
				f_i = pow (deg0, alpha);
				}
			else {
				f_i = 0;
				} 
			if (deg1>0.0000000001) {
				f_i_2 = pow (deg1, beta);
				}
			else {
				f_i_2=0;
				} 
		f_i_vec[i]=f_i+f_i_2;
	
		}
	num1=0;
	num2=0;
	den=0;
	for (i=0; i<N; i++) {
		if (c_i_vec[i]>0.0) {
			num1+=f_i_vec[i]*c_i_vec[i]*log(c_i_vec[i]);
			}
		
		
		num2=num2+f_i_vec[i]*d_i_vec[i];
		den=den+f_i_vec[i]*c_i_vec[i];
		
	
		
	
		}
	
	h=(num1-num2)/den;
	printf("%f %f %f\n", h, alpha, beta);
	
}
