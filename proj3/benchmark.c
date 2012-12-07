#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cblas.h>

/* Your function must have the following signature: */

void sgemm( int m, int n, float *A, float *C );

/* The benchmarking program */

int main( int argc, char **argv )
{
	srand(time(NULL));
	
	double counter_random = 0.0;
	double total_random = 0.0;
	double sub5_random = 0;
	
	double total_60 = 0.0;
	double counter_60 = 0.0;
	int m = 60, n = 60;
	
	printf("\nTesting 60 by 60 matrices 20 times\n");
	
	for (int i = 0; i < 2; i++){
		/* Allocate and fill 2 random matrices A, C */
		float *A = (float*) malloc( m * n * sizeof(float) );
		float *C = (float*) malloc( m * m * sizeof(float) );
		
		for( int i = 0; i < m*n; i++ ) A[i] = 2 * drand48() - 1;
		for( int i = 0; i < m*m; i++ ) C[i] = 2 * drand48() - 1;
		
		/* measure Gflop/s rate; time a sufficiently long sequence of calls to eliminate noise */
		double Gflop_s, seconds = -1.0;
		for( int n_iterations = 1; seconds < 0.1; n_iterations *= 2 ) 
		{
			/* warm-up */
			sgemm( m, n, A, C );
			
			/* measure time */
			struct timeval start, end;
			gettimeofday( &start, NULL );
			for( int i = 0; i < n_iterations; i++ )
				sgemm( m,n, A, C );
			gettimeofday( &end, NULL );
			seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
			
			/* compute Gflop/s rate */
			Gflop_s = 2e-9 * n_iterations * m * m * n / seconds;
		}
		
		printf( "%d by %d matrix \t %g Gflop/s\n", m, n, Gflop_s );
		total_60 = total_60 + Gflop_s;
		counter_60 = counter_60 + 1;
		
		
		/* Ensure that error does not exceed the theoretical error bound */
		
		/* Set initial C to 0 and do matrix multiply of A*B */
		memset( C, 0, sizeof( float ) * m * m );
		sgemm( m,n, A, C );
		
		/* Subtract A*B from C using standard sgemm (note that this should be 0 to within machine roundoff) */
		cblas_sgemm( CblasColMajor,CblasNoTrans,CblasTrans, m,m,n, -1, A,m, A,m, 1, C,m );
		
		/* Subtract the maximum allowed roundoff from each element of C */
		for( int i = 0; i < m*n; i++ ) A[i] = fabs( A[i] );
		for( int i = 0; i < m*m; i++ ) C[i] = fabs( C[i] );
		cblas_sgemm( CblasColMajor,CblasNoTrans,CblasTrans, m,m,n, -3.0*FLT_EPSILON*n, A,m, A,m, 1, C,m );
		
		/* After this test if any element in C is still positive something went wrong in square_sgemm */
		for( int i = 0; i < m * m; i++ )
			if( C[i] > 0 ) {
				printf( "FAILURE: error in matrix multiply exceeds an acceptable margin\n" );
				return -1;
			}
		
		/* release memory */
		free( C );
		free( A );
	}
	
	printf("\nTesting random sizes from m = [32, 100] to n = [32, 300] 100 times\n");
	
	/* Try different m */
	for ( int i = 0; i < 10; i++ ){
		int m = 100 + (rand() % 9000);
		int n = 32 + (rand() % 68);
		
		
		/* Allocate and fill 2 random matrices A, C */
		float *A = (float*) malloc( m * n * sizeof(float) );
		float *C = (float*) malloc( m * m * sizeof(float) );
		
		for( int i = 0; i < m*n; i++ ) A[i] = 2 * drand48() - 1;
		for( int i = 0; i < m*m; i++ ) C[i] = 2 * drand48() - 1;
		
		/* measure Gflop/s rate; time a sufficiently long sequence of calls to eliminate noise */
		double Gflop_s, seconds = -1.0;
		for( int n_iterations = 1; seconds < 0.1; n_iterations *= 2 ) 
		{
			/* warm-up */
			sgemm( m, n, A, C );
			
			/* measure time */
			struct timeval start, end;
			gettimeofday( &start, NULL );
			for( int i = 0; i < n_iterations; i++ )
				sgemm( m,n, A, C );
			gettimeofday( &end, NULL );
			seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
			
			/* compute Gflop/s rate */
			Gflop_s = 2e-9 * n_iterations * m * m * n / seconds;
		}
		
		printf( "%d by %d matrix \t %g Gflop/s\n", m, n, Gflop_s );
		total_random = total_random + Gflop_s;
		counter_random = counter_random++;
		if (Gflop_s < 5.0){
			sub5_random++;
		}
		
		
		/* Ensure that error does not exceed the theoretical error bound */
		
		/* Set initial C to 0 and do matrix multiply of A*B */
		memset( C, 0, sizeof( float ) * m * m );
		sgemm( m,n, A, C );
		
		/* Subtract A*B from C using standard sgemm (note that this should be 0 to within machine roundoff) */
		cblas_sgemm( CblasColMajor,CblasNoTrans,CblasTrans, m,m,n, -1, A,m, A,m, 1, C,m );
		
		/* Subtract the maximum allowed roundoff from each element of C */
		for( int i = 0; i < m*n; i++ ) A[i] = fabs( A[i] );
		for( int i = 0; i < m*m; i++ ) C[i] = fabs( C[i] );
		cblas_sgemm( CblasColMajor,CblasNoTrans,CblasTrans, m,m,n, -3.0*FLT_EPSILON*n, A,m, A,m, 1, C,m );
		
		/* After this test if any element in C is still positive something went wrong in square_sgemm */
		for( int i = 0; i < m * m; i++ )
			if( C[i] > 0 ) {
				printf( "FAILURE: error in matrix multiply exceeds an acceptable margin\n" );
				return -1;
			}
		
		/* release memory */
		free( C );
		free( A );
		
		
	}
	
	double average_random = total_random/counter_random;
	double average_60 = total_60/counter_60;
	int total = 0;
	printf("\n\nAverage for 60 by 60: %.8f Gflop/s\n", average_60);
	printf("Average for random sizes: %.8f Gflop/s\n", average_random);
	printf("\nPotential Grade for 60 by 60:\n");
	if (average_60 >= 10.5){
		total = 35;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 10){
		total = 34;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 9){
		total = 32;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 8){
		total = 30;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 7){
		total = 25;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 6){
		total = 20;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 5){
		total = 15;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 4){
		total = 10;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 3){
		total = 7;
		printf("%d/35\n", total);
	}
	else if (average_60 >= 2){
		total = 4;
		printf("%d/35\n", total);
	}
	else {
		total = 1;
		printf("%d/35\n", total);
	}
	
	printf("All or nothing grade for random matrices:\n");
	if (average_random >= 5.0){
		total = total + 20;
		printf("20/20\n");
	}
	else {
		printf("0/20\n");
	}
	
	printf("\nPotential Total Grade: %d/55\n", total);
	printf("\nNo partial credit because I have no clue how that's going to work out.\n");
}