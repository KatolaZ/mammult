#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"


int main(int argc, char *argv[]){

	
	if (argc < 6){
    		printf("Usage: %s <layer1> <layer2> <overlapping network> <N nodes> <b1> <b2>\n", argv[0]);
    		exit(1);
  		}


	/*dichiarazioni enzo*/
	FILE *filein0,*filein1, *filein, *fileout;
 	unsigned int N0, K0,N1, K1, N, K;
 	unsigned int *J_slap0, *r_slap0, *J_slap1, *r_slap1, *J_slap, *r_slap;
	double *w_slap;
	
	/*dichiarazioni mie*/
	/*r_slap e' lungo N+1, da 0 a N compresi*/
	int i, j;
	double f_j, f_j_2;
	double alpha = (atof(argv[5])); 
	double beta = (atof(argv[6]));
	
	/*il bias*/
	int ov;
	int deg0, deg1;
	double degM, prodM, part, intM;
	double degMrid, maxdegM = 200.0;

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



	double cf_i_vec_add[N];
	double cf_i_vec_mult[N];
	double cf_i_vec_part[N];
	double cf_i_vec_int[N];
	
	double tot_add=0, tot_mult=0, tot_part=0, tot_int=0;
	double c_i_add, c_i_mult, c_i_part, c_i_int;
	double f_add, f_mult, f_int;
	/*ciclo sui nodi dell'aggregato*/
	for (i=0; i<N; i++) {
		c_i_add=0;
		c_i_mult=0;
		c_i_part=0;
		c_i_int=0;
		
		/*ciclo sui primi vicini di i*/
		for (j=r_slap[i]; j<r_slap[i+1]; j++) {
			ov = w_slap[j];
			
			deg0=r_slap0_n[J_slap[j]+1]-r_slap0_n[J_slap[j]];
			
			deg1=r_slap1_n[J_slap[j]+1]-r_slap1_n[J_slap[j]];
			
			
			degM=(deg0+deg1)*1.0;
			//prodM=(deg0*deg1)*1.0;
			part = (M/(M-1))*  (1-(pow((deg0/degM),2))-(pow((deg1/degM),2))   );
			//intM=degM*part;
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

			c_i_add+=ov*(f_j+f_j_2);

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

			c_i_mult+=ov*(f_j*f_j_2);
			//c_i_part+=ov*part;

			part = (M/(M-1))*  (1-(pow((deg0/degM),2))-(pow((deg1/degM),2))   );
			if (part>0.0000000001) {
				f_j = pow (part, alpha);
				}
			else {
				f_j = 0;
				} 
			f_j_2 = pow (degM, beta); 

			c_i_int+=ov*(f_j*f_j_2);

			/*chiudo il for*/
			}
	
		deg0=r_slap0_n[i+1]-r_slap0_n[i];
		deg1=r_slap1_n[i+1]-r_slap1_n[i];
		
		degM=(deg0+deg1)*1.0;
		prodM=(deg0*deg1)*1.0;
		part = (M/(M-1))*  (1-(pow((deg0/degM),2))-(pow((deg1/degM),2)) ); 

		intM=degM*part;

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


		cf_i_vec_add[i]=c_i_add*(f_j+f_j_2);


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



		cf_i_vec_mult[i]=c_i_mult*(f_j*f_j_2);
		//cf_i_vec_part[i]=c_i_part*part;
		if (part>0.0000000001) {
				f_j = pow (part, alpha);
				}
			else {
				f_j = 0;
				} 
			f_j_2 = pow (degM, beta); 
			


		cf_i_vec_int[i]=c_i_int*(f_j*f_j_2);
		 
		tot_add+=cf_i_vec_add[i];
		tot_mult+=cf_i_vec_mult[i];
		tot_part+=cf_i_vec_part[i];
		tot_int+=cf_i_vec_int[i];
		}
	
	
	double vec_add[N];
	double vec_mult[N];
	double vec_part[N];
	double vec_int[N];
	double tot_add_rid=0, tot_mult_rid=0, tot_part_rid=0, tot_int_rid=0;
	
	for (i=0; i<N; i++) {
		vec_add[i]=cf_i_vec_add[i]/tot_add;
		vec_mult[i]=cf_i_vec_mult[i]/tot_mult;
		vec_part[i]=cf_i_vec_part[i]/tot_part;
		vec_int[i]=cf_i_vec_int[i]/tot_int;
		tot_add_rid+=vec_add[i];
		tot_mult_rid+=vec_mult[i];
		tot_part_rid+=vec_part[i];
		tot_int_rid+=vec_int[i];
		
		}

	//sigma delle distr
	double average_add, variance_add, std_deviation_add, sum_add = 0, sum1_add = 0;
	double average_mult, variance_mult, std_deviation_mult, sum_mult = 0, sum1_mult = 0;
	
	double average_int, variance_int, std_deviation_int, sum_int = 0, sum1_int = 0;
	double sigma_norm_add, sigma_norm_mult, sigma_norm_int;
	for (i=0; i<N; i++) {
		printf("%d %g %g %g %g %g\n", i, vec_add[i],vec_mult[i],vec_int[i], alpha, beta);
		
		}

}
