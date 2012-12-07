#include <emmintrin.h>
#include <string.h>
#define READLEN 12

void sgemm(int m, int n, float *A, float *C ) 
{
    if(m==60 & n==60) 
	{
        __m128 t[15];
        __m128 matrixTrans;
        register int i, j;
        for (i = 0; i < 60; i++) 
		{
            for (int k = 0; k < 15; k++) 
			{
				t[k] = _mm_setzero_ps();
			}
            for (j = 0; j < 60; j++) 
			{
                int jm = j*60;
                matrixTrans = _mm_load1_ps(A+i+jm);
				
                t[0] =  _mm_add_ps(t[0],  _mm_mul_ps(_mm_loadu_ps(A+jm), matrixTrans));
                t[1] =  _mm_add_ps(t[1],  _mm_mul_ps(_mm_loadu_ps(A+4+jm), matrixTrans));
                t[2] =  _mm_add_ps(t[2],  _mm_mul_ps(_mm_loadu_ps(A+8+jm), matrixTrans));
                t[3] =  _mm_add_ps(t[3],  _mm_mul_ps(_mm_loadu_ps(A+12+jm), matrixTrans));
                t[4] =  _mm_add_ps(t[4],  _mm_mul_ps(_mm_loadu_ps(A+16+jm), matrixTrans));
                t[5] =  _mm_add_ps(t[5],  _mm_mul_ps(_mm_loadu_ps(A+20+jm), matrixTrans));
                t[6] =  _mm_add_ps(t[6],  _mm_mul_ps(_mm_loadu_ps(A+24+jm), matrixTrans));
                t[7] =  _mm_add_ps(t[7],  _mm_mul_ps(_mm_loadu_ps(A+28+jm), matrixTrans));
                t[8] =  _mm_add_ps(t[8],  _mm_mul_ps(_mm_loadu_ps(A+32+jm), matrixTrans));
				t[9] =  _mm_add_ps(t[9],  _mm_mul_ps(_mm_loadu_ps(A+36+jm), matrixTrans));
				t[10] = _mm_add_ps(t[10], _mm_mul_ps(_mm_loadu_ps(A+40+jm), matrixTrans));
				t[11] = _mm_add_ps(t[11], _mm_mul_ps(_mm_loadu_ps(A+44+jm), matrixTrans));
				t[12] = _mm_add_ps(t[12], _mm_mul_ps(_mm_loadu_ps(A+48+jm), matrixTrans));
				t[13] = _mm_add_ps(t[13], _mm_mul_ps(_mm_loadu_ps(A+52+jm), matrixTrans));
				t[14] = _mm_add_ps(t[14], _mm_mul_ps(_mm_loadu_ps(A+56+jm), matrixTrans));
            }	
            
            for (int g = 0, h = 0; g < 60, h < 15; g+=4, h++) 
			{
				_mm_storeu_ps(C+g+i*60, t[h]);
			}
        }
        return;
    }
    
    register int i,j,k;
    __m128 trans1;
    __m128 trans2;
    __m128 trans3;
    __m128 colblock;
    __m128 dst;
    __m128 c1, c2, c3;
    
    const int rem_row = m - (m % READLEN);
    const int rem_col = n - (n % READLEN);
    const int newrows = rem_row+READLEN;
    const int newcols = rem_col+READLEN;
    const int padsize = newrows * newcols;
    float padcopy[padsize];
	
    for (j=0; j < n; j++) {
        memcpy((float*)(padcopy+newrows*j), A+m*j, m*sizeof(float));
        memset((float*)(padcopy+newrows*j+m), 0, (newrows-m)*sizeof(float));
    }
    memset((float*)(padcopy+newrows*j), 0, newrows*(newcols-n)*sizeof(float));
	
    for(j = 0; j < m; j+=3) {
        for(k = 0; k < n; k++) {
            trans1 = _mm_load1_ps(padcopy+j+k*newrows);
            trans2 = _mm_load1_ps(padcopy+j+1+k*newrows);
            trans3 = _mm_load1_ps(padcopy+j+2+k*newrows);
            for(i = 0; i < newrows; i+=READLEN) {
                colblock = _mm_loadu_ps(padcopy+k*newrows+i);
                c1 = _mm_mul_ps(trans1,colblock);
                c2 = _mm_mul_ps(trans2,colblock);
                c3 = _mm_mul_ps(trans3,colblock);
                dst = _mm_loadu_ps(C+j*m+i);
                _mm_storeu_ps(C+j*m+i,_mm_add_ps(dst, c1));
                dst = _mm_loadu_ps(C+(j+1)*m+i);
                _mm_storeu_ps(C+(j+1)*m+i,_mm_add_ps(dst,c2));
                dst = _mm_loadu_ps(C+(j+2)*m+i);
                _mm_storeu_ps(C+(j+2)*m+i,_mm_add_ps(dst,c3));
                
                colblock = _mm_loadu_ps(padcopy+k*newrows+i+4);
                c1 = _mm_mul_ps(trans1,colblock);
                c2 = _mm_mul_ps(trans2,colblock);
                c3 = _mm_mul_ps(trans3,colblock);
                dst = _mm_loadu_ps(C+j*m+i+4);
                _mm_storeu_ps(C+j*m+i+4,_mm_add_ps(dst,c1));
                dst = _mm_loadu_ps(C+(j+1)*m+i+4);
                _mm_storeu_ps(C+(j+1)*m+i+4,_mm_add_ps(dst,c2));
                dst = _mm_loadu_ps(C+(j+2)*m+i+4);
                _mm_storeu_ps(C+(j+2)*m+i+4,_mm_add_ps(dst,c3));
                
                colblock = _mm_loadu_ps(padcopy+k*newrows+i+8);
                c1 = _mm_mul_ps(trans1,colblock);
                c2 = _mm_mul_ps(trans2,colblock);
                c3 = _mm_mul_ps(trans3,colblock);
                dst = _mm_loadu_ps(C+j*m+i+8);
                _mm_storeu_ps(C+j*m+i+8,_mm_add_ps(dst,c1));
                dst = _mm_loadu_ps(C+(j+1)*m+i+8);
                _mm_storeu_ps(C+(j+1)*m+i+8,_mm_add_ps(dst,c2));
                dst = _mm_loadu_ps(C+(j+2)*m+i+8);
                _mm_storeu_ps(C+(j+2)*m+i+8,_mm_add_ps(dst,c3));
            }
        }
    }
}
-- 