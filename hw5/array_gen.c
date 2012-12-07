#include <time.h>
#include <stdlib.h>

void array_gen(char *res, int size) {
    srand(time(NULL));
    int i;
    for (i=0; i<size;++i)
	res[i] = rand()%256;
}
