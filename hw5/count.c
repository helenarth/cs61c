#include <emmintrin.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int lenGra16(char *arr, int len, char key, int a, int b);
int lenGra64(char *arr, int len, char key, int a, int b);

int count(char *arr, int len, char key) {
    int i;
    int res = 0;
    for (i=0;i<len;++i)
		if (arr[i] == key)
			res++;
    return res;	
}

int count_simd(char *arr, int len, char key) {
	int i;
	int inRes = 0;
	int t[16];
	int* p = t;
	/*
	if (len >= 64) {
		inRes = lenGra64(arr, len, key, 0, 64);
		//inRes += lenGra16(arr, len, key, len/64*64, 16);
		for (i=len/64*64;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
	else if (len >= 16) {
		inRes = lenGra16(arr, len, key, 0, 16);
		for (i=len/16*16;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
	else {
		for (i=len/16*16;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
	*/
	__m128i counter = _mm_setzero_si128();
	__m128i nKey = _mm_set1_epi8(key);
	
	if (len >= 64) {
		for (i = 0; i < len/64*64; i += 64) {
			counter += _mm_cmpeq_epi8(nKey, _mm_loadu_si128((__m128i*)(arr+i)));
			counter += _mm_cmpeq_epi8(nKey, _mm_loadu_si128((__m128i*)(arr+i+1)));
			counter += _mm_cmpeq_epi8(nKey, _mm_loadu_si128((__m128i*)(arr+i+2)));
			counter += _mm_cmpeq_epi8(nKey, _mm_loadu_si128((__m128i*)(arr+i+3)));
		}
		_mm_storeu_si128((__m128i*)p,counter);
		inRes += abs(t[0]+t[1]+t[2]+t[3]+t[4]+t[5]+t[6]+t[7]+t[8]+t[9]+t[10]+t[11]+t[12]+t[13]+t[14]+t[15]);
		counter = _mm_setzero_si128();
		inRes += lenGra16(arr, len, key, 0, 16);
		for (i=len/16*16;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
	else if (len >= 16) {
		inRes = lenGra16(arr, len, key, 0, 16);
		for (i=len/16*16;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
	else {
		for (i=len/16*16;i<len;++i){
			if (arr[i] == key){
				inRes++;
			}
		}
		return inRes;
	}
}
/*int lenGra64(char *arr, int len, char key, int a){
 
 lenGra16(arr, len, key, 16, 16
 lenGra16(arr, len, key, 0, 16);
 
 return res;	
 }*/

int lenGra16(char *arr, int len, char key, int a, int b){
	int res = 0;
	int i = a;
	for (i=a;i<len/b*b;i += b){
		if (arr[i] == key)
			res++;
		if (arr[i+1] == key)
			res++;
		if (arr[i+2] == key)
			res++;
		if (arr[i+3] == key)
			res++;
		if (arr[i+4] == key)
			res++;
		if (arr[i+5] == key)
			res++;
		if (arr[i+6] == key)
			res++;
		if (arr[i+7] == key)
			res++;
		if (arr[i+8] == key)
			res++;
		if (arr[i+9] == key)
			res++;
		if (arr[i+10] == key)
			res++;
		if (arr[i+11] == key)
			res++;
		if (arr[i+12] == key)
			res++;
		if (arr[i+13] == key)
			res++;
		if (arr[i+14] == key)
			res++;
		if (arr[i+15] == key)
			res++;
	}
    return res;	
}

int lenGra64(char *arr, int len, char key, int a, int b){
	int res = 0;
	int i = a;
	for (i=a;i<len/b*b;i += b){
		if (arr[i] == key)
			res++;
		if (arr[i+1] == key)
			res++;
		if (arr[i+2] == key)
			res++;
		if (arr[i+3] == key)
			res++;
		if (arr[i+4] == key)
			res++;
		if (arr[i+5] == key)
			res++;
		if (arr[i+6] == key)
			res++;
		if (arr[i+7] == key)
			res++;
		if (arr[i+8] == key)
			res++;
		if (arr[i+9] == key)
			res++;
		if (arr[i+10] == key)
			res++;
		if (arr[i+11] == key)
			res++;
		if (arr[i+12] == key)
			res++;
		if (arr[i+13] == key)
			res++;
		if (arr[i+14] == key)
			res++;
		if (arr[i+15] == key)
			res++;
		if (arr[i+16] == key)
			res++;
		if (arr[i+17] == key)
			res++;
		if (arr[i+18] == key)
			res++;
		if (arr[i+19] == key)
			res++;
		if (arr[i+20] == key)
			res++;
		if (arr[i+21] == key)
			res++;
		if (arr[i+22] == key)
			res++;
		if (arr[i+23] == key)
			res++;
		if (arr[i+24] == key)
			res++;
		if (arr[i+25] == key)
			res++;
		if (arr[i+26] == key)
			res++;
		if (arr[i+27] == key)
			res++;
		if (arr[i+28] == key)
			res++;
		if (arr[i+29] == key)
			res++;
		if (arr[i+30] == key)
			res++;
		if (arr[i+32] == key)
			res++;
		if (arr[i+33] == key)
			res++;
		if (arr[i+34] == key)
			res++;
		if (arr[i+35] == key)
			res++;
		if (arr[i+36] == key)
			res++;
		if (arr[i+37] == key)
			res++;
		if (arr[i+38] == key)
			res++;
		if (arr[i+39] == key)
			res++;
		if (arr[i+40] == key)
			res++;
		if (arr[i+41] == key)
			res++;
		if (arr[i+42] == key)
			res++;
		if (arr[i+43] == key)
			res++;
		if (arr[i+44] == key)
			res++;
		if (arr[i+45] == key)
			res++;
		if (arr[i+46] == key)
			res++;
		if (arr[i+47] == key)
			res++;
		if (arr[i+48] == key)
			res++;
		if (arr[i+49] == key)
			res++;
		if (arr[i+50] == key)
			res++;
		if (arr[i+51] == key)
			res++;
		if (arr[i+52] == key)
			res++;
		if (arr[i+53] == key)
			res++;
		if (arr[i+54] == key)
			res++;
		if (arr[i+55] == key)
			res++;
		if (arr[i+56] == key)
			res++;
		if (arr[i+57] == key)
			res++;
		if (arr[i+58] == key)
			res++;
		if (arr[i+59] == key)
			res++;
		if (arr[i+60] == key)
			res++;
		if (arr[i+61] == key)
			res++;
		if (arr[i+62] == key)
			res++;
		if (arr[i+63] == key)
			res++;
	}
    return res;	
}