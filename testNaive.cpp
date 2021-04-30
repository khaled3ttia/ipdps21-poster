#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include "khaledUtil.h"
#include <chrono>



int main(){

   
    int * nnzRowIdx; 
    int * nnzColIdx; 
    float * nnzVal; 
    int n,m, nnz;
    std::string filename = "fb.mtx";

    loadMTX(nnzRowIdx, nnzColIdx, nnzVal, &nnz, &n, &m, filename);

    float **mat = new float*[n];

    for (int i = 0 ; i < n ; i++){
        
        mat[i] = new float[m];

    }

    float *vec = new float[m];
    float *result = new float[n];

    for (int i = 0; i<nnz; i++){
        int row = nnzRowIdx[i];
        int col = nnzColIdx[i];

        mat[row][col] = nnzVal[i];

    }

    for (int i=0; i < n; i++){
        vec[i] = 1;

    }

   
    auto comp_start_time = std::chrono::high_resolution_clock::now();
    
    for (int i=0; i < n; i++){
        
        for (int j = 0 ; j < m; j++){
            
            result[i] += mat[i][j] * vec[j];

        }
    }
        

    
    auto comp_end_time = std::chrono::high_resolution_clock::now();

    auto comp_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(comp_end_time - comp_start_time).count();

    std::cout <<  comp_duration << std::endl;

    for (int i = 0 ; i < n ; i++){
        delete [] mat[i];
    }

    delete [] mat;
    delete [] vec;
    delete [] result;
    delete [] nnzVal;
    delete [] nnzColIdx;
    delete [] nnzRowIdx;
}
