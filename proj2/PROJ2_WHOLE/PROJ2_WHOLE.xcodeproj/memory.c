#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "processor.h"

/* Pointer to simulator memory */
uint8_t *mem;

/* Called by program loader to initialize memory. */
uint8_t *init_mem() {
	assert (mem == NULL);
	mem = calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
	return mem;
}

/* Returns 1 if memory access is ok, otherwise 0 */
int access_ok(uint32_t mipsaddr, mem_unit_t size) {
/*	
	if (mipsaddr >= 1) {
		if (mipsaddr < MEM_SIZE) {
			if (mipsaddr % (uint32_t)size == 0) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
	*/
	if ((size != 4) & (size != 2) & (size != 1)) {
		return 0;
    }
    return ((mipsaddr % size) == 0);
	//return (mipsaddr >= 1) & (mipsaddr < MEM_SIZE) & ((mipsaddr % size) == 0);
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
	
	if (!access_ok(mipsaddr, size)) {
		fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
		exit(-1);
	}
	/*
	if (size == SIZE_BYTE) 
	{
		*(uint8_t*)(mem + mipsaddr) = value;
	} 
	else if (size == SIZE_WORD) 
	{
		int i; 
		for (i = size - 1; i >= 0; i--) 
		{
			*(mem + mipsaddr + i) = (value & ((int32_t) 0x000000FF << 2*size*i)) >> 2*size*i;
		}
	} 
	*/
	if (size == 4) {
		*(uint32_t*)(mem + mipsaddr) = (uint32_t)value;
	} else if (size == 2) {
		*(uint16_t*)(mem + mipsaddr) = (uint16_t)value;
	} else if (size == 1) {
		*(uint8_t*)(mem + mipsaddr) = (uint8_t)value;
	} else {
		fprintf(stderr, "bad size in store=%d, addr=0x%x\n", size, mipsaddr);
		exit(-1);
	}      
	/*
	 switch (size) {
	 
	 case SIZE_HALF_WORD:
	 //tmp = (uint32_t*)((*(uint16_t*)(mem + mipsaddr)) & 0xFFFFFFFF);
	 *(int16_t*)(mem + mipsaddr) = value;	
	 break;
	 
	 case SIZE_BYTE:
	 //tmp = (uint32_t*)((*(uint8_t*)(mem + mipsaddr)) & 0xFFFFFFFF);
	 *(int8_t*)(mem + mipsaddr) = value;	
	 break;
	 
	 default:
	 *(int32_t*)(mem + mipsaddr) = value;
	 break;
	 }
	 */
}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {
	
	if (!access_ok(mipsaddr, size)) {
		fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
		exit(-1);
	}
	
	
	//return (size == SIZE_WORD) ? *(uint32_t*)(mem + mipsaddr) : (*(uint8_t*)(mem + mipsaddr));
	
	if (size == 4) {
		return *(uint32_t*)(mem + mipsaddr);
	} else if (size == 2) {
		return *(uint16_t*)(mem + mipsaddr);
	} else if (size == 1) {
		return *(uint8_t*)(mem + mipsaddr);
	} else {
		fprintf(stderr, "bad size in load, size=%d, addr=0x%x\n", size, mipsaddr);
		exit(-1);
	}      
	//uint32_t tmp = 0x00000000;
	/*switch (size) {
	 
	 case SIZE_BYTE:
	 return ((uint32_t)*(uint8_t*)(mem + mipsaddr));
	 //tmp = ((*(uint32_t*)(mem + mipsaddr)) & 0x000000FF);
	 break;
	 
	 case SIZE_HALF_WORD:
	 return ((uint32_t)*(uint16_t*)(mem + mipsaddr));
	 //tmp =  ((*(uint32_t*)(mem + mipsaddr)) & 0x0000FFFF);
	 break;
	 
	 default:
	 return *(uint32_t*)(mem + mipsaddr);
	 break;;
	 }
	 //return tmp;*/
}
