#include <nmmintrin.h>
#include <string.h>
#include <omp.h>
#include <stdio.h>
#define BLOCKSIZE 14
#define PADDING 4
#define JROLL 3
#define BLOCKWIDTH 16 //cache block width (number of columns in the transpose)
#define CSIZE 32768 //L1 size (bytes)
#define ROWWIDTH 3 //horizontal unroll
#define COLWIDTH 2 //vertical unroll

void sgemm(int m, int n, float *A, float *C ) {
	int nCol = m;
    int nRow = n;
    if((n % PADDING) != 0)
        nRow = n - (n % PADDING) + PADDING;
	if (m % JROLL != 0)
        nCol = m - m % JROLL + JROLL;
    const int pSize = nRow * nCol;
	float *newPaddedMatrix = (float *) calloc(pSize, 4);
    int i,j,k,g,h;
    int remainderN = n - (n % BLOCKSIZE);
    int remainderM = m - (m % BLOCKSIZE);
    
    __m128 zVector = _mm_setzero_ps();
	float t[4], t1[4], t2[4];
	int roll_limit = m - (m % ROWWIDTH);
	int kroll_limit = nRow - (nRow % COLWIDTH);
	int jroll_limit = m - (m % JROLL);
	int *p = t;
	int *p1 = t1;
	int *p2 = t2;
    
	#pragma omp parallel firstprivate (i,k,m,n,nRow,remainderN,remainderM,g,h,j)	
    {   
		#pragma omp for nowait schedule(runtime)
        for (i = 0; i < remainderN; i += BLOCKSIZE)
            for (j = 0; j < remainderM; j += BLOCKSIZE)         
                for (g = i; g < i + BLOCKSIZE; g++)
					#pragma omp unroll
                    for (h = j; h < j + BLOCKSIZE; h++)
                        newPaddedMatrix[g+h*nRow] = A[h+g*m];
		
        if (((n % BLOCKSIZE) != 0) || ((m % BLOCKSIZE) != 0)) {
			#pragma omp for schedule (runtime)
            for (g = remainderN; g < n; g++) {
				#pragma omp unroll(m % BLOCKSIZE)
                for (h = 0; h < m; h++) {
                    newPaddedMatrix[g+h*nRow] = A[h+g*m];
                }
            }
			#pragma omp for schedule(runtime)
            for (h = remainderM; h < m; h++) {
				#pragma omp unroll(n % BLOCKSIZE)
                for (g = 0; g < n; g++) {
                    newPaddedMatrix[g+h*nRow] = A[h+g*m];
                }
            }   
        }
		
		#pragma omp for schedule(runtime)
		for (j = 0; j < jroll_limit; j+=JROLL) {  
			for (i = 0; i < roll_limit; i+=ROWWIDTH) {
				__m128 matrixTrans1 = _mm_setzero_ps();
				__m128 matrixTrans2 = _mm_setzero_ps();
				__m128 matrixTrans3 = _mm_setzero_ps();
				__m128 matrixTrans1a = _mm_setzero_ps();
				__m128 matrixTrans2a = _mm_setzero_ps();
				__m128 matrixTrans3a = _mm_setzero_ps();
				__m128 matrixTrans1b = _mm_setzero_ps();
				__m128 matrixTrans2b = _mm_setzero_ps();
				__m128 matrixTrans3b = _mm_setzero_ps();
				#pragma omp unroll
				for(k = 0; k < kroll_limit; k+=4) {
					matrixTrans1 = _mm_add_ps(matrixTrans1, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
					matrixTrans2 = _mm_add_ps(matrixTrans2, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+1)*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
					matrixTrans3 = _mm_add_ps(matrixTrans3, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+2)*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
					matrixTrans1a = _mm_add_ps(matrixTrans1a, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+(j+1)*nRow)));
					matrixTrans2a = _mm_add_ps(matrixTrans2a, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+1)*nRow), *(__m128*)(newPaddedMatrix+k+(j+1)*nRow)));
					matrixTrans3a = _mm_add_ps(matrixTrans3a, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+2)*nRow), *(__m128*)(newPaddedMatrix+k+(j+1)*nRow)));
					matrixTrans1b = _mm_add_ps(matrixTrans1b, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+(j+2)*nRow)));
					matrixTrans2b = _mm_add_ps(matrixTrans2b, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+1)*nRow), *(__m128*)(newPaddedMatrix+k+(j+2)*nRow)));
					matrixTrans3b = _mm_add_ps(matrixTrans3b, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+2)*nRow), *(__m128*)(newPaddedMatrix+k+(j+2)*nRow)));
				}
				
				_mm_storeu_ps(p,matrixTrans1);
				_mm_storeu_ps(p2,matrixTrans2);
				_mm_storeu_ps(p3,matrixTrans3);
				
				*(C+i+j*m)   = t[0]+t[1]+t[2]+t[3];
				*(C+i+1+j*m) = t1[0]+t1[1]+t1[2]+t1[3];
				*(C+i+2+j*m) = t2[0]+t2[1]+t2[2]+t2[3];
				if (j+ 1 < m) { 
					_mm_storeu_ps(p,matrixTrans1a);
					_mm_storeu_ps(p2,matrixTrans2a);
					_mm_storeu_ps(p3,matrixTrans3a);
					
					*(C+i+(j+1)*m)   = t[0]+t[1]+t[2]+t[3];
					*(C+i+1+(j+1)*m) = t1[0]+t1[1]+t1[2]+t1[3];
					*(C+i+2+(j+1)*m) = t2[0]+t2[1]+t2[2]+t2[3];
					if (j+2 < m) {
						_mm_storeu_ps(p,matrixTrans1b);
						_mm_storeu_ps(p2,matrixTrans2b);
						_mm_storeu_ps(p3,matrixTrans3b);
                        
						*(C+i+(j+2)*m)   = t[0]+t[1]+t[2]+t[3];
						*(C+i+1+(j+2)*m) = t1[0]+t1[1]+t1[2]+t1[3];
						*(C+i+2+(j+2)*m) = t2[0]+t2[1]+t2[2]+t2[3];
					}
				}				
			}
			
			for (;i < m;i++) {
				__m128 matrixTrans1 = _mm_setzero_ps();
				__m128 matrixTrans1a = _mm_setzero_ps();
				__m128 matrixTrans1b = _mm_setzero_ps();
				#pragma omp unroll(3)
				for(k = 0; k < nRow; k+=4) {
					matrixTrans1 = _mm_add_ps(matrixTrans1, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+j*nRow)));
					matrixTrans1a = _mm_add_ps(matrixTrans1a, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+(j+1)*nRow)));
					matrixTrans1b = _mm_add_ps(matrixTrans1b, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+(j+2)*nRow)));
				}
				
				_mm_storeu_ps(p,matrixTrans1);
				*(C+i+j*m) = t[0]+t[1]+t[2]+t[3];
				if (j+1 < m) {
					_mm_storeu_ps(p2,matrixTrans1a);
					*(C+i+(j+1)*m) = t1[0]+t1[1]+t1[2]+t1[3];       
					if (j+2 < m) {
						_mm_storeu_ps(p3,matrixTrans1b);
						*(C+i+(j+2)*m) = t2[0]+t2[1]+t2[2]+t2[3];       
					}
				}
			}       
		}
		
		#pragma omp for
		for (j = jroll_limit; j < m; j+=1) {  
			for (i = 0; i < roll_limit; i+=ROWWIDTH) {
				__m128 matrixTrans1 = _mm_setzero_ps();
				__m128 matrixTrans2 = _mm_setzero_ps();
				__m128 matrixTrans3 = _mm_setzero_ps();
				#pragma omp unroll
				for(k = 0; k < kroll_limit; k+=4) {
					matrixTrans1 = _mm_add_ps(matrixTrans1, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
					matrixTrans2 = _mm_add_ps(matrixTrans2, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+1)*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
					matrixTrans3 = _mm_add_ps(matrixTrans3, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+(i+2)*nRow), _mm_loadu_ps(newPaddedMatrix+k+j*nRow)));
				}
				for (;i < m;i++) {
					__m128 matrixTrans1 = _mm_setzero_ps();
					#pragma omp unroll(3)
					for(k = 0; k < nRow; k+=4) {
						matrixTrans1 = _mm_add_ps(matrixTrans1, _mm_mul_ps(*(__m128*)(newPaddedMatrix+k+i*nRow), *(__m128*)(newPaddedMatrix+k+j*nRow)));
					}
					_mm_storeu_ps(p,matrixTrans1);
					*(C+i+j*m) = t[0]+t[1]+t[2]+t[3];
				}
			}
		} 
	}
    free(newPaddedMatrix);
	
}