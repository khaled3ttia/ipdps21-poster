#include <math.h>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include <immintrin.h>
#include <chrono>
#include <string>
#include "khaledUtil.h"

int main(){

    int* nnzRowIdx;
    int* nnzColIdx;
    float* nnzVal;
    // --end of COO arrays
		
    int n, m, nnz; 
    
    std::string filename = "fb.mtx";
	//Load as COO
    loadMTX(nnzRowIdx, nnzColIdx, nnzVal, &nnz, &n, &m, filename);
   
    // The matrix that will hold data
    float **mat = new float*[n];

    for (int i = 0; i<n; i++){
        mat[i] = new float[m];

    }

    // The vector to be multiplied by the matrix
    float *vec = new float[m];

    float *result = new float[n];

    // load the matrix
    for (int i = 0; i < nnz ; i++){
        int row = nnzRowIdx[i];
        int col = nnzColIdx[i];
        mat[row][col] = nnzVal[i];

    }   

    
    for (int i=0; i < n; i++){
        vec[i] = 1;

    }

    // for each row of the matrix, we store the start locations
    // of each pattern of the specified vector size (8 here)
    std::vector<int> startLocations[n];

    for (int i=0; i < n; i++){
        

        findStartLocations(mat[i], m,  startLocations[i], 8);
       
   }

    
    // The actual computation

    auto comp_start_time = std::chrono::high_resolution_clock::now();
    
    for (int i=0; i < n; i++){
        


        for(int loc : startLocations[i]){
            
            // get two 8-float vectors 
            // one from the matrix, and the corresponding 
            // one from the vectore        
            float matSlice[8];

            float vecSlice[8];
            for (int ii = loc; ii < loc+8; ii++){
                matSlice[ii-loc] = mat[i][ii];
                vecSlice[ii-loc] = vec[ii];
            }

            // do multiplication for only non-zeros
            for (int k = 0 ; k < 8; k++){


                if (matSlice[k] == 1){
                    
                    result[i] += matSlice[k] * vecSlice[k];

                }
            }
        }
        
    }
   
    auto comp_end_time = std::chrono::high_resolution_clock::now();

    auto comp_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(comp_end_time - comp_start_time).count();

    // print execution time in nanoseconds
    std::cout <<  comp_duration << std::endl;

    for (int i = 0 ; i< n ; i++) {
        
        delete []  mat[i];

    }
    delete [] mat;

    delete [] vec;

    delete [] result;
}
