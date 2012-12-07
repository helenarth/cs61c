#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h> /* where intrinsics are defined */

#define CLOCK_RATE_GHZ 2.26e9

/* Time stamp counter from Lecture 2/17 */
static __inline__ unsigned long long RDTSC(void) {
    unsigned hi,lo;
    __asm__ volatile("rdtsc" : "=a"(lo),"=d"(hi));
    return ((unsigned long long) lo)| (((unsigned long long)hi) << 32);
}

int sum_naive( int n, int *a )
{
    int sum = 0;
    for( int i = 0; i < n; i++ )
        sum += a[i];
    return sum;
}

int sum_unrolled( int n, int *a )
{
    int sum = 0;

    /* do the body of the work in a faster unrolled loop */
    for( int i = 0; i < n/4*4; i += 4 )
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
    }

    /* handle the small tail in a usual way */
    for( int i = n/4*4; i < n; i++ )   
        sum += a[i];

    return sum;
}

int sum_vectorized( int n, int *a )
{
    int return_val = 0;
    int return_array[4] = {0,0,0,0};
    __m128i sum  = _mm_setzero_si128();
    __m128i val_0 = _mm_setzero_si128();

    for(int i=0; i<n/4*4; i+=4)
    {
	val_0 = _mm_loadu_si128(a+i);
	sum = _mm_add_epi32(sum, val_0);
    }

    _mm_storeu_si128(return_array,sum);

    for(int i=0; i<n%4; i++)	//if not divis by 4 rest naive
	return_val += *(a+n-i-1);
    for(int i=0; i<4; i++)	//sum values from the 128 bit
	return_val += return_array[i];

    return return_val;
}
int sum_vectorized_unrolled( int n, int *a )
{
    /* UNROLL YOUR VECTORIZED CODE HERE*/
	int return_val = 0;
	int return_array[4] = {0,0,0,0}; //array of zeros to store final
	__m128i sum = _mm_setzero_si128();
	__m128i val_0 = _mm_setzero_si128();
	__m128i val_1 = _mm_setzero_si128();
	__m128i val_2 = _mm_setzero_si128();
	__m128i val_3 = _mm_setzero_si128();
	
	for (int i = 0; i<n/16*16; i+=16)	//128/32  = 4
	{
		val_0 = _mm_loadu_si128( (a+i) );
		val_1 = _mm_loadu_si128( (a+i+4) );
		val_2 = _mm_loadu_si128( (a+i+8) );
		val_3 = _mm_loadu_si128( (a+i+12) );
		
		sum = _mm_add_epi32(sum, val_0);
		sum = _mm_add_epi32(sum, val_1);
		sum = _mm_add_epi32(sum, val_2);
		sum = _mm_add_epi32(sum, val_3);
	}
	 _mm_storeu_si128(return_array,sum);
	 
    for(int i=0; i<(n%16); i++)	//if not divis by 4*4 rest naive
        return_val += *(a+n+i-1);

    for (int i=0; i<4; i++)	//sum values from the 128 bit
	return_val += return_array[i];

    return return_val;
}

void benchmark( int n, int *a, int (*computeSum)(int,int*), char *name )
{
    /* warm up */
    int sum = computeSum( n, a );

    /* measure */
    unsigned long long cycles = RDTSC();
    sum += computeSum( n, a );
    cycles = RDTSC()-cycles;
    
    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;
    
    /* report */
    printf( "%20s: ", name );
    if( sum == 2*sum_naive(n,a) ) 
		printf( "%.2f microseconds\n", microseconds );
    else
		printf( "ERROR!\n" );
}

int main( int argc, char **argv )
{
    const int n = 7777; /* small enough to fit in cache */
    
    /* init the array */
    int a[n] __attribute__ ((aligned (16))); /* align the array in memory by 16 bytes */
    for( int i = 0; i < n; i++ ) a[i] = rand( );
    
    /* benchmark series of codes */
    benchmark( n, a, sum_naive, "naive" );
    benchmark( n, a, sum_unrolled, "unrolled" );
    benchmark( n, a, sum_vectorized, "vectorized" );
    benchmark( n, a, sum_vectorized_unrolled, "vectorized unrolled" );

    return 0;
}

