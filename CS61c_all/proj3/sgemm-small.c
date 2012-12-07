//Daniel Nakashima - hi
//Chung Fai Tsim - eh

#include <stdio.h>
#include <emmintrin.h> 
/* This routine performs a sgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. 
		*ordered in column major ie row 2 = 2*n
		*column 2 row 3 = 2*n+3
*/    
/*
    int i,j,k;
    for( k = 0; i < n; i++ )
		for( i = 0; k < n; k++ )
			for( j = 0; j < n; j++ )
				C[i+j*n] += A[i+k*n]*B[k+j*n];
*/
//k controls how you move right from A and down B
//j controls how you move right from C and right from B
//i controls how you move down A and down C

 
void square_sgemm(int n, float* A, float* B, float* C)
{
   
    float *col0_A, *col1_A, *col2_A, *col3_A,*col4_A, *col5_A, *col6_A, *col7_A;

    float *C_index;
    float* b_t , *a0;

    
     __m128 b0, b1, b2,b3,b4,b5,b6,b7,c0;	
    //register to use later
    //stored column values
    register __m128 s0 = _mm_setzero_ps();
    register __m128 s1 = _mm_setzero_ps();
    register __m128 s2 = _mm_setzero_ps();
    register __m128 s3 = _mm_setzero_ps();
    register __m128 s4 = _mm_setzero_ps();
    register __m128 s5 = _mm_setzero_ps();
    register __m128 s6 = _mm_setzero_ps();
    register __m128 s7 = _mm_setzero_ps();


    if(n%64==0){
	//for each row j of B
    
	for (int k = 0; k < n; k+=8)//for every iteration of k find the starting index of each column
	{

		for (int j = 0; j < n; j++)
	  	{

		  
		        col0_A = A + k*n;
			col1_A = col0_A + n;
			col2_A = col1_A + n;
			col3_A = col2_A + n;
			col4_A = col3_A + n;
			col5_A = col4_A + n;
			col6_A = col5_A + n;
			col7_A = col6_A + n;
			
			C_index = C+j*n;

	                b0 = _mm_load1_ps(B+k+j*n);
			b1 = _mm_load1_ps(B+k+j*n+1);
			b2 = _mm_load1_ps(B+k+j*n+2);
			b3 = _mm_load1_ps(B+k+j*n+3);
			b4 = _mm_load1_ps(B+k+j*n+4);
			b5 = _mm_load1_ps(B+k+j*n+5);
			b6 = _mm_load1_ps(B+k+j*n+6);
			b7 = _mm_load1_ps(B+k+j*n+7);
			
			for(int i = 0; i<n/8*8; i+=8)    //this loop controls A as moves down the col
			{
			   
				//UNROLL 1 + 0

				c0 = _mm_loadu_ps(C_index);
				s0 = _mm_loadu_ps(col0_A);
				s1 = _mm_loadu_ps(col1_A);
				s2 = _mm_loadu_ps(col2_A);
				s3 = _mm_loadu_ps(col3_A); 
				s4 = _mm_loadu_ps(col4_A);
				s5 = _mm_loadu_ps(col5_A);
				s6 = _mm_loadu_ps(col6_A);
				s7 = _mm_loadu_ps(col7_A);
				
				
				c0 = _mm_add_ps(c0,_mm_mul_ps(s0,b0));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s1,b1));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s2,b2));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s3,b3));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s4,b4));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s5,b5));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s6,b6));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s7,b7));
				
				
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
			
			
			c0 = _mm_loadu_ps(C_index);
				s0 = _mm_loadu_ps(col0_A);
				s1 = _mm_loadu_ps(col1_A);
				s2 = _mm_loadu_ps(col2_A);
				s3 = _mm_loadu_ps(col3_A); 
				s4 = _mm_loadu_ps(col4_A);
				s5 = _mm_loadu_ps(col5_A);
				s6 = _mm_loadu_ps(col6_A);
				s7 = _mm_loadu_ps(col7_A);
						
				c0 = _mm_add_ps(c0,_mm_mul_ps(s0,b0));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s1,b1));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s2,b2));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s3,b3));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s4,b4));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s5,b5));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s6,b6));
				c0 = _mm_add_ps(c0,_mm_mul_ps(s7,b7));
				
				
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
    
    else{

	//for each row j of B
    
	for (int k = 0; k<n; k++)//for every iteration of k find the starting index of each column
	    { 
		
		for (int j = 0; j < n; j++)
	  	{
		  
		    a0 = A + k*n;
		    //a1 = a0 + n;
		    //a2 = a1 + n;
		    //a3 = a2 + n;
		    C_index = C+j*n;
			
		    b0 = _mm_load1_ps(B+k+j*n+0);
		    //b1 = _mm_load1_ps(B+k+j*n+1);
		    //		    b2 = _mm_load1_ps(B+k+j*n+2);
		    //b3 = _mm_load1_ps(B+k+j*n+3);		   
		    //innermost loop iterating down column of A
		    //implements register blocking
		    //actual calculation takes place here
		    for(int i = 0; i<n; i+=4)    //this loop controls A as moves down the col
			{
			    if (i+4>n)
				{
				    int offset = n-i;
				    for (int g=0; g<offset; g++)
					{
					    b_t = (B+k+j*n);
					    *(C_index+g) = *(C_index+g) + (*(a0+g) * *b_t);
					}
				    break;
				}
			     
			    s0 = _mm_loadu_ps(a0);

			    c0 = _mm_loadu_ps(C_index);
			    c0 = _mm_add_ps(c0,_mm_mul_ps(s0,b0));
			    	
			    _mm_storeu_ps(C_index, c0);
				
			    a0 += 4;
			    C_index += 4;
			}
		  }	
	}
	
    }
	

	
 }
