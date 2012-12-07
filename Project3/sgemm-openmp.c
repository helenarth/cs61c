/*
 *  sgemm-openmp.c
 *  Project3-2
 *
 *  Created by Nathan brummel (cs61c-mh) & Narek Isaghulyan (cs61c-nh) on 10/18/12.
 *  Copyright 2012 Berkeley. All rights reserved.
 *
 */


#include <nmmintrin.h>
#include <string.h>
#include <omp.h>
#include <stdio.h>
#define bSize 14
#define EXTRAPADDING 4
#define JROLL 2
#define ROWWIDTH 4 
#define COLWIDTH 2 

void sgemm(int m, int n, float *A, float *C ) {
	
    int nCol = m;
    int nRow = n;
    if((n % EXTRAPADDING) != 0)
        nRow = n-(n % EXTRAPADDING)+EXTRAPADDING;
    if ((m % JROLL) != 0)
        nCol = m-(m % JROLL)+JROLL;
    const int padsize = nRow * nCol;
    float *padtrans = (float *) calloc(padsize, 4);
    int i,j,k,g, h;
    int remainingN = n - (n % bSize);
    int remainingM = m - (m % bSize);
    __m128 zerovec = _mm_setzero_ps();
	
	
#pragma omp parallel firstprivate (i,k,m,n,nRow,remainingN,remainingM,g,h,j)
    {   
#pragma omp for nowait schedule(runtime)
        for (i = 0; i < remainingN; i += bSize)
            for (j = 0; j < remainingM; j += bSize)         
                for (g = i; g < i + bSize; g++)
#pragma omp unroll
                    for (h = j; h < j + bSize; h++)
                        padtrans[g+h*nRow] = A[h+g*m];
		
        if (((n % bSize) != 0) || ((m % bSize) != 0)) {
#pragma omp for schedule (runtime)
            for (g = remainingN; g < n; g++) {
				
                for (h = 0; h < m; h++) {
                    padtrans[g+h*nRow] = A[h+g*m];
                }
            }
            
            for (h = remainingM; h < m; h++) {
                
                for (g = 0; g < n; g++) {
                    padtrans[g+h*nRow] = A[h+g*m];
                }
            }   
        }
		float t0[4], t1[4], t2[4], t3[4];
		
        int roll_limit = m - (m % ROWWIDTH);
        int kroll_limit = nRow - (nRow % COLWIDTH);
        int jroll_limit = m - (m%JROLL);
        
        
#pragma omp for schedule(runtime)
        for (j = 0; j < m; j+=JROLL) {  
            for (i = 0; i < roll_limit; i+=ROWWIDTH) {
                __m128 localsum1 = _mm_setzero_ps();
                __m128 localsum2 = _mm_setzero_ps();
                __m128 localsum3 = _mm_setzero_ps();
                __m128 localsum4 = _mm_setzero_ps();
                __m128 localsum1a = _mm_setzero_ps();
                __m128 localsum2a = _mm_setzero_ps();
                __m128 localsum3a = _mm_setzero_ps();
                __m128 localsum4a =  _mm_setzero_ps();
				#pragma omp unroll
                for(k = 0; k < kroll_limit; k+=4) {
                    localsum1 = _mm_add_ps(localsum1, _mm_mul_ps(*(__m128*)(padtrans+k+i*nRow), _mm_loadu_ps(padtrans+k+j*nRow)));
                    localsum2 = _mm_add_ps(localsum2, _mm_mul_ps(*(__m128*)(padtrans+k+(i+1)*nRow), _mm_loadu_ps(padtrans+k+j*nRow)));
                    localsum3 = _mm_add_ps(localsum3, _mm_mul_ps(*(__m128*)(padtrans+k+(i+2)*nRow), _mm_loadu_ps(padtrans+k+j*nRow)));
                    localsum4 = _mm_add_ps(localsum4, _mm_mul_ps(*(__m128*)(padtrans+k+(i+3)*nRow), _mm_loadu_ps(padtrans+k+j*nRow)));
                    localsum1a = _mm_add_ps(localsum1a, _mm_mul_ps(*(__m128*)(padtrans+k+i*nRow), *(__m128*)(padtrans+k+(j+1)*nRow)));
                    localsum2a = _mm_add_ps(localsum2a, _mm_mul_ps(*(__m128*)(padtrans+k+(i+1)*nRow), *(__m128*)(padtrans+k+(j+1)*nRow)));
                    localsum3a = _mm_add_ps(localsum3a, _mm_mul_ps(*(__m128*)(padtrans+k+(i+2)*nRow), *(__m128*)(padtrans+k+(j+1)*nRow)));
                    localsum4a = _mm_add_ps(localsum4a, _mm_mul_ps(*(__m128*)(padtrans+k+(i+3)*nRow), *(__m128*)(padtrans+k+(j+1)*nRow)));
					
				}
                _mm_storeu_ps(t0,localsum1);
                _mm_storeu_ps(t1,localsum2);
                _mm_storeu_ps(t2,localsum3);
                _mm_storeu_ps(t3,localsum4);
				
                *(C+i+j*m)   = t0[0]+t0[1]+t0[2]+t0[3];
                *(C+i+1+j*m) = t1[0]+t1[1]+t1[2]+t1[3];
                *(C+i+2+j*m) = t2[0]+t2[1]+t2[2]+t2[3];
                *(C+i+3+j*m) = t3[0]+t3[1]+t3[2]+t3[3];
				
                if (j+1 < m) {
                    _mm_storeu_ps(t0,localsum1a);
                    _mm_storeu_ps(t1,localsum2a);
                    _mm_storeu_ps(t2,localsum3a);
                    _mm_storeu_ps(t3,localsum4a);
					
                    *(C+i+(j+1)*m)   = t0[0]+t0[1]+t0[2]+t0[3];
                    *(C+i+1+(j+1)*m) = t1[0]+t1[1]+t1[2]+t1[3];
                    *(C+i+2+(j+1)*m) = t2[0]+t2[1]+t2[2]+t2[3];
                    *(C+i+3+(j+1)*m) = t3[0]+t3[1]+t3[2]+t3[3];
                }
				
            }
#pragma omp unroll(3)
            for (;i < m;i++) {
                __m128 localsum1 = _mm_setzero_ps();
                __m128 localsum1a = _mm_setzero_ps();
#pragma omp unroll(3)
                for(k = 0; k < nRow; k+=4) {
                    localsum1 = _mm_add_ps(localsum1, _mm_mul_ps(*(__m128*)(padtrans+k+i*nRow), *(__m128*)(padtrans+k+j*nRow)));
                    localsum1a = _mm_add_ps(localsum1a, _mm_mul_ps(*(__m128*)(padtrans+k+i*nRow), *(__m128*)(padtrans+k+(j+1)*nRow)));
                }
				
                _mm_storeu_ps(t0,localsum1);
                *(C+i+j*m) = t0[0]+t0[1]+t0[2]+t0[3];
                if (j+1 < m) {
                    _mm_storeu_ps(t0,localsum1a);
                    *(C+i+(j+1)*m) = t0[0]+t0[1]+t0[2]+t0[3];       
                }
				
            }   
        }
    }
    free(padtrans);
}