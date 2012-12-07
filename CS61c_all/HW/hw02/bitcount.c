/*
Name:			Daniel Nakashima
LAB sec time:	Friday 1-3
hw2_3 a and b
*/
	
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int bitCount(unsigned int n);

int main( int argc, char *argv[] ) 
{	
	int input_num;
	
	if (argc == 1)
	{
		printf ("# 1-bits in base 2 representation of %u = %d, should be 0\n", 0, bitCount (0));
		printf ("# 1-bits in base 2 representation of %u = %d, should be 1\n", 1, bitCount (1));
		printf ("# 1-bits in base 2 representation of %u = %d, should be 17\n", 2863377066u, bitCount(2863377066u));
		printf ("# 1-bits in base 2 representation of %u = %d, should be 1\n", 268435456, bitCount(268435456));
		printf ("# 1-bits in base 2 representation of %u = %d, should be 31\n", 4294705151u, bitCount(4294705151u));
    }
	else if (argc == 2)
	{
		input_num = atoi(argv[1]);
		printf("=> %d", bitCount(input_num));
    }
	else
	{
		printf("too many arguments");
    }
		
	getchar();
    return 0;
}

int bitCount(unsigned int n) 
{
	/* your code here */
    int i = 0;
	int total = 0;
	while (pow(2,i) <= n) 
	{i++;}
	
	while (i>=0) 
	{
		if (n/(pow(2,i)) >= 1) 
		{
			total++;
			n -= pow(2,i);
		}
		i--;
    }  
	return total;
}	

