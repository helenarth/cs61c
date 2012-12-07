#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

#define TRUE 1
#define FALSE 0

/* Pointer to simulator memory */
uint8_t *mem;

/* Called by program loader to initialize memory. */
uint8_t *init_mem() 
{
	assert (mem == NULL);
	mem = calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
	return mem;
}
/*
inputs:
mipsaddr => 32 bit unsigned integer
size	 =>	enum can be either 1,2,4 form SIZE_BYTE, _HALF_WORD, _WORD
*/
int access_ok(uint32_t mipsaddr, mem_unit_t size) 
{
	//check if mipsaddr is within range
	/* TODO YOUR CODE HERE */
	if (mipsaddr>0 && mipsaddr<MEM_SIZE)
		switch(size)
		{
			case SIZE_WORD:
				if (mipsaddr%4==0)
					return TRUE;
				break;
			case SIZE_HALF_WORD:
				if (mipsaddr%2==0)
					return TRUE;
				break;
			case SIZE_BYTE:	//ALWAYS TRUE
				return TRUE;
				break;
		}
	return FALSE;
}

/*
 Writes size bytes of value into mips memory at mipsaddr
inputs: 
mipsaddr => 32 bit unsigned integer
size =>		enum can be either 1,2,4 form SIZE_BYTE, _HALF_WORD, _WORD
value => 	32 bit unsigned integer
*/


void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) 
{	int32_t mask_word = 0x000000FF;
	int16_t mask_half = 0x00FF;
	int i;
	
	if (!access_ok(mipsaddr, size)) 
	{
		fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
		exit(-1);
	}
	
	switch (size)
	{
	//the following for loops first isolate the desired bits using mask/and statement //then does appropriate little endian repositioning
	//finally assigning value to appropriate address in memory 
		case SIZE_WORD:
			for (i=0; i<SIZE_WORD; i++) {
				*(mem + mipsaddr + i) = (value & (mask_word<<8*i)) >> 8*i; }
			break;

		case SIZE_HALF_WORD:
			for (i=0; i<SIZE_HALF_WORD; i++) {
				*(mem + mipsaddr + i) = (value & (mask_half<<8*i)) >> 8*i; }
			break;

		case SIZE_BYTE:	//SIZE_BYTE
			*(mem+mipsaddr) = value;
			break;	
	}	
}	

/* Returns zero-extended value from mips memory 
inputs:
mipsaddr => 32 bit unsigned integer
size	 =>	enum can be either 1,2,4 form SIZE_BYTE, _HALF_WORD, _WORD
*/
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) 
{
    uint8_t temp_byte;
    uint16_t temp_halfword;
	
	if (!access_ok(mipsaddr, size)) 
	{
		fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
		exit(-1);
	}
	
	switch (size)
	{
		case SIZE_BYTE:
		    temp_byte = *(uint8_t*)(mem+mipsaddr);
		    return (uint32_t)temp_byte;
			break;
		case SIZE_HALF_WORD:
		    temp_halfword = *(uint16_t*)(mem + mipsaddr);
			return (uint32_t)temp_halfword;
			break;
		case SIZE_WORD:
			break;
	}
	return *(uint32_t*)(mem + mipsaddr);
}
