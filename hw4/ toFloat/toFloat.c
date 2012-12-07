/*
 *  toFloat.c
 *   toFloat
 *
 *  Created by Nathan brummel on 9/30/12.
 *  Copyright 2012 Berkeley. All rights reserved.
 *
 */
#include <stdlib.h>
#include "toFloat.h"
float unsignedToFloat( unsigned int x );

int main(){

	float x = unsignedToFloat(54984);
	printf("the value of 54984 in float is %f", x);
	
	
	return 0;
}

float unsignedToFloat( unsigned int x ) {
	
	unsigned int result = 0;
	unsigned int bucket = 0x80000000;
	int power = 1;
	int tmp;
	if(x == 0) {
		result = 0;
	}
	
	while((1 << power) <= x) {
		power++;
	}
	
	power--;
	tmp = (x - (1 << power)) << 23;	   //set tmp to x- (2^powerOfTwo)  
	         //bitshift tmp to the left by 23
	tmp = tmp >> power;    	   //bitshift tmp to the right by the powerOfTwo
	power += 127;          	   // add the power bias of 127
	power = power << 23;     // Bitshift the biased powerOfTwo
	result = power | tmp; 	 	   //or-add to the mantissa  
	
	if(x == 0) {
		result = 0;
	}
	return *(float*)&result;
}