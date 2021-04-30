#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include <immintrin.h>
#include <chrono>
#include "khaledUtil.h"

// functions I used for debugging
void print128_float(__m128 reg){

    float val[4];

    memcpy(val, &reg, sizeof(val));

    std::cout << val[0] << "|" << val[1] << "|" << val[2] << "|" << val[3] << std::endl;

}

void print256_float(__m256 reg){

    float val[8];
       

    memcpy(val, &reg, sizeof(val));
    std::cout << "Intrinsic version:" << std::endl;
    std::cout << val[0] << " | " << val[1] << " | " << val[2] << " | " << val[3] << "| " << val[4] << " | " << val[5] << " | " << val[6] << " | " << val[7] << std::endl; 

}
/*
void print512_float(__m512 reg){

    float val[16];
       

    memcpy(val, &reg, sizeof(val));
    std::cout << "Intrinsic version:" << std::endl;
    std::cout << val[0] << " | " << val[1] << " | " << val[2] << " | " << val[3] << "| " << val[4] << " | " << val[5] << " | " << val[6] << " | " << val[7] << " | " << val[8] << " | " << val[9] << " | " << val[10] << " | " << val[11] << " | " << val[12] << " | " << val[13] << " | " << val[14] << " | " << val[15] <<  std::endl; 

}
*/


int main(){

    int * nnzRowIdx;
    int * nnzColIdx;
    float * nnzVal;
    int n, m,nnz; 
    std::string filename = "fb.mtx";

    loadMTX(nnzRowIdx, nnzColIdx, nnzVal, &nnz, &n, &m, filename);

    float **mat = new float*[n];

    for (int i = 0 ; i<n ; i++){
        mat[i] = new float[m];
    }

    float *vec = new float[m];
    float *result = new float[n];


    for (int i = 0 ; i < nnz; i++){
        int row = nnzRowIdx[i];

        int col = nnzColIdx[i];

        mat[row][col] = nnzVal[i];


    }

    for (int i=0; i < n; i++){
        vec[i] = 1;

    }

    std::vector<int> startLocations[n];

    for (int i=0; i < n; i++){
        

        findStartLocations(mat[i], m,  startLocations[i], 8);
       

    }

    
    // actual computation
    auto comp_start_time = std::chrono::high_resolution_clock::now();
    //std::cout << " value of n is : " << n << std::endl;    
    for (int i=0; i < n; i++){
        
        // set the accumlator to zero

        __m256 acc = _mm256_setzero_ps();

        for(int loc : startLocations[i]){
            //std::cout << "i:" << i << " loc:" << loc <<std::endl;        
            __m256 matV, vecV;

            // load 8 values starting from the start index (loc)
            // from the matrix

            matV = _mm256_loadu_ps(&(mat[i][loc]));
            
            // load the corresponding 8 values from the vector

            vecV = _mm256_loadu_ps(&vec[loc]);
            
            // do the multiply-add operation at once

            acc = _mm256_fmadd_ps(matV, vecV, acc);

        }
        

        // all this code is just to reduce the sum at each row

        const __m128 hiQuad = _mm256_extractf128_ps(acc, 1);

        const __m128 loQuad = _mm256_castps256_ps128(acc);

        const __m128 sumQuad = _mm_add_ps(loQuad, hiQuad);

        const __m128 loDual = sumQuad;

        const __m128 hiDual = _mm_movehl_ps(sumQuad, sumQuad);

        const __m128 sumDual = _mm_add_ps(loDual, hiDual);

        const __m128 lo = sumDual;

        const __m128 hi = _mm_shuffle_ps(sumDual, sumDual, 0x1);

        const __m128 sum = _mm_add_ss(lo, hi);
       
        result[i] = sum[0];

    }
   
    auto comp_end_time = std::chrono::high_resolution_clock::now();

    auto comp_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(comp_end_time - comp_start_time).count();

    // print execution time in nanoseconds
    std::cout <<  comp_duration << std::endl;

    for (int i = 0; i < n; i++){
        delete [] mat[i];

    }
    delete [] mat;
    delete [] vec;
    delete [] result;
    delete [] nnzVal;
    delete [] nnzColIdx;
    delete [] nnzRowIdx;
}
