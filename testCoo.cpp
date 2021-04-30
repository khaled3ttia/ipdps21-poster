#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include "khaledUtil.h"
#include <chrono>


int main(){

    // datastructures for COO   
    int * nnzRowIdx; 
    int * nnzColIdx; 
    float * nnzVal; 
    int n,m, nnz;

    std::string filename = "fb.mtx";


    // Load the matrix from the mtx file 
    loadMTX(nnzRowIdx, nnzColIdx, nnzVal, &nnz, &n, &m, filename);

    // The dense vector to be multiplied by the matrix
    float *vec = new float[m];

    float *result = new float[n];

    for (int i=0; i < n; i++){
        vec[i] = 1;

    }

    // the actual computation

    auto comp_start_time = std::chrono::high_resolution_clock::now();
    

    for (int i = 0; i < nnz; i++){
        
        int row = nnzRowIdx[i];
        int col = nnzColIdx[i];

        result[row] += nnzVal[i] * vec[col];

    }

   
    auto comp_end_time = std::chrono::high_resolution_clock::now();

    auto comp_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(comp_end_time - comp_start_time).count();

    // print execution time in nanoseconds

    std::cout << comp_duration << std::endl;

    //free memory
    delete [] vec;
    delete [] result;
    delete [] nnzVal;
    delete [] nnzColIdx;
    delete [] nnzRowIdx;
}
