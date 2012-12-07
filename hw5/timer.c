#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include "array_gen.h"
#include "count.h"


int main() {
      struct timeval start, end;
      int len = 1;
      int j;
      srand(time(NULL));
      char key = rand()%256;
      int num_iter = 100;
      for (j=0;j<5;j++) {
	  len = len*10+rand()%10;
	  char arr[len];
	  array_gen(arr,len);
	  gettimeofday( &start, NULL );
	  int r1 = count(arr, len, key);
	  int r2 = count_simd(arr, len, key);
	  if (r1 != r2) {
	      printf("Incorrect result for length %d: Expected %d, Got %d\n", len, r1, r2);
	      continue;
	  }
	  int i;
	  for (i=0; i< num_iter; i++)
	      r1 = count(arr, len, key);
	  gettimeofday( &end, NULL );
	  double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	  gettimeofday( &start, NULL );
	  for (i=0; i< num_iter; i++)
	      r2 = count_simd(arr, len, key);
	  gettimeofday( &end, NULL );
	  double seconds_simd = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	  printf("For length %d, the approximate speedup was %f\n", len, seconds/seconds_simd);
      }
      return 0;
}


    

