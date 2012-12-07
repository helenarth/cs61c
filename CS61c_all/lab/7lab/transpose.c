#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void transpose( int n, int blocksize, int *dst, int *src ) {
    int i,j;
    int l,k;
  
    /* create block size of blocksize then transpose those blocks*/
    /* TO DO: implement blocking (two more loops) */
    /* 
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            dst[j+i*n] = src[i+j*n];
    */
    // number of blocks within the matrix
    // take a block and transpose that block then move that block
    // to correct position
    
    //this will transpose a block
    //outer loops will traverse block by block
    
    for (i=0; i<n; i+=blocksize)
	for (j=0; j<n; j+=blocksize)
	    for(k=i; k<i+blocksize && k<n; k++)
		for(l=j; l<j+blocksize && l<n; l++)
		    dst[k+l*n] = src[l+k*n];
    
}

int main( int argc, char **argv ) {
    int n = 2000,i,j;
    int blocksize = 2; /* to do: find a better block size */

    /* allocate an n*n block of integers for the matrices */
    int *A = (int*)malloc( n*n*sizeof(int) );
    int *B = (int*)malloc( n*n*sizeof(int) );

    /* initialize A,B to random integers */
    srand48( time( NULL ) );
    for( i = 0; i < n*n; i++ ) A[i] = lrand48( );
    for( i = 0; i < n*n; i++ ) B[i] = lrand48( );

    /* measure performance */
    struct timeval start, end;

    gettimeofday(&start, NULL);
    transpose(n, blocksize, B, A);
    gettimeofday(&end, NULL);

    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    printf( "%g milliseconds\n", seconds*1e3 );

    /* check correctness */
    for( i = 0; i < n; i++ )
        for( j = 0; j < n; j++ )
            if( B[j+i*n] != A[i+j*n] ) {
	        printf("Error!!!! Transpose does not result in correct answer!!\n");
	        exit( -1 );
            }
  
    /* release resources */
    free( A );
    free( B );
    return 0;
}

