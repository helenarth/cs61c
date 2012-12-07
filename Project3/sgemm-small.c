/*
 *  sgemm-small.c
 *  Project3
 *
 *  Created by Nathan brummel (cs61c-mh) & Narek Isaghulyan (cs61c-nh) on 10/18/12.
 *  Copyright 2012 Berkeley. All rights reserved.
 *
 */

#include <emmintrin.h>
#include <string.h>

void sgemm(int m, int n, float *A, float *C ) 
{
    if(m==60 & n==60) 
	{
        __m128 t[15];
        __m128 matrixTrans;
        register int i, j;
        for (i = 0; i < 60; i++) {
			
			t[0] = _mm_setzero_ps();		//sets temp matrix t to 0 for all elements.
			t[1] = _mm_setzero_ps();
			t[2] = _mm_setzero_ps();
			t[3] = _mm_setzero_ps();
			t[4] = _mm_setzero_ps();
			t[5] = _mm_setzero_ps();
			t[6] = _mm_setzero_ps();
			t[7] = _mm_setzero_ps();
			t[8] = _mm_setzero_ps();
			t[9] = _mm_setzero_ps();
			t[10] = _mm_setzero_ps();
			t[11] = _mm_setzero_ps();
			t[12] = _mm_setzero_ps();
			t[13] = _mm_setzero_ps();
			t[14] = _mm_setzero_ps();
	
            for (j = 0; j < 60; j++) {
				
                int blockTimes60 = j*60;
                matrixTrans = _mm_load1_ps(A+i+blockTimes60);	//loads A, performs mm, and into temp t.
				
                t[0] =  _mm_add_ps(t[0],  _mm_mul_ps(_mm_loadu_ps(A+blockTimes60), matrixTrans));
                t[1] =  _mm_add_ps(t[1],  _mm_mul_ps(_mm_loadu_ps(A+4+blockTimes60), matrixTrans));
                t[2] =  _mm_add_ps(t[2],  _mm_mul_ps(_mm_loadu_ps(A+8+blockTimes60), matrixTrans));
                t[3] =  _mm_add_ps(t[3],  _mm_mul_ps(_mm_loadu_ps(A+12+blockTimes60), matrixTrans));
                t[4] =  _mm_add_ps(t[4],  _mm_mul_ps(_mm_loadu_ps(A+16+blockTimes60), matrixTrans));
                t[5] =  _mm_add_ps(t[5],  _mm_mul_ps(_mm_loadu_ps(A+20+blockTimes60), matrixTrans));
                t[6] =  _mm_add_ps(t[6],  _mm_mul_ps(_mm_loadu_ps(A+24+blockTimes60), matrixTrans));
                t[7] =  _mm_add_ps(t[7],  _mm_mul_ps(_mm_loadu_ps(A+28+blockTimes60), matrixTrans));
                t[8] =  _mm_add_ps(t[8],  _mm_mul_ps(_mm_loadu_ps(A+32+blockTimes60), matrixTrans));
				t[9] =  _mm_add_ps(t[9],  _mm_mul_ps(_mm_loadu_ps(A+36+blockTimes60), matrixTrans));
				t[10] = _mm_add_ps(t[10], _mm_mul_ps(_mm_loadu_ps(A+40+blockTimes60), matrixTrans));
				t[11] = _mm_add_ps(t[11], _mm_mul_ps(_mm_loadu_ps(A+44+blockTimes60), matrixTrans));
				t[12] = _mm_add_ps(t[12], _mm_mul_ps(_mm_loadu_ps(A+48+blockTimes60), matrixTrans));
				t[13] = _mm_add_ps(t[13], _mm_mul_ps(_mm_loadu_ps(A+52+blockTimes60), matrixTrans));
				t[14] = _mm_add_ps(t[14], _mm_mul_ps(_mm_loadu_ps(A+56+blockTimes60), matrixTrans));
            }	
            
			for (int g = 0, h = 0; g < 60, h < 15; g+=12, h+=3) {		//stores t into C, tried to unroll more but found 
																		//3 to be the best (nice factor of 60 and 15) was
				_mm_storeu_ps(C+g+i*60, t[h]);							//better than 5.
				_mm_storeu_ps(C+g+4+i*60, t[h+1]);
				_mm_storeu_ps(C+g+8+i*60, t[h+2]);
			}
        }
        return;
    }
    
    register int i,j,k;
    __m128 matrixTransA, matrixTransB, matrixTransC, dataBlock;
	const int nRow = m + 12 - (m % 12);
    const int nCol = n + 12 - (n % 12);
    float newPaddedMatrix[nRow * nCol];
	
    for (j=0; j < n; j++) {								//Pads the matrix with 0's to deal with edge cases.
		
        memmove((float*)(newPaddedMatrix+nRow*j), A+m*j, m*sizeof(float));
        memset((float*)(newPaddedMatrix+nRow*j+m), 0, (nRow-m)*sizeof(float));
		
	}
	//don't think I need this; did before.
    //memset((float*)(cPadding+nRow*j), 0, nRow*(nCol-n)*sizeof(float));
	
    for(j = 0; j < m; j+=3) {							//performs mm with the new padded matrix.
        for(k = 0; k < n; k++) {						//loads the blocks to be worked on.
            
			matrixTransA = _mm_load1_ps(newPaddedMatrix+j+k*nRow);
            matrixTransB = _mm_load1_ps(newPaddedMatrix+j+1+k*nRow);
            matrixTransC = _mm_load1_ps(newPaddedMatrix+j+2+k*nRow);
			
            for(i = 0; i < nRow/24*24; i+=24) {			//Performs operations
				
                dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i);
                _mm_storeu_ps(C+j*m+i,_mm_add_ps(_mm_loadu_ps(C+j*m+i), _mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i),_mm_mul_ps(matrixTransC,dataBlock)));
                
                dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+4);
                _mm_storeu_ps(C+j*m+i+4,_mm_add_ps(_mm_loadu_ps(C+j*m+i+4),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+4,_mm_add_ps( _mm_loadu_ps(C+(j+1)*m+i+4),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+4,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+4),_mm_mul_ps(matrixTransC,dataBlock)));
                
                dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+8);
                _mm_storeu_ps(C+j*m+i+8,_mm_add_ps(_mm_loadu_ps(C+j*m+i+8),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+8,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i+8),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+8,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+8),_mm_mul_ps(matrixTransC,dataBlock)));
				
				dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+12);
                _mm_storeu_ps(C+j*m+i+12,_mm_add_ps(_mm_loadu_ps(C+j*m+i+12),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+12,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i+12),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+12,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+12),_mm_mul_ps(matrixTransC,dataBlock)));
				
				dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+16);
                _mm_storeu_ps(C+j*m+i+16,_mm_add_ps(_mm_loadu_ps(C+j*m+i+16),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+16,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i+16),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+16,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+16),_mm_mul_ps(matrixTransC,dataBlock)));
				
				dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+20);
                _mm_storeu_ps(C+j*m+i+20,_mm_add_ps(_mm_loadu_ps(C+j*m+i+20),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+20,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i+20),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+20,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+20),_mm_mul_ps(matrixTransC,dataBlock)));
            }
			for(i = nRow/24*24; i < nRow; i+=12) {		//Performs operations
				
                dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i);
                _mm_storeu_ps(C+j*m+i,_mm_add_ps(_mm_loadu_ps(C+j*m+i), _mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i),_mm_mul_ps(matrixTransC,dataBlock)));
				
				dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+4);
                _mm_storeu_ps(C+j*m+i+4,_mm_add_ps(_mm_loadu_ps(C+j*m+i+4),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+4,_mm_add_ps( _mm_loadu_ps(C+(j+1)*m+i+4),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+4,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+4),_mm_mul_ps(matrixTransC,dataBlock)));
                
                dataBlock = _mm_loadu_ps(newPaddedMatrix+k*nRow+i+8);
                _mm_storeu_ps(C+j*m+i+8,_mm_add_ps(_mm_loadu_ps(C+j*m+i+8),_mm_mul_ps(matrixTransA,dataBlock)));
                _mm_storeu_ps(C+(j+1)*m+i+8,_mm_add_ps(_mm_loadu_ps(C+(j+1)*m+i+8),_mm_mul_ps(matrixTransB,dataBlock)));
                _mm_storeu_ps(C+(j+2)*m+i+8,_mm_add_ps(_mm_loadu_ps(C+(j+2)*m+i+8),_mm_mul_ps(matrixTransC,dataBlock)));
			}
        }
    }
}