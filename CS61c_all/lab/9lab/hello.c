#include <stdio.h>
#include <omp.h>

int main() {
	# pragma omp parallel
	{
		int thread_ID = omp_get_thread_num();
		int thread_tot = omp_get_num_threads();
		
		for (int i=0; i<5; i++)
		    {
			printf("count is \t %d\n",i);
			printf("ID =>\t\t%d\n", thread_ID);
			printf("TOTAL =>\t%d\n\n", thread_tot);
		    }

	}
}
