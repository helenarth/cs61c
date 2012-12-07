#include <stdio.h>
#include <omp.h>
#define NUM_THREADS 16


int main()
{
    static long num_steps = 10000;
    double step = 1.0/(double)num_steps; //delta x ;
    double pi = 0.0;
       
    omp_set_num_threads(NUM_THREADS);	//set number of threads
    
    #pragma omp parallel //shared(num_steps,id), private(sum)
    {
	double x;
	double sum=0.0;
	int id = omp_get_thread_num();
	    
	for (int i=0; i<num_steps; i+=NUM_THREADS)
	{
	    x = (i+id) * step;
	    sum += 4.0/(1.0+x*x);
	}
    
    
    #pragma omp critical
	pi += sum*step;
    }

    printf("VALUE OF PI IS => %f", pi);
    return 0;
}
