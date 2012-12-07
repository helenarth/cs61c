//Daniel Nakashima 	cs61c-hi
//Chung Fai Tsim 	cs61c-eh
#include <stdio.h>
#include <emmintrin.h> 
#include <string.h>
#include <omp.h>
/* This routine performs a sgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. 
 *ordered in column major ie row 2 = 2*n
 *column 2 row 3 = 2*n+3
 */    

void sgemm(int n, float* A, float* C)//, float* C)
{
    float *col0_A, *col1_A, *col2_A, *col3_A,*col4_A, *col5_A, *col6_A, *col7_A;
    float *C_index;
	int i,j,k;
    
	__m128 c0, c1, c2,c3,c4,c5,c6,c7;
	
    register __m128 s0 = _mm_setzero_ps();
    register __m128 s1 = _mm_setzero_ps();
    register __m128 s2 = _mm_setzero_ps();
    register __m128 s3 = _mm_setzero_ps();
    register __m128 s4 = _mm_setzero_ps();
    register __m128 s5 = _mm_setzero_ps();
    register __m128 s6 = _mm_setzero_ps();
    register __m128 s7 = _mm_setzero_ps();
    
    omp_set_num_threads(16); 
    
    if(n%16==0)
	{
    	
#pragma omp parallel for private( k,i,C_index,c1,c2,c3,c4,c5,c6,c7,c0,s0,s1,s2,s3,s4,s5,s6,s7,col0_A,col1_A,col2_A,col3_A,col4_A,col5_A,col6_A,col7_A ) 
		
#pragma omp schedule(dynamic)
		
		for (int j = 0; j < n; ++j)
		{
			for (int k = 0; k < n/8; ++k)//for every iteration of k find the starting index of each column
			{	  
				col0_A = A + k*8*n;
				col1_A = col0_A + n;
				col2_A = col1_A + n;
				col3_A = col2_A + n;
				col4_A = col3_A + n;
				col5_A = col4_A + n;
				col6_A = col5_A + n;
				col7_A = col6_A + n;
				
				C_index = C+j*n;
				
				c0 = _mm_load1_ps(C+k*8+j*n);
				c1 = _mm_load1_ps(C+k*8+j*n+1);
				c2 = _mm_load1_ps(C+k*8+j*n+2);
				c3 = _mm_load1_ps(C+k*8+j*n+3);
				c4 = _mm_load1_ps(C+k*8+j*n+4);
				c5 = _mm_load1_ps(C+k*8+j*n+5);
				c6 = _mm_load1_ps(C+k*8+j*n+6);
				c7 = _mm_load1_ps(C+k*8+j*n+7);
				
				for(int i = 0; i<n/16; ++i)    //this loop controls A as moves down the col
				{
					c0 = _mm_loadu_ps(C_index);
					s0 = _mm_loadu_ps(col0_A);
					s1 = _mm_loadu_ps(col1_A);
					s2 = _mm_loadu_ps(col2_A);
					s3 = _mm_loadu_ps(col3_A); 
					s4 = _mm_loadu_ps(col4_A);
					s5 = _mm_loadu_ps(col5_A);
					s6 = _mm_loadu_ps(col6_A);
					s7 = _mm_loadu_ps(col7_A);
					
					c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
					c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
					c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
					c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
					c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
					c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
					c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
					c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
					
					_mm_storeu_ps(C_index, c0);
					
					col0_A += 4;
					col1_A += 4;
					col2_A += 4;
					col3_A += 4;
					col4_A += 4;
					col5_A += 4;
					col6_A += 4;
					col7_A += 4;
					C_index += 4;
					
					
					//UNROLL 1x
					c0 = _mm_loadu_ps(C_index);
					s0 = _mm_loadu_ps(col0_A);
					s1 = _mm_loadu_ps(col1_A);
					s2 = _mm_loadu_ps(col2_A);
					s3 = _mm_loadu_ps(col3_A); 
					s4 = _mm_loadu_ps(col4_A);
					s5 = _mm_loadu_ps(col5_A);
					s6 = _mm_loadu_ps(col6_A);
					s7 = _mm_loadu_ps(col7_A);
					
					c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
					c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
					c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
					c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
					c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
					c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
					c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
					c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
					
					_mm_storeu_ps(C_index, c0);
					
					col0_A += 4;
					col1_A += 4;
					col2_A += 4;
					col3_A += 4;
					col4_A += 4;
					col5_A += 4;
					col6_A += 4;
					col7_A += 4;
					C_index += 4;
					
					//UNROLL 2x
					c0 = _mm_loadu_ps(C_index);
					s0 = _mm_loadu_ps(col0_A);
					s1 = _mm_loadu_ps(col1_A);
					s2 = _mm_loadu_ps(col2_A);
					s3 = _mm_loadu_ps(col3_A); 
					s4 = _mm_loadu_ps(col4_A);
					s5 = _mm_loadu_ps(col5_A);
					s6 = _mm_loadu_ps(col6_A);
					s7 = _mm_loadu_ps(col7_A);
					
					c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
					c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
					c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
					c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
					c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
					c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
					c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
					c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
					
					_mm_storeu_ps(C_index, c0);
					
					col0_A += 4;
					col1_A += 4;
					col2_A += 4;
					col3_A += 4;
					col4_A += 4;
					col5_A += 4;
					col6_A += 4;
					col7_A += 4;
					C_index += 4;
					
					//UNROLL 3x
					c0 = _mm_loadu_ps(C_index);
					s0 = _mm_loadu_ps(col0_A);
					s1 = _mm_loadu_ps(col1_A);
					s2 = _mm_loadu_ps(col2_A);
					s3 = _mm_loadu_ps(col3_A); 
					s4 = _mm_loadu_ps(col4_A);
					s5 = _mm_loadu_ps(col5_A);
					s6 = _mm_loadu_ps(col6_A);
					s7 = _mm_loadu_ps(col7_A);
					
					c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
					c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
					c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
					c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
					c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
					c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
					c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
					c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
					
					_mm_storeu_ps(C_index, c0);
					
					col0_A += 4;
					col1_A += 4;
					col2_A += 4;
					col3_A += 4;
					col4_A += 4;
					col5_A += 4;
					col6_A += 4;
					col7_A += 4;
					C_index += 4;
				}//i
			}//k
		}//j
	}//if
    
    else
	{
    	int nx = n/8*8 +8; 
   		float *Ax = malloc(nx*nx*sizeof(float));
    	float *Cx = malloc(nx*nx*sizeof(float));
		
#pragma omp parallel
    	{ 
#pragma omp for schedule(dynamic)
			for(int i =0; i<n; i++){
				memcpy(Ax+i*nx, A+i*n, n*sizeof(float));
				memset(Ax+i*nx+n, 0, sizeof( float )*(nx-n));	
			}
			
			
#pragma omp for schedule(dynamic)
			for(int i =0; i<n; i++){
				memcpy(Cx+i*nx, C+i*n, n*sizeof(float));
				memset(Cx+i*nx+n, 0, sizeof( float )*(nx-n));	
			}
			
#pragma omp barrier
    	}    	
		
		int temp=n;  	
    	n=nx;
		
#pragma omp parallel private( k,i, C_index,c1,c2,c3,c4,c5,c6,c7,c0,s0,s1,s2,s3,s4,s5,s6,s7,col0_A,col1_A,col2_A,col3_A,col4_A,col5_A,col6_A,col7_A ) 
		{
#pragma omp for schedule(dynamic)
			
			
			for (int j = 0; j < n; ++j)
			{				
				for (int k = 0; k < n/8; ++k)//for every iteration of k find the starting index of each column
				{
					col0_A = Ax + (k*8)*n;
					col1_A = col0_A + n;
					col2_A = col1_A + n;
					col3_A = col2_A + n;
					col4_A = col3_A + n;
					col5_A = col4_A + n;
					col6_A = col5_A + n;
					col7_A = col6_A + n;
					
					C_index = Cx+j*n;
					
					c0 = _mm_load1_ps(Cx+k*8+j*n);
					c1 = _mm_load1_ps(Cx+k*8+j*n+1);
					c2 = _mm_load1_ps(Cx+k*8+j*n+2);
					c3 = _mm_load1_ps(Cx+k*8+j*n+3);
					c4 = _mm_load1_ps(Cx+k*8+j*n+4);
					c5 = _mm_load1_ps(Cx+k*8+j*n+5);
					c6 = _mm_load1_ps(Cx+k*8+j*n+6);
					c7 = _mm_load1_ps(Cx+k*8+j*n+7);
					
					for(int i = 0; i<n/8; ++i)    //this loop controls A as moves down the col
					{
						//UNROLL 0x
						c0 = _mm_loadu_ps(C_index);
						s0 = _mm_loadu_ps(col0_A);
						s1 = _mm_loadu_ps(col1_A);
						s2 = _mm_loadu_ps(col2_A);
						s3 = _mm_loadu_ps(col3_A); 
						s4 = _mm_loadu_ps(col4_A);
						s5 = _mm_loadu_ps(col5_A);
						s6 = _mm_loadu_ps(col6_A);
						s7 = _mm_loadu_ps(col7_A);
						
						c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
						c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
						c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
						c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
						c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
						c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
						c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
						c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
						
						_mm_storeu_ps(C_index, c0);
						
						col0_A += 4;
						col1_A += 4;
						col2_A += 4;
						col3_A += 4;
						col4_A += 4;
						col5_A += 4;
						col6_A += 4;
						col7_A += 4;
						C_index += 4;
						
						//UNROLL 1x
						c0 = _mm_loadu_ps(C_index);
						s0 = _mm_loadu_ps(col0_A);
						s1 = _mm_loadu_ps(col1_A);
						s2 = _mm_loadu_ps(col2_A);
						s3 = _mm_loadu_ps(col3_A); 
						s4 = _mm_loadu_ps(col4_A);
						s5 = _mm_loadu_ps(col5_A);
						s6 = _mm_loadu_ps(col6_A);
						s7 = _mm_loadu_ps(col7_A);
						
						c0 =_mm_mul_ps(s0,c0);// _mm_add_ps(c0,_mm_mul_ps(s0,c0));
						c0 =_mm_mul_ps(s1,c1);//_mm_add_ps(c0,_mm_mul_ps(s1,c1));
						c0 =_mm_mul_ps(s2,c2);// _mm_add_ps(c0,_mm_mul_ps(s2,c2));
						c0 =_mm_mul_ps(s3,c3);// _mm_add_ps(c0,_mm_mul_ps(s3,c3));
						c0 =_mm_mul_ps(s4,c4);// _mm_add_ps(c0,_mm_mul_ps(s4,c4));
						c0 =_mm_mul_ps(s5,c5);// _mm_add_ps(c0,_mm_mul_ps(s5,c5));
						c0 =_mm_mul_ps(s6,c6);// _mm_add_ps(c0,_mm_mul_ps(s6,c6));
						c0 =_mm_mul_ps(s7,c7);// _mm_add_ps(c0,_mm_mul_ps(s7,c7));
						
						_mm_storeu_ps(C_index, c0);
						
						col0_A += 4;
						col1_A += 4;
						col2_A += 4;
						col3_A += 4;
						col4_A += 4;
						col5_A += 4;
						col6_A += 4;
						col7_A += 4;
						C_index += 4;
					}
				}	
			}
		}
		
		
		n=temp;
		for(int i =0; i<n; i++)
		{memcpy(C+i*n, Cx+i*nx, n*sizeof(float));}
    	
		free(Ax);
		free(Cx);
		//free(Cx);
	}
	
}//fn end
